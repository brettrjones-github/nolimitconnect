cmake_minimum_required(VERSION 3.12.2)

project(libjpg)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libjpg STATIC
  ../../jcapimin.c
  ../../jcapistd.c
  ../../jccoefct.c
  ../../jccolor.c
  ../../jcdctmgr.c
  ../../jchuff.c
  ../../jchuff.h
  ../../jcinit.c
  ../../jcmainct.c
  ../../jcmarker.c
  ../../jcmaster.c
  ../../jcomapi.c
  ../../jconfig.h
  ../../jcparam.c
  ../../jcphuff.c
  ../../jcprepct.c
  ../../jcsample.c
  ../../jctrans.c
  ../../jdapimin.c
  ../../jdapistd.c
  ../../jdatadst.c
  ../../jdatasrc.c
  ../../jdcoefct.c
  ../../jdcolor.c
  ../../jdct.h
  ../../jddctmgr.c
  ../../jdhuff.c
  ../../jdhuff.h
  ../../jdinput.c
  ../../jdmainct.c
  ../../jdmarker.c
  ../../jdmaster.c
  ../../jdmerge.c
  ../../jdphuff.c
  ../../jdpostct.c
  ../../jdsample.c
  ../../jdtrans.c
  ../../jerror.c
  ../../jerror.h
  ../../jfdctflt.c
  ../../jfdctfst.c
  ../../jfdctint.c
  ../../jidctflt.c
  ../../jidctfst.c
  ../../jidctint.c
  ../../jidctred.c
  ../../jinclude.h
  ../../jmemansi.c
  ../../jmemmgr.c
  ../../jmemsys.h
  ../../jmorecfg.h
  ../../jpegint.h
  ../../jpeglib.h
  ../../jquant1.c
  ../../jquant2.c
  ../../jutils.c
  ../../jversion.h
  ../../transupp.c
  ../../transupp.h
  ../../VxJpg.cpp
  ../../VxJpgLib.h
)
# Output file name
set_target_properties(libjpg
  PROPERTIES
  OUTPUT_NAME_DEBUG libjpgD
)

# Preprocessor definitions
target_compile_definitions(libjpg PRIVATE
  $<$<CONFIG:Debug>:_MBCS;HAVE_CONFIG_H;WIN32;_DEBUG;_LIB;TARGET_OS_WINDOWS;_CRT_SECURE_NO_WARNINGS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libjpg PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libjpg PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libjpg PRIVATE
    ""
  )
endif ()
