cmake_minimum_required(VERSION 3.12.2)

project(libpthread)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libpthread STATIC
  ../../../../GoTvCompilerConfig.h
  ../../../../GoTvConfig.h
  ../../../../GoTvCpuArchDefines.h
  ../../../../GoTvDependLibrariesConfig.h
  ../../../../GoTvFeaturesConfig.h
  ../../../../GoTvTargetOsConfig.h
  ../../config_pthread.h
  ../../implement.h
  ../../pthread.c
  ../../pthread.h
  ../../sched.h
  ../../semaphore.h
)
# Output file name
set_target_properties(libpthread
  PROPERTIES
  OUTPUT_NAME_DEBUG libpthreadD
)

# Additional include directories
set_property(TARGET libpthread
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libpthread\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libpthread\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libpthread\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libpthread\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libpthread PRIVATE
  $<$<CONFIG:Debug>:_MBCS;__CLEANUP_C;WIN32;_DEBUG;_WINDOWS;_LIB;PTW32_BUILD>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libpthread PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libpthread PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libpthread PRIVATE
    ""
  )
endif ()

# Link libraries
set_property(TARGET libpthread
  APPEND PROPERTY LINK_LIBRARIES
  "$<$<CONFIG:Debug>:ws2_32.lib>"
)
