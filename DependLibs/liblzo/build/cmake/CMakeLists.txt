cmake_minimum_required(VERSION 3.12.2)

project(liblzo)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(liblzo STATIC
  ../../compr1b.h
  ../../compr1c.h
  ../../config_liblzo.h
  ../../config1.h
  ../../config1a.h
  ../../config1b.h
  ../../config1c.h
  ../../config1f.h
  ../../config1x.h
  ../../config1y.h
  ../../config1z.h
  ../../config2a.h
  ../../lzo_asm.h
  ../../lzo_conf.h
  ../../lzo_crc.c
  ../../lzo_dict.h
  ../../lzo_func.h
  ../../lzo_init.c
  ../../lzo_ptr.c
  ../../lzo_ptr.h
  ../../lzo_str.c
  ../../lzo_supp.h
  ../../lzo_util.c
  ../../lzo1.c
  ../../lzo1.h
  ../../lzo1_99.c
  ../../lzo1a.c
  ../../lzo1a.h
  ../../lzo1a_99.c
  ../../lzo1a_de.h
  ../../lzo1b.h
  ../../lzo1b_1.c
  ../../lzo1b_2.c
  ../../lzo1b_3.c
  ../../lzo1b_4.c
  ../../lzo1b_5.c
  ../../lzo1b_6.c
  ../../lzo1b_7.c
  ../../lzo1b_8.c
  ../../lzo1b_9.c
  ../../lzo1b_99.c
  ../../lzo1b_9x.c
  ../../lzo1b_cc.c
  ../../lzo1b_cc.h
  ../../lzo1b_d1.c
  ../../lzo1b_d2.c
  ../../lzo1b_de.h
  ../../lzo1b_rr.c
  ../../lzo1b_xx.c
  ../../lzo1c.h
  ../../lzo1c_1.c
  ../../lzo1c_2.c
  ../../lzo1c_3.c
  ../../lzo1c_4.c
  ../../lzo1c_5.c
  ../../lzo1c_6.c
  ../../lzo1c_7.c
  ../../lzo1c_8.c
  ../../lzo1c_9.c
  ../../lzo1c_99.c
  ../../lzo1c_9x.c
  ../../lzo1c_cc.c
  ../../lzo1c_cc.h
  ../../lzo1c_d1.c
  ../../lzo1c_d2.c
  ../../lzo1c_rr.c
  ../../lzo1c_xx.c
  ../../lzo1f.h
  ../../lzo1f_1.c
  ../../lzo1f_9x.c
  ../../lzo1f_d1.c
  ../../lzo1f_d2.c
  ../../lzo1x.h
  ../../lzo1x_1.c
  ../../lzo1x_1k.c
  ../../lzo1x_1l.c
  ../../lzo1x_1o.c
  ../../lzo1x_9x.c
  ../../lzo1x_d1.c
  ../../lzo1x_d2.c
  ../../lzo1x_d3.c
  ../../lzo1x_o.c
  ../../lzo1y.h
  ../../lzo1y_1.c
  ../../lzo1y_9x.c
  ../../lzo1y_d1.c
  ../../lzo1y_d2.c
  ../../lzo1y_d3.c
  ../../lzo1y_o.c
  ../../lzo1z.h
  ../../lzo1z_9x.c
  ../../lzo1z_d1.c
  ../../lzo1z_d2.c
  ../../lzo1z_d3.c
  ../../lzo2a.h
  ../../lzo2a_9x.c
  ../../lzo2a_d1.c
  ../../lzo2a_d2.c
  ../../lzoconf.h
  ../../lzodefs.h
  ../../lzoutil.h
  ../../stats1a.h
  ../../stats1b.h
  ../../stats1c.h
)
# Output file name
set_target_properties(liblzo
  PROPERTIES
  OUTPUT_NAME_DEBUG liblzoD
)

# Additional include directories
set_property(TARGET liblzo
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\liblzo\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\liblzo\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\liblzo\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\liblzo\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(liblzo PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(liblzo PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(liblzo PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(liblzo PRIVATE
    ""
  )
endif ()
