/*
 *      Copyright (C) 2017 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "config_kodi.h"
#ifdef HAVE_QT_GUI

#include "RPProcessInfoQt.h"

using namespace KODI;
using namespace RETRO;

CRPProcessInfoQt::CRPProcessInfoQt() :
  CRPProcessInfo("Qt")
{
}

CRPProcessInfo* CRPProcessInfoQt::Create()
{
    return new CRPProcessInfoQt();
}

void CRPProcessInfoQt::Register()
{
    CRPProcessInfo::RegisterProcessControl(CRPProcessInfoQt::Create);
}

#endif // HAVE_QT_GUI
