/*
 *  Copyright (C) 2011-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "HTTPVfsHandler.h"
#include "MediaSource.h"
#include "NlcUrl.h"
#include "filesystem/File.h"
#include "network/WebServer.h"
#include "settings/MediaSourceSettings.h"
#include "utils/URIUtils.h"

CHTTPVfsHandler::CHTTPVfsHandler(const HTTPRequest &request)
  : CHTTPFileHandler(request)
{
  std::string file;
  int responseStatus = MHD_HTTP_BAD_REQUEST;

  if (m_request.pathUrl.size() > 5)
  {
    file = m_request.pathUrl.substr(5);

    if (XFILE::CFile::Exists(file))
    {
      bool accessible = false;
      if (file.substr(0, 8) == "image://")
        accessible = true;
      else
      {
        std::string sourceTypes[] = { "video", "music", "pictures" };
        unsigned int size = sizeof(sourceTypes) / sizeof(std::string);

        std::string realPath = URIUtils::GetRealPath(file);
        // for rar:// and zip:// paths we need to extract the path to the archive instead of using the VFS path
        while (URIUtils::IsInArchive(realPath))
          realPath = NlcUrl(realPath).GetHostName();

        VECSOURCES *sources = NULL;
        for (unsigned int index = 0; index < size && !accessible; index++)
        {
          sources = CMediaSourceSettings::GetInstance().GetSources(sourceTypes[index]);
          if (sources == NULL)
            continue;

          for (VECSOURCES::const_iterator source = sources->begin(); source != sources->end() && !accessible; ++source)
          {
            // don't allow access to locked / disabled sharing sources
            if (source->m_iHasLock == 2 || !source->m_allowSharing)
              continue;

            for (std::vector<std::string>::const_iterator path = source->vecPaths.begin(); path != source->vecPaths.end(); ++path)
            {
              std::string realSourcePath = URIUtils::GetRealPath(*path);
              if (URIUtils::PathHasParent(realPath, realSourcePath, true))
              {
                accessible = true;
                break;
              }
            }
          }
        }
      }

      if (accessible)
        responseStatus = MHD_HTTP_OK;
      // the file exists but not in one of the defined sources so we deny access to it
      else
        responseStatus = MHD_HTTP_UNAUTHORIZED;
    }
    else
      responseStatus = MHD_HTTP_NOT_FOUND;
  }

  // set the file and the HTTP response status
  SetFile(file, responseStatus);
}

bool CHTTPVfsHandler::CanHandleRequest(const HTTPRequest &request) const
{
  return request.pathUrl.find("/vfs") == 0;
}
