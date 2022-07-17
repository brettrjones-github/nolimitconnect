/*
 *      Copyright (c) 2002 Frodo
 *      Portions Copyright (c) by the authors of ffmpeg and xvid
 *  Copyright (C) 2002-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// FilePipe.h: interface for the CPipeFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPIPE_H__DD2B0A9E_4971_4A29_B525_78CEFCDAF4A1__INCLUDED_)
#define AFX_FILEPIPE_H__DD2B0A9E_4971_4A29_B525_78CEFCDAF4A1__INCLUDED_

#include <string>
#include <vector>

#include "IFile.h"
#include "threads/CriticalSection.h"
#include "PipesManager.h"

namespace XFILE
{
  
class CPipeFile : public IFile, public IPipeListener
{
public:
  CPipeFile();
  ~CPipeFile() override;
  int64_t GetPosition() override;
  int64_t GetLength() override;
  virtual void SetLength(int64_t len);
  bool Open(const NlcUrl& url) override;
  bool Exists(const NlcUrl& url) override;
  int Stat(const NlcUrl& url, struct __stat64* buffer) override;
  int Stat(struct __stat64* buffer) override;
  ssize_t Read(void* lpBuf, size_t uiBufSize) override;
  ssize_t Write(const void* lpBuf, size_t uiBufSize) override;
  int64_t Seek(int64_t iFilePosition, int iWhence = SEEK_SET) override;
  void Close() override;
  void Flush() override;
  virtual int64_t GetAvailableRead();

  bool OpenForWrite(const NlcUrl& url, bool bOverWrite = false) override;

  bool Delete(const NlcUrl& url) override;
  bool Rename(const NlcUrl& url, const NlcUrl& urlnew) override;
  int IoControl(EIoControl request, void* param) override;
  
  std::string GetName() const;
  
  void OnPipeOverFlow() override;
  void OnPipeUnderFlow() override;

  void AddListener(IPipeListener *l);
  void RemoveListener(IPipeListener *l);

  void SetEof();
  bool IsEof();
  bool IsEmpty();
  bool IsClosed();
  
  void SetOpenThreshold(int threshold);

protected:
  int64_t m_pos = 0;
  int64_t m_length = -1;
  
  XFILE::Pipe *m_pipe;
  
  CCriticalSection m_lock;
  std::vector<XFILE::IPipeListener *> m_listeners;
};

}
#endif // !defined(AFX_FILEPIPE_H__DD2B0A9E_4971_4A29_B525_78CEFCDAF4A1__INCLUDED_)
