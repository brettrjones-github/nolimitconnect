#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "DynamicDll.h"

///////////////////////////////////////////////////////////
//
//  class for a conversion of Dynamically loaded dll to static lib linkage
//
class StaticDll : public DllDynamic, public LibraryLoader
{
public:
  StaticDll() = delete;
  explicit StaticDll(const std::string& strDllName);
  virtual ~StaticDll();
  bool Load() override;
  void Unload() override;
  bool IsLoaded() const  override;
  bool CanLoad() ;
  bool EnableDelayedUnload(bool bOnOff) ;
  bool SetFile(const std::string& strDllName);

  // overrides of Library Loader
  //virtual bool Load() = 0;
  //virtual void Unload() = 0;

  virtual int ResolveExport( const char* symbol, void** ptr, bool logging = true ) override;
  virtual int ResolveOrdinal( unsigned long ordinal, void** ptr ) override;
  virtual bool IsSystemDll() override;
  virtual HMODULE GetHModule() override;
  virtual bool HasSymbols() override;

protected:
  virtual bool ResolveExports() override;
  virtual bool LoadSymbols() override;
};
