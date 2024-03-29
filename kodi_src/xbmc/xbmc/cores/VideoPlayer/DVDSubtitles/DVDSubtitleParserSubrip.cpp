/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DVDSubtitleParserSubrip.h"
#include "cores/VideoPlayer/DVDCodecs/Overlay/DVDOverlayText.h"
#include "cores/VideoPlayer/Interface/Addon/TimingConstants.h"
#include "utils/StringUtils.h"
#include "DVDSubtitleTagSami.h"

CDVDSubtitleParserSubrip::CDVDSubtitleParserSubrip(std::unique_ptr<CDVDSubtitleStream> && pStream, const std::string& strFile)
    : CDVDSubtitleParserText(std::move(pStream), strFile)
{
}

CDVDSubtitleParserSubrip::~CDVDSubtitleParserSubrip()
{
  Dispose();
}

bool CDVDSubtitleParserSubrip::Open(CDVDStreamInfo &hints)
{
  if (!CDVDSubtitleParserText::Open())
    return false;

  CDVDSubtitleTagSami TagConv;
  if (!TagConv.Init())
    return false;

  char line[1024];
  std::string strLine;

  while (m_pStream->ReadLine(line, sizeof(line)))
  {
    strLine = line;
    StringUtils::Trim(strLine);

    if (strLine.length() > 0)
    {
      char sep;
      int hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2;
      int c = sscanf(strLine.c_str(), "%d%c%d%c%d%c%d --> %d%c%d%c%d%c%d\n",
                     &hh1, &sep, &mm1, &sep, &ss1, &sep, &ms1,
                     &hh2, &sep, &mm2, &sep, &ss2, &sep, &ms2);

      if (c == 1)
      {
        // numbering, skip it
      }
      else if (c == 14) // time info
      {
        CDVDOverlayText* pOverlay = new CDVDOverlayText();
        pOverlay->Acquire(); // increase ref count with one so that we can hold a handle to this overlay

        pOverlay->iPTSStartTime = ((double)(((hh1 * 60 + mm1) * 60) + ss1) * 1000 + ms1) * (DVD_TIME_BASE / 1000);
        pOverlay->iPTSStopTime  = ((double)(((hh2 * 60 + mm2) * 60) + ss2) * 1000 + ms2) * (DVD_TIME_BASE / 1000);

        while (m_pStream->ReadLine(line, sizeof(line)))
        {
          strLine = line;
          StringUtils::Trim(strLine);

          // empty line, next subtitle is about to start
          if (strLine.length() <= 0) break;

          TagConv.ConvertLine(pOverlay, strLine.c_str(), strLine.length());
        }
        TagConv.CloseTag(pOverlay);
        m_collection.Add(pOverlay);
      }
    }
  }
  m_collection.Sort();
  return true;
}

