cmake_minimum_required(VERSION 3.12.2)

project(libass)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libass STATIC
  ../../ass.c
  ../../ass.h
  ../../ass_bitmap.c
  ../../ass_bitmap.h
  ../../ass_blur.c
  ../../ass_cache.c
  ../../ass_cache.h
  ../../ass_cache_template.h
  ../../ass_compat.h
  ../../ass_coretext.c
  ../../ass_coretext.h
  ../../ass_directwrite.c
  ../../ass_directwrite.h
  ../../ass_drawing.c
  ../../ass_drawing.h
  ../../ass_font.c
  ../../ass_font.h
  ../../ass_fontconfig.c
  ../../ass_fontconfig.h
  ../../ass_fontselect.c
  ../../ass_fontselect.h
  ../../ass_func_template.h
  ../../ass_library.c
  ../../ass_library.h
  ../../ass_parse.c
  ../../ass_parse.h
  ../../ass_rasterizer.c
  ../../ass_rasterizer.h
  ../../ass_rasterizer_c.c
  ../../ass_render.c
  ../../ass_render.h
  ../../ass_render_api.c
  ../../ass_shaper.c
  ../../ass_shaper.h
  ../../ass_string.c
  ../../ass_string.h
  ../../ass_strtod.c
  ../../ass_types.h
  ../../ass_utils.c
  ../../ass_utils.h
  ../../config_libass.h
  ../../dwrite_c.h
)
# Output file name
set_target_properties(libass
  PROPERTIES
  OUTPUT_NAME_DEBUG libassD
)

# Additional include directories
set_property(TARGET libass
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libass\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libass\build\VS2017\..\..\..\libfreetype\include;
    J:\NlcJunk\DependLibs\libass\build\VS2017\..\..\..\libfribid\lib;
    J:\NlcJunk\DependLibs\libass\build\VS2017\..\..\..\libfribid\charset;
    J:\NlcJunk\DependLibs\libass\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libass\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libass\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libass PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libass PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libass PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libass PRIVATE
    ""
  )
endif ()
