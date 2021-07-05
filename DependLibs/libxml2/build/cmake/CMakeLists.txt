cmake_minimum_required(VERSION 3.12.2)

project(libxml2)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libxml2 STATIC
  ../../acconfig.h
  ../../c14n.h
  ../../c14n_xml2.c
  ../../catalog.h
  ../../catalog_xml2.c
  ../../chvalid.h
  ../../chvalid_xml2.c
  ../../config.h
  ../../debugXML.h
  ../../debugXML_xml2.c
  ../../dict.h
  ../../dict_xml2.c
  ../../DOCBparser.h
  ../../DOCBparser_xml2.c
  ../../elfgcchack.h
  ../../encoding.h
  ../../encoding_xml2.c
  ../../entities.h
  ../../entities_xml2.c
  ../../error_xml2.c
  ../../globals.h
  ../../globals_xml2.c
  ../../hash.h
  ../../hash_xml2.c
  ../../HTMLparser.h
  ../../HTMLparser_xml2.c
  ../../HTMLtree.h
  ../../HTMLtree_xml2.c
  ../../legacy_xml2.c
  ../../libxml.h
  ../../libxml2_config.h
  ../../libxml2_config_android.h
  ../../libxml2_config_linux.h
  ../../libxml2_config_windows.h
  ../../list.h
  ../../list_xml2.c
  ../../nanoftp.h
  ../../nanoftp_xml2.c
  ../../nanohttp.h
  ../../nanohttp_xml2.c
  ../../parser.h
  ../../parser_xml2.c
  ../../parserInternals.h
  ../../parserInternals_xml2.c
  ../../pattern.h
  ../../pattern_xml2.c
  ../../relaxng.h
  ../../relaxng_xml2.c
  ../../SAX.h
  ../../SAX_xml2.c
  ../../SAX2.h
  ../../SAX2_xml2.c
  ../../schemasInternals.h
  ../../schematron.h
  ../../schematron_xml2.c
  ../../threads.h
  ../../threads_xml2.c
  ../../tree.h
  ../../tree_xml2.c
  ../../trio.h
  ../../trio_xml2.c
  ../../triodef.h
  ../../trionan.h
  ../../triop.h
  ../../triostr.h
  ../../triostr_xml2.c
  ../../uri.h
  ../../uri_xml2.c
  ../../valid.h
  ../../valid_xml2.c
  ../../win32config.h
  ../../wsockcompat.h
  ../../xinclude.h
  ../../xinclude_xml2.c
  ../../xlink.h
  ../../xlink_xml2.c
  ../../xmlautomata.h
  ../../xmlerror.h
  ../../xmlexports.h
  ../../xmlIO.h
  ../../xmlIO_xml2.c
  ../../xmlmemory.h
  ../../xmlmemory_xml2.c
  ../../xmlmodule.h
  ../../xmlmodule_xml2.c
  ../../xmlreader.h
  ../../xmlreader_xml2.c
  ../../xmlregexp.h
  ../../xmlregexp_xml2.c
  ../../xmlsave.h
  ../../xmlsave_xml2.c
  ../../xmlschemas.h
  ../../xmlschemas_xml2.c
  ../../xmlschemastypes.h
  ../../xmlschemastypes_xml2.c
  ../../xmlstring.h
  ../../xmlstring_xml2.c
  ../../xmlunicode.h
  ../../xmlunicode_xml2.c
  ../../xmlversion.h
  ../../xmlwriter.h
  ../../xmlwriter_xml2.c
  ../../xpath.h
  ../../xpath_xml2.c
  ../../xpathInternals.h
  ../../xpointer_xml2.c
)
# Output file name
set_target_properties(libxml2
  PROPERTIES
  OUTPUT_NAME_DEBUG libxml2D
)

# Additional include directories
set_property(TARGET libxml2
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libxml2\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libxml2\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libxml2\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libxml2\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libxml2 PRIVATE
  $<$<CONFIG:Debug>:_MBCS;LIBXML_STATIC;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libxml2 PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libxml2 PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libxml2 PRIVATE
    ""
  )
endif ()
