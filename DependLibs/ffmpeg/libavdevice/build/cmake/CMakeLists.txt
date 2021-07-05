cmake_minimum_required(VERSION 3.12.2)

project(FfmpegAvDeviceLib)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(FfmpegAvDeviceLib STATIC
  ../../alldevices.c
  ../../alsa.c
  ../../alsa.h
  ../../alsa_dec.c
  ../../alsa_enc.c
  ../../avdevice.c
  ../../avdevice.h
  ../../avdevice_internal.h
  ../../bktr.c
  ../../caca.c
  ../../decklink_common.cpp
  ../../decklink_common.h
  ../../decklink_common_c.h
  ../../decklink_dec.cpp
  ../../decklink_dec.h
  ../../decklink_dec_c.c
  ../../decklink_enc.cpp
  ../../decklink_enc.h
  ../../decklink_enc_c.c
  ../../dshow.c
  ../../dshow_capture.h
  ../../dshow_common.c
  ../../dshow_crossbar.c
  ../../dshow_enummediatypes.c
  ../../dshow_enumpins.c
  ../../dshow_filter.c
  ../../dshow_pin.c
  ../../dv1394.c
  ../../dv1394.h
  ../../fbdev_common.c
  ../../fbdev_common.h
  ../../fbdev_dec.c
  ../../fbdev_enc.c
  ../../file_open.c
  ../../gdigrab.c
  ../../iec61883.c
  ../../jack.c
  ../../lavfi.c
  ../../libcdio.c
  ../../libdc1394.c
  ../../openal-dec.c
  ../../opengl_enc.c
  ../../opengl_enc_shaders.h
  ../../oss.c
  ../../oss.h
  ../../oss_dec.c
  ../../oss_enc.c
  ../../pulse_audio_common.c
  ../../pulse_audio_common.h
  ../../pulse_audio_dec.c
  ../../pulse_audio_enc.c
  ../../sndio.c
  ../../sndio.h
  ../../sndio_dec.c
  ../../sndio_enc.c
  ../../timefilter.c
  ../../timefilter.h
  ../../utils.c
  ../../v4l.c
  ../../v4l2.c
  ../../v4l2-common.c
  ../../v4l2-common.h
  ../../v4l2enc.c
  ../../version.h
  ../../vfwcap.c
  ../../x11grab.c
  ../../xcbgrab.c
  ../../xv.c
)
# Output file name
set_target_properties(FfmpegAvDeviceLib
  PROPERTIES
  OUTPUT_NAME_DEBUG FfmpegAvDeviceLibD
)

# Additional include directories
set_property(TARGET FfmpegAvDeviceLib
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\ffmpeg\libavdevice\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavdevice\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavdevice\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavdevice\build\VS2017\..\..\..\..\..\;
    J:\NlcJunk\DependLibs\ffmpeg\libavdevice\build\VS2017\..\..\..\..\..\sysheaders\;
    J:\NlcJunk\DependLibs\ffmpeg\libavdevice\build\VS2017\..\..\..\..\..\GoTvCore\xbmc\xbmc;
    J:\NlcJunk\DependLibs\ffmpeg\libavdevice\build\VS2017\..\..\..\..\openssl-1.0.2p\include\>
)

# Preprocessor definitions
target_compile_definitions(FfmpegAvDeviceLib PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(FfmpegAvDeviceLib PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(FfmpegAvDeviceLib PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(FfmpegAvDeviceLib PRIVATE
    ""
  )
endif ()
