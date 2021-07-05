cmake_minimum_required(VERSION 3.12.2)

project(libiconv)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libiconv STATIC
  ../../aliases.h
  ../../aliases_aix.h
  ../../aliases_aix_sysaix.h
  ../../aliases_dos.h
  ../../aliases_extra.h
  ../../aliases_osf1.h
  ../../aliases_osf1_sysosf1.h
  ../../aliases_sysaix.h
  ../../aliases_syshpux.h
  ../../aliases_sysosf1.h
  ../../aliases_syssolaris.h
  ../../aliases2.h
  ../../armscii_8.h
  ../../ascii.h
  ../../atarist.h
  ../../big5.h
  ../../big5_2003.h
  ../../big5hkscs1999.h
  ../../big5hkscs2001.h
  ../../big5hkscs2004.h
  ../../big5hkscs2008.h
  ../../c99.h
  ../../canonical.h
  ../../canonical_aix.h
  ../../canonical_aix_sysaix.h
  ../../canonical_dos.h
  ../../canonical_extra.h
  ../../canonical_local.h
  ../../canonical_local_sysaix.h
  ../../canonical_local_syshpux.h
  ../../canonical_local_sysosf1.h
  ../../canonical_local_syssolaris.h
  ../../canonical_osf1.h
  ../../canonical_osf1_sysosf1.h
  ../../canonical_sysaix.h
  ../../canonical_syshpux.h
  ../../canonical_sysosf1.h
  ../../canonical_syssolaris.h
  ../../ces_big5.h
  ../../ces_gbk.h
  ../../cjk_variants.h
  ../../cns11643.h
  ../../cns11643_1.h
  ../../cns11643_15.h
  ../../cns11643_2.h
  ../../cns11643_3.h
  ../../cns11643_4.h
  ../../cns11643_4a.h
  ../../cns11643_4b.h
  ../../cns11643_5.h
  ../../cns11643_6.h
  ../../cns11643_7.h
  ../../cns11643_inv.h
  ../../config_libiconv.h
  ../../converters.h
  ../../cp1046.h
  ../../cp1124.h
  ../../cp1125.h
  ../../cp1129.h
  ../../cp1131.h
  ../../cp1133.h
  ../../cp1161.h
  ../../cp1162.h
  ../../cp1163.h
  ../../cp1250.h
  ../../cp1251.h
  ../../cp1252.h
  ../../cp1253.h
  ../../cp1254.h
  ../../cp1255.h
  ../../cp1256.h
  ../../cp1257.h
  ../../cp1258.h
  ../../cp437.h
  ../../cp737.h
  ../../cp775.h
  ../../cp850.h
  ../../cp852.h
  ../../cp853.h
  ../../cp855.h
  ../../cp856.h
  ../../cp857.h
  ../../cp858.h
  ../../cp860.h
  ../../cp861.h
  ../../cp862.h
  ../../cp863.h
  ../../cp864.h
  ../../cp865.h
  ../../cp866.h
  ../../cp869.h
  ../../cp874.h
  ../../cp922.h
  ../../cp932.h
  ../../cp932ext.h
  ../../cp936.h
  ../../cp936ext.h
  ../../cp943.h
  ../../cp949.h
  ../../cp950.h
  ../../cp950ext.h
  ../../dec_hanyu.h
  ../../dec_kanji.h
  ../../euc_cn.h
  ../../euc_jisx0213.h
  ../../euc_jp.h
  ../../euc_kr.h
  ../../euc_tw.h
  ../../flags.h
  ../../flushwc.h
  ../../gb12345.h
  ../../gb12345ext.h
  ../../gb18030.h
  ../../gb18030ext.h
  ../../gb18030uni.h
  ../../gb2312.h
  ../../gbk.h
  ../../gbkext_inv.h
  ../../gbkext1.h
  ../../gbkext2.h
  ../../georgian_academy.h
  ../../georgian_ps.h
  ../../hkscs1999.h
  ../../hkscs2001.h
  ../../hkscs2004.h
  ../../hkscs2008.h
  ../../hp_roman8.h
  ../../hz.h
  ../../iconv.h
  ../../iconv_iconv.c
  ../../iconv_open1.h
  ../../iconv_open2.h
  ../../iso2022_cn.h
  ../../iso2022_cnext.h
  ../../iso2022_jp.h
  ../../iso2022_jp1.h
  ../../iso2022_jp2.h
  ../../iso2022_jp3.h
  ../../iso2022_kr.h
  ../../iso646_cn.h
  ../../iso646_jp.h
  ../../iso8859_1.h
  ../../iso8859_10.h
  ../../iso8859_11.h
  ../../iso8859_13.h
  ../../iso8859_14.h
  ../../iso8859_15.h
  ../../iso8859_16.h
  ../../iso8859_2.h
  ../../iso8859_3.h
  ../../iso8859_4.h
  ../../iso8859_5.h
  ../../iso8859_6.h
  ../../iso8859_7.h
  ../../iso8859_8.h
  ../../iso8859_9.h
  ../../isoir165.h
  ../../isoir165ext.h
  ../../java.h
  ../../jisx0201.h
  ../../jisx0208.h
  ../../jisx0212.h
  ../../jisx0213.h
  ../../johab.h
  ../../johab_hangul.h
  ../../koi8_r.h
  ../../koi8_ru.h
  ../../koi8_t.h
  ../../koi8_u.h
  ../../ksc5601.h
  ../../libiconv.rc
  ../../localcharset.h
  ../../localcharset_iconv.c
  ../../loop_unicode.h
  ../../loop_wchar.h
  ../../loops.h
  ../../mac_arabic.h
  ../../mac_centraleurope.h
  ../../mac_croatian.h
  ../../mac_cyrillic.h
  ../../mac_greek.h
  ../../mac_hebrew.h
  ../../mac_iceland.h
  ../../mac_roman.h
  ../../mac_romania.h
  ../../mac_thai.h
  ../../mac_turkish.h
  ../../mac_ukraine.h
  ../../mulelao.h
  ../../nextstep.h
  ../../pt154.h
  ../../relocatable.h
  ../../relocatable_iconv.c
  ../../riscos1.h
  ../../rk1048.h
  ../../shift_jisx0213.h
  ../../sjis.h
  ../../tcvn.h
  ../../tds565.h
  ../../tis620.h
  ../../translit.h
  ../../ucs2.h
  ../../ucs2be.h
  ../../ucs2internal.h
  ../../ucs2le.h
  ../../ucs2swapped.h
  ../../ucs4.h
  ../../ucs4be.h
  ../../ucs4internal.h
  ../../ucs4le.h
  ../../ucs4swapped.h
  ../../uhc_1.h
  ../../uhc_2.h
  ../../utf16.h
  ../../utf16be.h
  ../../utf16le.h
  ../../utf32.h
  ../../utf32be.h
  ../../utf32le.h
  ../../utf7.h
  ../../utf8.h
  ../../vietcomb.h
  ../../viscii.h
)
# Output file name
set_target_properties(libiconv
  PROPERTIES
  OUTPUT_NAME_DEBUG libiconvD
)

# Additional include directories
set_property(TARGET libiconv
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    ..\..\;
    ..\..\..\;
    J:\NlcJunk\DependLibs\libiconv\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libiconv\build\VS2017\..\..\..\..\sysheaders;
    "C:\Program Files (x86)\Visual Leak Detector\include">
)

# Preprocessor definitions
target_compile_definitions(libiconv PRIVATE
  $<$<CONFIG:Debug>:_MBCS;_WIN64;_DEBUG;_WINDOWS;_USRDLL;BUILDING_LIBICONV;BUILDING_LIBCHARSET;LIBICONV_EXPORTS>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libiconv PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libiconv PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libiconv PRIVATE
    ""
  )
endif ()
