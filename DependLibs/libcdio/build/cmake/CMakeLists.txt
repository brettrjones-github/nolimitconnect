cmake_minimum_required(VERSION 3.12.2)

project(libcdio)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libcdio STATIC
  ../../config_libcdio.h
  ../../include/cdio/audio.h
  ../../include/cdio/bytesex.h
  ../../include/cdio/bytesex_asm.h
  ../../include/cdio/cd_types.h
  ../../include/cdio/cdio.h
  ../../include/cdio/cdio_version.h
  ../../include/cdio/cdtext.h
  ../../include/cdio/device.h
  ../../include/cdio/disc.h
  ../../include/cdio/ds.h
  ../../include/cdio/dvd.h
  ../../include/cdio/ecma_167.h
  ../../include/cdio/iso9660.h
  ../../include/cdio/logging.h
  ../../include/cdio/memory.h
  ../../include/cdio/mmc.h
  ../../include/cdio/mmc_cmds.h
  ../../include/cdio/mmc_hl_cmds.h
  ../../include/cdio/mmc_ll_cmds.h
  ../../include/cdio/mmc_util.h
  ../../include/cdio/posix.h
  ../../include/cdio/read.h
  ../../include/cdio/rock.h
  ../../include/cdio/scsi_mmc.h
  ../../include/cdio/sector.h
  ../../include/cdio/track.h
  ../../include/cdio/types.h
  ../../include/cdio/udf.h
  ../../include/cdio/udf_file.h
  ../../include/cdio/udf_time.h
  ../../include/cdio/utf8.h
  ../../include/cdio/util.h
  ../../include/cdio/xa.h
  ../../lib/cdio++/cdio_cdio.cpp
  ../../lib/cdio++/devices_cdio.cpp
  ../../lib/cdio++/iso9660_cdio.cpp
  ../../lib/driver/_cdio_generic_cdio.c
  ../../lib/driver/_cdio_stdio.h
  ../../lib/driver/_cdio_stdio_cdio.c
  ../../lib/driver/_cdio_stream.h
  ../../lib/driver/_cdio_stream_cdio.c
  ../../lib/driver/abs_path_cdio.c
  ../../lib/driver/aix_cdio.c
  ../../lib/driver/audio_cdio.c
  ../../lib/driver/bsdi_cdio.c
  ../../lib/driver/cd_types_cdio.c
  ../../lib/driver/cdio_assert.h
  ../../lib/driver/cdio_driver.c
  ../../lib/driver/cdio_private.h
  ../../lib/driver/cdtext_cdio.c
  ../../lib/driver/cdtext_private.h
  ../../lib/driver/device_cdio.c
  ../../lib/driver/disc_cdio.c
  ../../lib/driver/ds_cdio.c
  ../../lib/driver/filemode.h
  ../../lib/driver/FreeBSD/freebsd.h
  ../../lib/driver/FreeBSD/freebsd_cam_cdio.c
  ../../lib/driver/FreeBSD/freebsd_cdio.c
  ../../lib/driver/FreeBSD/freebsd_ioctl_cdio.c
  ../../lib/driver/generic.h
  ../../lib/driver/gnu_linux_cdio.c
  ../../lib/driver/image.h
  ../../lib/driver/image/bincue_cdio.c
  ../../lib/driver/image/cdrdao_cdio.c
  ../../lib/driver/image/nrg.h
  ../../lib/driver/image/nrg_cdio.c
  ../../lib/driver/image_common.h
  ../../lib/driver/image_common_cdio.c
  ../../lib/driver/logging_cdio.c
  ../../lib/driver/memory_cdio.c
  ../../lib/driver/mmc/mmc_cdio.c
  ../../lib/driver/mmc/mmc_cmd_helper.h
  ../../lib/driver/mmc/mmc_hl_cmds_cdio.c
  ../../lib/driver/mmc/mmc_ll_cmds_cdio.c
  ../../lib/driver/mmc/mmc_private.h
  ../../lib/driver/mmc/mmc_util_cdio.c
  ../../lib/driver/MSWindows/aspi32.h
  ../../lib/driver/MSWindows/aspi32_cdio.c
  ../../lib/driver/MSWindows/win32.h
  ../../lib/driver/MSWindows/win32_cdio.c
  ../../lib/driver/MSWindows/win32_ioctl_cdio.c
  ../../lib/driver/netbsd_cdio.c
  ../../lib/driver/os2_cdio.c
  ../../lib/driver/osx_cdio.c
  ../../lib/driver/portable.h
  ../../lib/driver/read_cdio.c
  ../../lib/driver/realpath_cdio.c
  ../../lib/driver/sector_cdio.c
  ../../lib/driver/solaris_cdio.c
  ../../lib/driver/track_cdio.c
  ../../lib/driver/utf8_cdio.c
  ../../lib/driver/util_cdio.c
  ../../lib/iso9660/iso9660_cdio2.c
  ../../lib/iso9660/iso9660_fs_cdio2.c
  ../../lib/iso9660/iso9660_private.h
  ../../lib/iso9660/rock_cdio.c
  ../../lib/iso9660/xa_cdio.c
  ../../lib/udf/filemode_cdio.c
  ../../lib/udf/udf_cdio.c
  ../../lib/udf/udf_file_cdio.c
  ../../lib/udf/udf_fs.h
  ../../lib/udf/udf_fs_cdio.c
  ../../lib/udf/udf_private.h
  ../../lib/udf/udf_time_cdio.c
)
# Output file name
set_target_properties(libcdio
  PROPERTIES
  OUTPUT_NAME_DEBUG libcdioD
)

# Additional include directories
set_property(TARGET libcdio
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libcdio\build\VS2017\..\..\lib\iso9660;
    J:\NlcJunk\DependLibs\libcdio\build\VS2017\..\..\lib\driver;
    J:\NlcJunk\DependLibs\libcdio\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libcdio\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libcdio\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libcdio\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libcdio PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libcdio PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libcdio PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libcdio PRIVATE
    ""
  )
endif ()
