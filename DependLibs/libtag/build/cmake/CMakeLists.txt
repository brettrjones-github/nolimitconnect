cmake_minimum_required(VERSION 3.12.2)

project(libtag)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libtag STATIC
  ../../ape/apefile.cpp
  ../../ape/apefile.h
  ../../ape/apefooter.cpp
  ../../ape/apefooter.h
  ../../ape/apeitem.cpp
  ../../ape/apeitem.h
  ../../ape/apeproperties.cpp
  ../../ape/apeproperties.h
  ../../ape/apetag.cpp
  ../../ape/apetag.h
  ../../asf/asfattribute.cpp
  ../../asf/asfattribute.h
  ../../asf/asffile.cpp
  ../../asf/asffile.h
  ../../asf/asfpicture.cpp
  ../../asf/asfpicture.h
  ../../asf/asfproperties.cpp
  ../../asf/asfproperties.h
  ../../asf/asftag.cpp
  ../../asf/asftag.h
  ../../asf/asfutils.h
  ../../audioproperties.cpp
  ../../audioproperties.h
  ../../config_libtag.h
  ../../fileref.cpp
  ../../fileref.h
  ../../flac/flacfile.cpp
  ../../flac/flacfile.h
  ../../flac/flacmetadatablock.cpp
  ../../flac/flacmetadatablock.h
  ../../flac/flacpicture.cpp
  ../../flac/flacpicture.h
  ../../flac/flacproperties.cpp
  ../../flac/flacproperties.h
  ../../flac/flacunknownmetadatablock.cpp
  ../../flac/flacunknownmetadatablock.h
  ../../it/itfile.cpp
  ../../it/itfile.h
  ../../it/itproperties.cpp
  ../../it/itproperties.h
  ../../mod/modfile.cpp
  ../../mod/modfile.h
  ../../mod/modfilebase.cpp
  ../../mod/modfilebase.h
  ../../mod/modfileprivate.h
  ../../mod/modproperties.cpp
  ../../mod/modproperties.h
  ../../mod/modtag.cpp
  ../../mod/modtag.h
  ../../mp4/mp4atom.cpp
  ../../mp4/mp4atom.h
  ../../mp4/mp4coverart.cpp
  ../../mp4/mp4coverart.h
  ../../mp4/mp4file.cpp
  ../../mp4/mp4file.h
  ../../mp4/mp4item.cpp
  ../../mp4/mp4item.h
  ../../mp4/mp4properties.cpp
  ../../mp4/mp4properties.h
  ../../mp4/mp4tag.cpp
  ../../mp4/mp4tag.h
  ../../mpc/mpcfile.cpp
  ../../mpc/mpcfile.h
  ../../mpc/mpcproperties.cpp
  ../../mpc/mpcproperties.h
  ../../mpeg/id3v1/id3v1genres.cpp
  ../../mpeg/id3v1/id3v1genres.h
  ../../mpeg/id3v1/id3v1tag.cpp
  ../../mpeg/id3v1/id3v1tag.h
  ../../mpeg/id3v2/frames/attachedpictureframe.cpp
  ../../mpeg/id3v2/frames/attachedpictureframe.h
  ../../mpeg/id3v2/frames/chapterframe.cpp
  ../../mpeg/id3v2/frames/chapterframe.h
  ../../mpeg/id3v2/frames/commentsframe.cpp
  ../../mpeg/id3v2/frames/commentsframe.h
  ../../mpeg/id3v2/frames/eventtimingcodesframe.cpp
  ../../mpeg/id3v2/frames/eventtimingcodesframe.h
  ../../mpeg/id3v2/frames/generalencapsulatedobjectframe.cpp
  ../../mpeg/id3v2/frames/generalencapsulatedobjectframe.h
  ../../mpeg/id3v2/frames/ownershipframe.cpp
  ../../mpeg/id3v2/frames/ownershipframe.h
  ../../mpeg/id3v2/frames/podcastframe.cpp
  ../../mpeg/id3v2/frames/podcastframe.h
  ../../mpeg/id3v2/frames/popularimeterframe.cpp
  ../../mpeg/id3v2/frames/popularimeterframe.h
  ../../mpeg/id3v2/frames/privateframe.cpp
  ../../mpeg/id3v2/frames/privateframe.h
  ../../mpeg/id3v2/frames/relativevolumeframe.cpp
  ../../mpeg/id3v2/frames/relativevolumeframe.h
  ../../mpeg/id3v2/frames/synchronizedlyricsframe.cpp
  ../../mpeg/id3v2/frames/synchronizedlyricsframe.h
  ../../mpeg/id3v2/frames/tableofcontentsframe.cpp
  ../../mpeg/id3v2/frames/tableofcontentsframe.h
  ../../mpeg/id3v2/frames/textidentificationframe.cpp
  ../../mpeg/id3v2/frames/textidentificationframe.h
  ../../mpeg/id3v2/frames/uniquefileidentifierframe.cpp
  ../../mpeg/id3v2/frames/uniquefileidentifierframe.h
  ../../mpeg/id3v2/frames/unknownframe.cpp
  ../../mpeg/id3v2/frames/unknownframe.h
  ../../mpeg/id3v2/frames/unsynchronizedlyricsframe.cpp
  ../../mpeg/id3v2/frames/unsynchronizedlyricsframe.h
  ../../mpeg/id3v2/frames/urllinkframe.cpp
  ../../mpeg/id3v2/frames/urllinkframe.h
  ../../mpeg/id3v2/id3v2extendedheader.cpp
  ../../mpeg/id3v2/id3v2extendedheader.h
  ../../mpeg/id3v2/id3v2footer.cpp
  ../../mpeg/id3v2/id3v2footer.h
  ../../mpeg/id3v2/id3v2frame.cpp
  ../../mpeg/id3v2/id3v2frame.h
  ../../mpeg/id3v2/id3v2framefactory.cpp
  ../../mpeg/id3v2/id3v2framefactory.h
  ../../mpeg/id3v2/id3v2header.cpp
  ../../mpeg/id3v2/id3v2header.h
  ../../mpeg/id3v2/id3v2synchdata.cpp
  ../../mpeg/id3v2/id3v2synchdata.h
  ../../mpeg/id3v2/id3v2tag.cpp
  ../../mpeg/id3v2/id3v2tag.h
  ../../mpeg/mpegfile.cpp
  ../../mpeg/mpegfile.h
  ../../mpeg/mpegheader.cpp
  ../../mpeg/mpegheader.h
  ../../mpeg/mpegproperties.cpp
  ../../mpeg/mpegproperties.h
  ../../mpeg/mpegutils.h
  ../../mpeg/xingheader.cpp
  ../../mpeg/xingheader.h
  ../../ogg/flac/oggflacfile.cpp
  ../../ogg/flac/oggflacfile.h
  ../../ogg/oggfile.cpp
  ../../ogg/oggfile.h
  ../../ogg/oggpage.cpp
  ../../ogg/oggpage.h
  ../../ogg/oggpageheader.cpp
  ../../ogg/oggpageheader.h
  ../../ogg/opus/opusfile.cpp
  ../../ogg/opus/opusfile.h
  ../../ogg/opus/opusproperties.cpp
  ../../ogg/opus/opusproperties.h
  ../../ogg/speex/speexfile.cpp
  ../../ogg/speex/speexfile.h
  ../../ogg/speex/speexproperties.cpp
  ../../ogg/speex/speexproperties.h
  ../../ogg/vorbis/vorbisfile.cpp
  ../../ogg/vorbis/vorbisfile.h
  ../../ogg/vorbis/vorbisproperties.cpp
  ../../ogg/vorbis/vorbisproperties.h
  ../../ogg/xiphcomment.cpp
  ../../ogg/xiphcomment.h
  ../../riff/aiff/aifffile.cpp
  ../../riff/aiff/aifffile.h
  ../../riff/aiff/aiffproperties.cpp
  ../../riff/aiff/aiffproperties.h
  ../../riff/rifffile.cpp
  ../../riff/rifffile.h
  ../../riff/riffutils.h
  ../../riff/wav/infotag.cpp
  ../../riff/wav/infotag.h
  ../../riff/wav/wavfile.cpp
  ../../riff/wav/wavfile.h
  ../../riff/wav/wavproperties.cpp
  ../../riff/wav/wavproperties.h
  ../../s3m/s3mfile.cpp
  ../../s3m/s3mfile.h
  ../../s3m/s3mproperties.cpp
  ../../s3m/s3mproperties.h
  ../../tag.cpp
  ../../tag.h
  ../../taglib.h
  ../../taglib_export.h
  ../../tagunion.cpp
  ../../tagunion.h
  ../../tagutils.cpp
  ../../tagutils.h
  ../../toolkit/taglib.h
  ../../toolkit/tbytevector.cpp
  ../../toolkit/tbytevector.h
  ../../toolkit/tbytevectorlist.cpp
  ../../toolkit/tbytevectorlist.h
  ../../toolkit/tbytevectorstream.cpp
  ../../toolkit/tbytevectorstream.h
  ../../toolkit/tdebug.cpp
  ../../toolkit/tdebug.h
  ../../toolkit/tdebuglistener.cpp
  ../../toolkit/tdebuglistener.h
  ../../toolkit/tfile.cpp
  ../../toolkit/tfile.h
  ../../toolkit/tfilestream.cpp
  ../../toolkit/tfilestream.h
  ../../toolkit/tiostream.cpp
  ../../toolkit/tiostream.h
  ../../toolkit/tlist.h
  ../../toolkit/tmap.h
  ../../toolkit/tpropertymap.cpp
  ../../toolkit/tpropertymap.h
  ../../toolkit/trefcounter.cpp
  ../../toolkit/trefcounter.h
  ../../toolkit/tstring.cpp
  ../../toolkit/tstring.h
  ../../toolkit/tstringlist.cpp
  ../../toolkit/tstringlist.h
  ../../toolkit/tutils.h
  ../../toolkit/tzlib.cpp
  ../../toolkit/tzlib.h
  ../../toolkit/unicode.cpp
  ../../toolkit/unicode.h
  ../../trueaudio/trueaudiofile.cpp
  ../../trueaudio/trueaudiofile.h
  ../../trueaudio/trueaudioproperties.cpp
  ../../trueaudio/trueaudioproperties.h
  ../../wavpack/wavpackfile.cpp
  ../../wavpack/wavpackfile.h
  ../../wavpack/wavpackproperties.cpp
  ../../wavpack/wavpackproperties.h
  ../../xm/xmfile.cpp
  ../../xm/xmfile.h
  ../../xm/xmproperties.cpp
  ../../xm/xmproperties.h
)
# Output file name
set_target_properties(libtag
  PROPERTIES
  OUTPUT_NAME_DEBUG libtagD
)

# Additional include directories
set_property(TARGET libtag
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libtag\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libtag\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libtag\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libtag\build\VS2017\..\..\..\..\sysheaders>
)

# Preprocessor definitions
target_compile_definitions(libtag PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libtag PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libtag PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libtag PRIVATE
    ""
  )
endif ()
