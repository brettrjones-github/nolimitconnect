#pragma once

// these headers take a huge amount of time to compile so moved to separate header

/****************************************
 * Additional platform specific includes
 ****************************************/

#if defined(TARGET_OS_WINDOWS)
# ifndef DIRECTINPUT_VERSION
#  define DIRECTINPUT_VERSION 0x0800
# endif // DIRECTINPUT_VERSION
# include "mmsystem.h"
# include "DInput.h"
# include "DSound.h"
# define DSSPEAKER_USE_DEFAULT DSSPEAKER_STEREO
# define LPDIRECTSOUND8 LPDIRECTSOUND
# undef GetFreeSpace
# include <platform/win32/PlatformInclude.h>

# ifdef HAS_DX
#  include "d3d9.h"   // On Win32, we're always using DirectX for something, whether it be the actual rendering
#  include "d3d11_1.h"
#  include "dxgi.h"
#  include "d3dcompiler.h"
//#  ifdef __cplusplus // directx math requires c++ compile or don't include it
#   include "directxmath.h"
//#  endif //__cplusplus
#  include <d3d9types.h>
# endif // HAS_DX
#endif // TARGET_OS_WINDOWS

#ifdef HAS_SDL
# include "libsdl/SDL.h"
#endif
