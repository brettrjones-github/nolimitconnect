/*
 *      Copyright (C) 2005-2015 Team XBMC
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
 *  along with Kodi; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "network/EventServer.h"
#include "network/Network.h"
#include "threads/SystemClock.h"
#include "Application.h"
#include "AppInboundProtocol.h"
#include "dialogs/GUIDialogBusy.h"
#include "events/EventLog.h"
#include "events/NotificationEvent.h"
#include "interfaces/builtins/Builtins.h"
#include "utils/JobManager.h"
#include "utils/Variant.h"
#include "LangInfoKodi.h"
#include "utils/Screenshot.h"
#include "NlcCoreUtil.h"
#include "NlcUrl.h"
#include "guilib/GUIComponent.h"
#include "guilib/TextureManager.h"
#include "cores/IPlayer.h"
#include "cores/VideoPlayer/DVDFileInfo.h"
#include "cores/AudioEngine/Engines/ActiveAE/ActiveAE.h"
#include "cores/AudioEngine/Utils/AEUtil.h"
#include "cores/playercorefactory/PlayerCoreFactory.h"
#include "PlayListPlayer.h"
#include "Autorun.h"
#include "video/Bookmark.h"
#include "video/VideoLibraryQueue.h"
#include "music/MusicLibraryQueue.h"
#include "guilib/GUIControlProfiler.h"
#include "utils/LangCodeExpander.h"
#include "GUIInfoManager.h"
#include "playlists/PlayListFactory.h"
#include "guilib/GUIFontManager.h"
#include "guilib/GUIColorManager.h"
#include "guilib/StereoscopicsManager.h"
#include "addons/BinaryAddonCache.h"
#include "addons/LanguageResource.h"
#include "addons/Skin.h"
#include "addons/VFSEntry.h"
#include "interfaces/generic/ScriptInvocationManager.h"
#ifdef HAS_PYTHON
#include "interfaces/python/XBPython.h"
#endif
#include "input/actions/ActionTranslator.h"
#include "input/ButtonTranslator.h"
#include "guilib/GUIAudioManager.h"
#include "GUIPassword.h"
#include "input/InertialScrollingHandler.h"
#include "messaging/ThreadMessage.h"
#include "messaging/ApplicationMessenger.h"
#include "messaging/helpers/DialogHelper.h"
#include "messaging/helpers/DialogOKHelper.h"
#include "SectionLoader.h"
#include "cores/DllLoader/DllLoaderContainer.h"
#include "GUIUserMessages.h"
#include "filesystem/Directory.h"
#include "filesystem/DirectoryCache.h"
#include "filesystem/StackDirectory.h"
#include "filesystem/SpecialProtocol.h"
#include "filesystem/DllLibCurl.h"
#include "filesystem/PluginDirectory.h"
#include "utils/SystemInfo.h"
#include "utils/TimeUtils.h"
#include "GUILargeTextureManager.h"
#include "TextureCache.h"
#include "playlists/SmartPlayList.h"
#include "playlists/PlayList.h"
#include "profiles/ProfileManager.h"
#include "windowing/WinSystem.h"
#include "powermanagement/PowerManager.h"
#include "powermanagement/DPMSSupport.h"
#include "settings/Settings.h"
#include "settings/AdvancedSettings.h"
#include "settings/DisplaySettings.h"
#include "settings/MediaSettings.h"
#include "settings/SkinSettings.h"
#include "guilib/LocalizeStrings.h"
#include "utils/CPUInfo.h"
#include "utils/FileExtensionProvider.h"
#include "utils/log.h"
#include "SeekHandler.h"
#include "ServiceBroker.h"

#include "input/KeyboardLayoutManager.h"

#ifdef HAS_UPNP
#include "network/upnp/UPnP.h"
#include "filesystem/UPnPDirectory.h"
#endif
#if defined(TARGET_POSIX) && defined(HAS_FILESYSTEM_SMB)
#include "platform/posix/filesystem/SMBDirectory.h"
#endif
#ifdef HAS_FILESYSTEM_NFS
#include "filesystem/NFSFile.h"
#endif
#ifdef HAS_FILESYSTEM_SFTP
#include "filesystem/SFTPFile.h"
#endif
#include "PartyModeManager.h"
#include "network/ZeroconfBrowser.h"
#ifndef TARGET_POSIX
#include "threads/platform/win/Win32Exception.h"
#endif
#ifdef HAS_DBUS
#include <dbus/dbus.h>
#endif
#include "interfaces/json-rpc/JSONRPC.h"
#include "interfaces/AnnouncementManager.h"
#include "peripherals/Peripherals.h"
#include "peripherals/devices/PeripheralImon.h"
#include "music/infoscanner/MusicInfoScanner.h"
#include "music/MusicUtils.h"
#include "music/MusicThumbLoader.h"

 // Windows includes
#include "guilib/GUIWindowManager.h"
#include "video/dialogs/GUIDialogVideoInfo.h"
#include "windows/GUIWindowScreensaver.h"
#include "video/PlayerController.h"

// Dialog includes
#include "video/dialogs/GUIDialogVideoBookmarks.h"
#include "dialogs/GUIDialogKaiToast.h"
#include "dialogs/GUIDialogSubMenu.h"
#include "dialogs/GUIDialogButtonMenu.h"
#include "dialogs/GUIDialogSimpleMenu.h"
#include "dialogs/GUIDialogVolumeBar.h"
#include "addons/settings/GUIDialogAddonSettings.h"

// PVR related include Files
#include "pvr/PVRGUIActions.h"
#include "pvr/PVRManager.h"

#include "video/dialogs/GUIDialogFullScreenInfo.h"
#include "guilib/GUIControlFactory.h"
#include "dialogs/GUIDialogCache.h"
#include "dialogs/GUIDialogPlayEject.h"
#include "utils/URIUtils.h"
#include "utils/XMLUtils.h"
#include "addons/AddonInstaller.h"
#include "addons/AddonManager.h"
#include "addons/RepositoryUpdater.h"
#include "music/tags/MusicInfoTag.h"
#include "music/tags/MusicInfoTagLoaderFactory.h"
#include "CompileInfo.h"

#ifdef TARGET_WINDOWS
#include "win32util.h"
#endif

#ifdef TARGET_DARWIN_OSX
#include "platform/darwin/osx/CocoaInterface.h"
#include "platform/darwin/osx/XBMCHelper.h"
#endif
#ifdef TARGET_DARWIN
#include "platform/darwin/DarwinUtils.h"
#endif

//BRJ #ifdef HAS_DVD_DRIVE
//#include <cdio/logging.h>
//#endif

#include "storage/MediaManager.h"
#include "utils/SaveFileStateJob.h"
#include "utils/AlarmClock.h"
#include "utils/StringUtils.h"
#include "DatabaseManager.h"
#include "input/InputManager.h"

#ifdef TARGET_POSIX
#include "XHandle.h"
#include "XTimeUtils.h"
#include "platform/posix/filesystem/PosixDirectory.h"
#endif

#if defined(TARGET_ANDROID) && !defined(HAVE_QT_GUI)
# include <android/jni/Build.h>
# include "platform/android/activity/XBMCApp.h"
# include "platform/android/activity/AndroidFeatures.h"
#elif defined(TARGET_ANDROID) && defined(HAVE_QT_GUI)
# include <qtandroid/jni/Build.h>
# include "platform/qt/KodiQtApp.h"
# include "platform/qt/qtandroid/activity/AndroidFeatures.h"
#endif

#ifdef TARGET_WINDOWS
#include "platform/Environment.h"
#endif

#if defined(HAS_LIBAMCODEC)
#include "utils/AMLUtils.h"
#endif

//TODO: XInitThreads
#ifdef HAVE_X11
#include "X11/Xlib.h"
#endif

#include "cores/FFmpeg.h"
#include "utils/CharsetConverter.h"
#include "pictures/GUIWindowSlideShow.h"

#include "GuiInterface/INlc.h"

#include "windowing/qt/WinSystemQt.h"


using namespace ADDON;
using namespace XFILE;
#ifdef HAS_DVD_DRIVE
using namespace MEDIA_DETECT;
#endif
using namespace PLAYLIST;
using namespace VIDEO;
using namespace MUSIC_INFO;
using namespace EVENTSERVER;
using namespace JSONRPC;
using namespace ANNOUNCEMENT;
using namespace PVR;
using namespace PERIPHERALS;
using namespace KODI;
using namespace KODI::MESSAGING;
using namespace ActiveAE;

using namespace XbmcThreads;

using KODI::MESSAGING::HELPERS::DialogResponse;

#define MAX_FFWD_SPEED 5

//============================================================================
void CApplication::fromGuiKeyPressEvent( int moduleNum, int key, int mod )
{
    if( CServiceBroker::GetWinSystem() )
    {
        XBMC_Event msg{ XBMC_KEYDOWN };
        msg.key.keysym.scancode = 0;
        msg.key.keysym.sym = ( XBMCKey )key;
        msg.key.keysym.unicode = 0;
        msg.key.keysym.mod = ( XBMCMod )mod;

        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
    }
}

//============================================================================
void CApplication::fromGuiKeyReleaseEvent( int moduleNum, int key, int mod )
{
    if( CServiceBroker::GetWinSystem() )
    {
        XBMC_Event msg{ XBMC_KEYUP };
        msg.key.keysym.scancode = 0;
        msg.key.keysym.sym = ( XBMCKey )key;
        msg.key.keysym.unicode = 0;
        msg.key.keysym.mod = ( XBMCMod )mod;
        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
    }
}

//============================================================================
void CApplication::fromGuiMousePressEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton )
{
    if( CServiceBroker::GetWinSystem() )
    {
        XBMC_Event msg{ XBMC_MOUSEBUTTONDOWN };
        msg.button = { ( unsigned char )mouseButton, ( uint16_t )mouseXPos, ( uint16_t )mouseYPos };
        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
    }
}

//============================================================================
void CApplication::fromGuiMouseReleaseEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton )
{
    if( CServiceBroker::GetWinSystem() )
    {
        XBMC_Event msg{ XBMC_MOUSEBUTTONUP };
        msg.button = { (unsigned char)mouseButton, (uint16_t)mouseXPos, (uint16_t)mouseYPos };
        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
    }
}

//============================================================================
void CApplication::fromGuiMouseMoveEvent( int moduleNum, int mouseXPos, int mouseYPos )
{
    XBMC_Event msg{ XBMC_MOUSEMOTION };
    msg.motion.x = (uint16_t)mouseXPos;
    msg.motion.y = (uint16_t)mouseYPos;
    CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
    if( winSys )
    {
        winSys->MessagePush( &msg );
    }
}

//============================================================================
void CApplication::fromGuiResizeBegin( int moduleNum, int winWidth, int winHeight )
{
	/*
	if( CServiceBroker::GetWinSystem() )
	{
		XBMC_Event msg{ XBMC_VIDEORESIZE };
		msg.resize = { winWidth, winHeight };
        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
	}*/
}

//============================================================================
void CApplication::fromGuiResizeEvent( int moduleNum, int winWidth, int winHeight )
{
	/*
    if( CServiceBroker::GetWinSystem() )
    {
        XBMC_Event msg{ XBMC_VIDEORESIZE };
        msg.resize = { winWidth, winHeight };
        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
    }*/
}

//============================================================================
void CApplication::fromGuiResizeEnd( int moduleNum, int winWidth, int winHeight )
{
	if( CServiceBroker::GetWinSystem() )
	{
		XBMC_Event msg{ XBMC_VIDEORESIZE };
		msg.resize = { winWidth, winHeight };
        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
	}
}

//============================================================================
void CApplication::fromGuiCloseEvent( int moduleNum )
{
    if( CServiceBroker::GetWinSystem() )
    {
        XBMC_Event msg{ XBMC_QUIT };
        CWinSystemQt* winSys = dynamic_cast< CWinSystemQt* >( CServiceBroker::GetWinSystem() );
        if( winSys )
        {
            winSys->MessagePush( &msg );
        }
    }
}

//============================================================================
void CApplication::fromGuiVisibleEvent( int moduleNum, bool isVisible )
{

}
