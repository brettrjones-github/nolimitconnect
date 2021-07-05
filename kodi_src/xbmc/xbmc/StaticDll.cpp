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

#include "StaticDll.h"
#include "SectionLoader.h"
#include "filesystem/File.h"
#include "utils/log.h"

using namespace XFILE;

/*
StaticDll::StaticDll()
    : DllDynamic( strDllName )
,    LibraryLoader( )
{
    m_dll = this;
    m_DelayUnload = true;
}
*/

StaticDll::StaticDll( const std::string& strDllName ) 
: DllDynamic( strDllName )
, LibraryLoader( strDllName )
{
    m_dll = this;
    m_DelayUnload = true;
}

StaticDll::~StaticDll()
{
    Unload();
}

bool StaticDll::Load()
{
    if( m_dll )
        return true;

    if( !( m_dll = CSectionLoader::LoadDLL( m_strDllName, m_DelayUnload, LoadSymbols() ) ) )
        return false;

    if( !ResolveExports() )
    {
        CLog::Log( LOGERROR, "Unable to resolve exports from dll %s", m_strDllName.c_str() );
        Unload();
        return false;
    }

    return true;
}

void StaticDll::Unload()
{
    if( m_dll )
        CSectionLoader::UnloadDLL( m_strDllName );
    m_dll = NULL;
}

bool StaticDll::CanLoad()
{
    return CFile::Exists( m_strDllName );
}

bool StaticDll::EnableDelayedUnload( bool bOnOff )
{
    if( m_dll )
        return false;

    m_DelayUnload = bOnOff;

    return true;
}

bool StaticDll::SetFile( const std::string& strDllName )
{
    if( m_dll )
        return false;

    m_strDllName = strDllName;
    return true;
}

bool StaticDll::ResolveExports()
{
    return true;
}

bool StaticDll::LoadSymbols( )
{
    return true;
}

bool StaticDll::IsLoaded( ) const
{
    return true;
}

int StaticDll::ResolveExport( const char* symbol, void** ptr, bool logging )
{
    return 0;
}

int StaticDll::ResolveOrdinal( unsigned long ordinal, void** ptr )
{
    return 0;
}

bool StaticDll::IsSystemDll()
{
    return false;
}

HMODULE StaticDll::GetHModule()
{
    return 0;
}

bool StaticDll::HasSymbols()
{
    return false;
}