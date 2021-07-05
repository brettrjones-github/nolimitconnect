cmake_minimum_required(VERSION 3.12.2)

project(libbento4)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libbento4 STATIC
  ../../Ap4.cpp
  ../../Ap4.h
  ../../Ap48bdlAtom.cpp
  ../../Ap48bdlAtom.h
  ../../Ap4AinfAtom.cpp
  ../../Ap4AinfAtom.h
  ../../Ap4Array.h
  ../../Ap4Atom.cpp
  ../../Ap4Atom.h
  ../../Ap4AtomFactory.cpp
  ../../Ap4AtomFactory.h
  ../../Ap4AtomSampleTable.cpp
  ../../Ap4AtomSampleTable.h
  ../../Ap4AvccAtom.cpp
  ../../Ap4AvccAtom.h
  ../../Ap4BlocAtom.cpp
  ../../Ap4BlocAtom.h
  ../../Ap4ByteStream.cpp
  ../../Ap4ByteStream.h
  ../../Ap4Co64Atom.cpp
  ../../Ap4Co64Atom.h
  ../../Ap4Command.cpp
  ../../Ap4Command.h
  ../../Ap4CommandFactory.cpp
  ../../Ap4CommandFactory.h
  ../../Ap4CommonEncryption.cpp
  ../../Ap4CommonEncryption.h
  ../../Ap4Config.h
  ../../Ap4Constants.h
  ../../Ap4ContainerAtom.cpp
  ../../Ap4ContainerAtom.h
  ../../Ap4CttsAtom.cpp
  ../../Ap4CttsAtom.h
  ../../Ap4DataBuffer.cpp
  ../../Ap4DataBuffer.h
  ../../Ap4Debug.cpp
  ../../Ap4Debug.h
  ../../Ap4Dec3Atom.cpp
  ../../Ap4Dec3Atom.h
  ../../Ap4DecoderConfigDescriptor.cpp
  ../../Ap4DecoderConfigDescriptor.h
  ../../Ap4DecoderSpecificInfoDescriptor.cpp
  ../../Ap4DecoderSpecificInfoDescriptor.h
  ../../Ap4Descriptor.cpp
  ../../Ap4Descriptor.h
  ../../Ap4DescriptorFactory.cpp
  ../../Ap4DescriptorFactory.h
  ../../Ap4DrefAtom.cpp
  ../../Ap4DrefAtom.h
  ../../Ap4DynamicCast.h
  ../../Ap4ElstAtom.cpp
  ../../Ap4ElstAtom.h
  ../../Ap4EsDescriptor.cpp
  ../../Ap4EsDescriptor.h
  ../../Ap4EsdsAtom.cpp
  ../../Ap4EsdsAtom.h
  ../../Ap4Expandable.cpp
  ../../Ap4Expandable.h
  ../../Ap4File.cpp
  ../../Ap4File.h
  ../../Ap4FileByteStream.h
  ../../Ap4FileCopier.cpp
  ../../Ap4FileCopier.h
  ../../Ap4FileWriter.cpp
  ../../Ap4FileWriter.h
  ../../Ap4FragmentSampleTable.cpp
  ../../Ap4FragmentSampleTable.h
  ../../Ap4FrmaAtom.cpp
  ../../Ap4FrmaAtom.h
  ../../Ap4FtypAtom.cpp
  ../../Ap4FtypAtom.h
  ../../Ap4GrpiAtom.cpp
  ../../Ap4GrpiAtom.h
  ../../Ap4HdlrAtom.cpp
  ../../Ap4HdlrAtom.h
  ../../Ap4HintTrackReader.cpp
  ../../Ap4HintTrackReader.h
  ../../Ap4HmhdAtom.cpp
  ../../Ap4HmhdAtom.h
  ../../Ap4HvccAtom.cpp
  ../../Ap4HvccAtom.h
  ../../Ap4IkmsAtom.cpp
  ../../Ap4IkmsAtom.h
  ../../Ap4Interfaces.h
  ../../Ap4IodsAtom.cpp
  ../../Ap4IodsAtom.h
  ../../Ap4Ipmp.cpp
  ../../Ap4Ipmp.h
  ../../Ap4IproAtom.cpp
  ../../Ap4IproAtom.h
  ../../Ap4IsfmAtom.cpp
  ../../Ap4IsfmAtom.h
  ../../Ap4IsltAtom.cpp
  ../../Ap4IsltAtom.h
  ../../Ap4IsmaCryp.cpp
  ../../Ap4IsmaCryp.h
  ../../Ap4LinearReader.cpp
  ../../Ap4LinearReader.h
  ../../Ap4List.h
  ../../Ap4Marlin.cpp
  ../../Ap4Marlin.h
  ../../Ap4MdhdAtom.cpp
  ../../Ap4MdhdAtom.h
  ../../Ap4MehdAtom.cpp
  ../../Ap4MehdAtom.h
  ../../Ap4MfhdAtom.cpp
  ../../Ap4MfhdAtom.h
  ../../Ap4MfroAtom.cpp
  ../../Ap4MfroAtom.h
  ../../Ap4MoovAtom.cpp
  ../../Ap4MoovAtom.h
  ../../Ap4Movie.cpp
  ../../Ap4Movie.h
  ../../Ap4MovieFragment.cpp
  ../../Ap4MovieFragment.h
  ../../Ap4Mpeg2Ts.cpp
  ../../Ap4Mpeg2Ts.h
  ../../Ap4MvhdAtom.cpp
  ../../Ap4MvhdAtom.h
  ../../Ap4NmhdAtom.cpp
  ../../Ap4NmhdAtom.h
  ../../Ap4ObjectDescriptor.cpp
  ../../Ap4ObjectDescriptor.h
  ../../Ap4OdafAtom.cpp
  ../../Ap4OdafAtom.h
  ../../Ap4OddaAtom.cpp
  ../../Ap4OddaAtom.h
  ../../Ap4OdheAtom.cpp
  ../../Ap4OdheAtom.h
  ../../Ap4OhdrAtom.cpp
  ../../Ap4OhdrAtom.h
  ../../Ap4OmaDcf.cpp
  ../../Ap4OmaDcf.h
  ../../Ap4PdinAtom.cpp
  ../../Ap4PdinAtom.h
  ../../Ap4Piff.cpp
  ../../Ap4Piff.h
  ../../Ap4Processor.cpp
  ../../Ap4Processor.h
  ../../Ap4Protection.cpp
  ../../Ap4Protection.h
  ../../Ap4PsshAtom.cpp
  ../../Ap4PsshAtom.h
  ../../Ap4Results.cpp
  ../../Ap4Results.h
  ../../Ap4RtpAtom.cpp
  ../../Ap4RtpAtom.h
  ../../Ap4RtpHint.cpp
  ../../Ap4RtpHint.h
  ../../Ap4SaioAtom.cpp
  ../../Ap4SaioAtom.h
  ../../Ap4SaizAtom.cpp
  ../../Ap4SaizAtom.h
  ../../Ap4Sample.cpp
  ../../Ap4Sample.h
  ../../Ap4SampleDescription.cpp
  ../../Ap4SampleDescription.h
  ../../Ap4SampleEntry.cpp
  ../../Ap4SampleEntry.h
  ../../Ap4SampleSource.cpp
  ../../Ap4SampleSource.h
  ../../Ap4SampleTable.cpp
  ../../Ap4SampleTable.h
  ../../Ap4SbgpAtom.cpp
  ../../Ap4SbgpAtom.h
  ../../Ap4SchmAtom.cpp
  ../../Ap4SchmAtom.h
  ../../Ap4SdpAtom.cpp
  ../../Ap4SdpAtom.h
  ../../Ap4SegmentBuilder.cpp
  ../../Ap4SegmentBuilder.h
  ../../Ap4SencAtom.cpp
  ../../Ap4SencAtom.h
  ../../Ap4SgpdAtom.cpp
  ../../Ap4SgpdAtom.h
  ../../Ap4SidxAtom.cpp
  ../../Ap4SidxAtom.h
  ../../Ap4SLConfigDescriptor.cpp
  ../../Ap4SLConfigDescriptor.h
  ../../Ap4SmhdAtom.cpp
  ../../Ap4SmhdAtom.h
  ../../Ap4StcoAtom.cpp
  ../../Ap4StcoAtom.h
  ../../Ap4SthdAtom.cpp
  ../../Ap4SthdAtom.h
  ../../Ap4String.cpp
  ../../Ap4String.h
  ../../Ap4StscAtom.cpp
  ../../Ap4StscAtom.h
  ../../Ap4StsdAtom.cpp
  ../../Ap4StsdAtom.h
  ../../Ap4StssAtom.cpp
  ../../Ap4StssAtom.h
  ../../Ap4StszAtom.cpp
  ../../Ap4StszAtom.h
  ../../Ap4SttsAtom.cpp
  ../../Ap4SttsAtom.h
  ../../Ap4Stz2Atom.cpp
  ../../Ap4Stz2Atom.h
  ../../Ap4SyntheticSampleTable.cpp
  ../../Ap4SyntheticSampleTable.h
  ../../Ap4TencAtom.cpp
  ../../Ap4TencAtom.h
  ../../Ap4TfdtAtom.cpp
  ../../Ap4TfdtAtom.h
  ../../Ap4TfhdAtom.cpp
  ../../Ap4TfhdAtom.h
  ../../Ap4TfraAtom.cpp
  ../../Ap4TfraAtom.h
  ../../Ap4TimsAtom.cpp
  ../../Ap4TimsAtom.h
  ../../Ap4TkhdAtom.cpp
  ../../Ap4TkhdAtom.h
  ../../Ap4Track.cpp
  ../../Ap4Track.h
  ../../AP4TrafAtom.cpp
  ../../Ap4TrafAtom.h
  ../../Ap4TrakAtom.cpp
  ../../Ap4TrakAtom.h
  ../../Ap4TrefTypeAtom.cpp
  ../../Ap4TrefTypeAtom.h
  ../../Ap4TrexAtom.cpp
  ../../Ap4TrexAtom.h
  ../../Ap4TrunAtom.cpp
  ../../Ap4TrunAtom.h
  ../../Ap4Types.h
  ../../Ap4UrlAtom.cpp
  ../../Ap4UrlAtom.h
  ../../Ap4Utils.cpp
  ../../Ap4Utils.h
  ../../Ap4UuidAtom.cpp
  ../../Ap4UuidAtom.h
  ../../Ap4Version.h
  ../../Ap4VmhdAtom.cpp
  ../../Ap4VmhdAtom.h
  ../../Codecs/Ap4AdtsParser.cpp
  ../../Codecs/Ap4AdtsParser.h
  ../../Codecs/Ap4AvcParser.cpp
  ../../Codecs/Ap4AvcParser.h
  ../../Codecs/Ap4BitStream.cpp
  ../../Codecs/Ap4BitStream.h
  ../../Codecs/Ap4HevcParser.cpp
  ../../Codecs/Ap4HevcParser.h
  ../../Codecs/Ap4Mp4AudioInfo.cpp
  ../../Codecs/Ap4Mp4AudioInfo.h
  ../../Codecs/Ap4NalParser.cpp
  ../../Codecs/Ap4NalParser.h
  ../../config_libbento4.h
  ../../Crypto/Ap4AesBlockCipher.cpp
  ../../Crypto/Ap4AesBlockCipher.h
  ../../Crypto/Ap4Hmac.cpp
  ../../Crypto/Ap4Hmac.h
  ../../Crypto/Ap4KeyWrap.cpp
  ../../Crypto/Ap4KeyWrap.h
  ../../Crypto/Ap4StreamCipher.cpp
  ../../Crypto/Ap4StreamCipher.h
  ../../MetaData/Ap4MetaData.cpp
  ../../MetaData/Ap4MetaData.h
  ../../System/GoTv/Ap4GoTvFileByteStream.cpp
  ../../System/Posix/Ap4PosixRandom.cpp
  ../../System/StdC/Ap4StdCFileByteStream.cpp
  ../../System/Win32/Ap4Win32Random.cpp
)
# Output file name
set_target_properties(libbento4
  PROPERTIES
  OUTPUT_NAME_DEBUG libbento4D
)

# Additional include directories
set_property(TARGET libbento4
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    J:\NlcJunk\DependLibs\libbento4\build\VS2017\..\..\;
    J:\NlcJunk\DependLibs\libbento4\build\VS2017\..\..\..\;
    J:\NlcJunk\DependLibs\libbento4\build\VS2017\..\..\..\..\;
    J:\NlcJunk\DependLibs\libbento4\build\VS2017\..\..\..\..\sysheaders;
    J:\NlcJunk\DependLibs\libbento4\build\VS2017\..\..\..\..\GoTvCore\xbmc\xbmc>
)

# Preprocessor definitions
target_compile_definitions(libbento4 PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;_DEBUG;WIN64;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libbento4 PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libbento4 PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libbento4 PRIVATE
    ""
  )
endif ()
