/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "DVDDemux.h"
#include "threads/CriticalSection.h"
#include "threads/SystemClock.h"
#include <map>
#include <memory>
#include <vector>

extern "C" {
#include "libavformat/avformat.h"
}

class CDVDDemuxFFmpeg;
class GoTvUrl;

class CDemuxStreamVideoFFmpeg : public CDemuxStreamVideo
{
public:
  explicit CDemuxStreamVideoFFmpeg(AVStream* stream) : m_stream(stream) {}
  std::string GetStreamName() override;

  std::string m_description;
protected:
  AVStream* m_stream = nullptr;
};

class CDemuxStreamAudioFFmpeg : public CDemuxStreamAudio
{
public:
  explicit CDemuxStreamAudioFFmpeg(AVStream* stream) : m_stream(stream) {}
  std::string GetStreamName() override;

  std::string m_description;
protected:
  CDVDDemuxFFmpeg *m_parent = nullptr;
  AVStream* m_stream  = nullptr;
};

class CDemuxStreamSubtitleFFmpeg
  : public CDemuxStreamSubtitle
{
public:
  explicit CDemuxStreamSubtitleFFmpeg(AVStream* stream) : m_stream(stream) {}
  std::string GetStreamName() override;

  std::string m_description;
protected:
  CDVDDemuxFFmpeg *m_parent = nullptr;
  AVStream* m_stream = nullptr;
};

class CDemuxParserFFmpeg
{
public:
  ~CDemuxParserFFmpeg();
  AVCodecParserContext* m_parserCtx = nullptr;
  AVCodecContext* m_codecCtx = nullptr;
};

#define FFMPEG_DVDNAV_BUFFER_SIZE 2048  // for dvd's

struct StereoModeConversionMap;

class CDVDDemuxFFmpeg : public CDVDDemux
{
public:
  CDVDDemuxFFmpeg();
  ~CDVDDemuxFFmpeg() override;

  bool Open(std::shared_ptr<CDVDInputStream> pInput, bool streaminfo = true, bool fileinfo = false);
  void Dispose();
  bool Reset() override ;
  void Flush() override;
  void Abort() override;
  void SetSpeed(int iSpeed) override;
  std::string GetFileName() override;

  DemuxPacket* Read() override;

  bool SeekTime(double time, bool backwards = false, double* startpts = NULL) override;
  bool SeekByte(int64_t pos);
  int GetStreamLength() override;
  CDemuxStream* GetStream(int iStreamId) const override;
  std::vector<CDemuxStream*> GetStreams() const override;
  int GetNrOfStreams() const override;
  int GetPrograms(std::vector<ProgramInfo>& programs) override;
  void SetProgram(int progId) override;

  bool SeekChapter(int chapter, double* startpts = NULL) override;
  int GetChapterCount() override;
  int GetChapter() override;
  void GetChapterName(std::string& strChapterName, int chapterIdx=-1) override;
  int64_t GetChapterPos(int chapterIdx=-1) override;
  std::string GetStreamCodecName(int iStreamId) override;

  bool Aborted();

  AVFormatContext* m_pFormatContext;
  std::shared_ptr<CDVDInputStream> m_pInput;

protected:
  friend class CDemuxStreamAudioFFmpeg;
  friend class CDemuxStreamVideoFFmpeg;
  friend class CDemuxStreamSubtitleFFmpeg;

  CDemuxStream* AddStream(int streamIdx);
  void AddStream(int streamIdx, CDemuxStream* stream);
  void CreateStreams(unsigned int program = UINT_MAX);
  void DisposeStreams();
  void ParsePacket(AVPacket *pkt);
  bool IsVideoReady();
  void ResetVideoStreams();
  AVDictionary *GetFFMpegOptionsFromInput();
  double ConvertTimestamp(int64_t pts, int den, int num);
  void UpdateCurrentPTS();
  bool IsProgramChange();
  unsigned int HLSSelectProgram();

  std::string GetStereoModeFromMetadata(AVDictionary *pMetadata);
  std::string ConvertCodecToInternalStereoMode(const std::string &mode, const StereoModeConversionMap *conversionMap);

  void GetL16Parameters(int &channels, int &samplerate);
  double SelectAspect(AVStream* st, bool& forced);

  CCriticalSection m_critSection;
  std::map<int, CDemuxStream*> m_streams;
  std::map<int, std::unique_ptr<CDemuxParserFFmpeg>> m_parsers;

  AVIOContext* m_ioContext = nullptr;

  double   m_currentPts = 0; // used for stream length estimation
  bool     m_bMatroska = false;
  bool     m_bAVI = false;
  bool     m_bSup = false;
  int      m_speed = 0;
  unsigned int m_program = 0;
  unsigned int m_streamsInProgram = 0;
  unsigned int m_newProgram = 0;

  XbmcThreads::EndTime  m_timeout;

  // Due to limitations of ffmpeg, we only can detect a program change
  // with a packet. This struct saves the packet for the next read and
  // signals STREAMCHANGE to player
  struct
  {
    AVPacket pkt;       // packet ffmpeg returned
    int      result;    // result from av_read_packet
  }m_pkt;

  bool m_streaminfo = false;
  bool m_checkvideo = false;
  int m_displayTime = 0;
  double m_dtsAtDisplayTime = 0;
  bool m_seekToKeyFrame = false;
  double m_startTime = 0;
};

