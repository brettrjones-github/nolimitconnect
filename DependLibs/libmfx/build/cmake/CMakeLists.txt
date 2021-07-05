cmake_minimum_required(VERSION 3.12.2)

project(libmfx)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libmfx STATIC
  ../../config_libmfx.h
  ../../mfx/mfxastructures.h
  ../../mfx/mfxaudio.h
  ../../mfx/mfxbrc.h
  ../../mfx/mfxcamera.h
  ../../mfx/mfxcommon.h
  ../../mfx/mfxdefs.h
  ../../mfx/mfxenc.h
  ../../mfx/mfxfei.h
  ../../mfx/mfxfeihevc.h
  ../../mfx/mfxjpeg.h
  ../../mfx/mfxla.h
  ../../mfx/mfxmvc.h
  ../../mfx/mfxpak.h
  ../../mfx/mfxplugin.h
  ../../mfx/mfxsc.h
  ../../mfx/mfxsession.h
  ../../mfx/mfxstructures.h
  ../../mfx/mfxvideo.h
  ../../mfx/mfxvp8.h
  ../../mfx/mfxvp9.h
  ../../mfx/mfxvstructures.h
  ../../src/mfx_critical_section.cpp
  ../../src/mfx_critical_section.h
  ../../src/mfx_critical_section_linux.cpp
  ../../src/mfx_dispatcher.cpp
  ../../src/mfx_dispatcher.h
  ../../src/mfx_dispatcher_defs.h
  ../../src/mfx_dispatcher_log.cpp
  ../../src/mfx_dispatcher_log.h
  ../../src/mfx_dxva2_device.cpp
  ../../src/mfx_dxva2_device.h
  ../../src/mfx_exposed_functions_list.h
  ../../src/mfx_function_table.cpp
  ../../src/mfx_library_iterator.cpp
  ../../src/mfx_library_iterator.h
  ../../src/mfx_library_iterator_linux.cpp
  ../../src/mfx_load_dll.cpp
  ../../src/mfx_load_dll.h
  ../../src/mfx_load_dll_linux.cpp
  ../../src/mfx_load_plugin.cpp
  ../../src/mfx_load_plugin.h
  ../../src/mfx_main.cpp
  ../../src/mfx_plugin_cfg_parser.cpp
  ../../src/mfx_plugin_cfg_parser.h
  ../../src/mfx_plugin_hive.cpp
  ../../src/mfx_plugin_hive.h
  ../../src/mfx_plugin_hive_linux.cpp
  ../../src/mfx_va_glue.c
  ../../src/mfx_va_glue.h
  ../../src/mfx_vector.h
  ../../src/mfx_win_reg_key.cpp
  ../../src/mfx_win_reg_key.h
  ../../src/mfxaudio_exposed_functions_list.h
)
# Output file name
set_target_properties(libmfx
  PROPERTIES
  OUTPUT_NAME_DEBUG libmfxD
)

# Additional include directories
set_property(TARGET libmfx
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libmfx\build\VS2017\../..;
    J:\NlcJunk\DependLibs\libmfx\build\VS2017\../../src;
    J:\NlcJunk\DependLibs\libmfx\build\VS2017\../../mfx;
    J:\NlcJunk\DependLibs\libmfx\build\VS2017\../../..;
    J:\NlcJunk\DependLibs\libmfx\build\VS2017\../../../..;
    J:\NlcJunk\DependLibs\libmfx\build\VS2017\../../../../sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libmfx PRIVATE
  $<$<CONFIG:Debug>:_MBCS;WIN64;_DEBUG;_LIB;MFX_VA>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libmfx PRIVATE
    "$<$<CONFIG:Debug>:/Gm>"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libmfx PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libmfx PRIVATE
    ""
  )
endif ()
