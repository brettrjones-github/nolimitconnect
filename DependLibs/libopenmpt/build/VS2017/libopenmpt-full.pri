HEADERS += 	$$PWD/../../common/BuildSettings.h \
	$$PWD/../../common/CompilerDetect.h \
	$$PWD/../../common/ComponentManager.h \
	$$PWD/../../common/Endianness.h \
	$$PWD/../../common/FileReader.h \
	$$PWD/../../common/FileReaderFwd.h \
	$$PWD/../../common/FlagSet.h \
	$$PWD/../../common/Logging.h \
	$$PWD/../../common/Profiler.h \
	$$PWD/../../common/StringFixer.h \
	$$PWD/../../common/WriteMemoryDump.h \
	$$PWD/../../common/misc_util.h \
	$$PWD/../../common/mptBufferIO.h \
	$$PWD/../../common/mptCPU.h \
	$$PWD/../../common/mptCRC.h \
	$$PWD/../../common/mptFileIO.h \
	$$PWD/../../common/mptIO.h \
	$$PWD/../../common/mptLibrary.h \
	$$PWD/../../common/mptMutex.h \
	$$PWD/../../common/mptOS.h \
	$$PWD/../../common/mptPathString.h \
	$$PWD/../../common/mptRandom.h \
	$$PWD/../../common/mptString.h \
	$$PWD/../../common/mptStringFormat.h \
	$$PWD/../../common/mptStringParse.h \
	$$PWD/../../common/mptThread.h \
	$$PWD/../../common/mptTime.h \
	$$PWD/../../common/mptTypeTraits.h \
	$$PWD/../../common/mptUUID.h \
	$$PWD/../../common/mptWine.h \
	$$PWD/../../common/serialization_utils.h \
	$$PWD/../../common/stdafx.h \
	$$PWD/../../common/typedefs.h \
	$$PWD/../../common/version.h \
	$$PWD/../../common/versionNumber.h \
	$$PWD/../../libopenmpt/libopenmpt.h \
	$$PWD/../../libopenmpt/libopenmpt.hpp \
	$$PWD/../../libopenmpt/libopenmpt_config.h \
	$$PWD/../../libopenmpt/libopenmpt_ext.h \
	$$PWD/../../libopenmpt/libopenmpt_ext.hpp \
	$$PWD/../../libopenmpt/libopenmpt_ext_impl.hpp \
	$$PWD/../../libopenmpt/libopenmpt_impl.hpp \
	$$PWD/../../libopenmpt/libopenmpt_internal.h \
	$$PWD/../../libopenmpt/libopenmpt_stream_callbacks_buffer.h \
	$$PWD/../../libopenmpt/libopenmpt_stream_callbacks_fd.h \
	$$PWD/../../libopenmpt/libopenmpt_stream_callbacks_file.h \
	$$PWD/../../libopenmpt/libopenmpt_version.h \
	$$PWD/../../soundbase/SampleFormat.h \
	$$PWD/../../soundbase/SampleFormatConverters.h \
	$$PWD/../../soundbase/SampleFormatCopy.h \
	$$PWD/../../sounddsp/AGC.h \
	$$PWD/../../sounddsp/DSP.h \
	$$PWD/../../sounddsp/EQ.h \
	$$PWD/../../sounddsp/Reverb.h \
	$$PWD/../../soundlib/AudioCriticalSection.h \
	$$PWD/../../soundlib/AudioReadTarget.h \
	$$PWD/../../soundlib/ChunkReader.h \
	$$PWD/../../soundlib/Container.h \
	$$PWD/../../soundlib/Dither.h \
	$$PWD/../../soundlib/Dlsbank.h \
	$$PWD/../../soundlib/FloatMixer.h \
	$$PWD/../../soundlib/ITCompression.h \
	$$PWD/../../soundlib/ITTools.h \
	$$PWD/../../soundlib/IntMixer.h \
	$$PWD/../../soundlib/Loaders.h \
	$$PWD/../../soundlib/MIDIEvents.h \
	$$PWD/../../soundlib/MIDIMacros.h \
	$$PWD/../../soundlib/MPEGFrame.h \
	$$PWD/../../soundlib/Message.h \
	$$PWD/../../soundlib/MixFuncTable.h \
	$$PWD/../../soundlib/Mixer.h \
	$$PWD/../../soundlib/MixerInterface.h \
	$$PWD/../../soundlib/MixerLoops.h \
	$$PWD/../../soundlib/MixerSettings.h \
	$$PWD/../../soundlib/ModChannel.h \
	$$PWD/../../soundlib/ModInstrument.h \
	$$PWD/../../soundlib/ModSample.h \
	$$PWD/../../soundlib/ModSampleCopy.h \
	$$PWD/../../soundlib/ModSequence.h \
	$$PWD/../../soundlib/OggStream.h \
	$$PWD/../../soundlib/Paula.h \
	$$PWD/../../soundlib/Resampler.h \
	$$PWD/../../soundlib/RowVisitor.h \
	$$PWD/../../soundlib/S3MTools.h \
	$$PWD/../../soundlib/SampleIO.h \
	$$PWD/../../soundlib/Snd_defs.h \
	$$PWD/../../soundlib/Sndfile.h \
	$$PWD/../../soundlib/SoundFilePlayConfig.h \
	$$PWD/../../soundlib/Tables.h \
	$$PWD/../../soundlib/Tagging.h \
	$$PWD/../../soundlib/UMXTools.h \
	$$PWD/../../soundlib/WAVTools.h \
	$$PWD/../../soundlib/WindowedFIR.h \
	$$PWD/../../soundlib/XMTools.h \
	$$PWD/../../soundlib/mod_specifications.h \
	$$PWD/../../soundlib/modcommand.h \
	$$PWD/../../soundlib/modsmp_ctrl.h \
	$$PWD/../../soundlib/pattern.h \
	$$PWD/../../soundlib/patternContainer.h \
	$$PWD/../../soundlib/plugins/DigiBoosterEcho.h \
	$$PWD/../../soundlib/plugins/LFOPlugin.h \
	$$PWD/../../soundlib/plugins/OpCodes.h \
	$$PWD/../../soundlib/plugins/PlugInterface.h \
	$$PWD/../../soundlib/plugins/PluginEventQueue.h \
	$$PWD/../../soundlib/plugins/PluginManager.h \
	$$PWD/../../soundlib/plugins/PluginMixBuffer.h \
	$$PWD/../../soundlib/plugins/PluginStructs.h \
	$$PWD/../../soundlib/plugins/dmo/Chorus.h \
	$$PWD/../../soundlib/plugins/dmo/Compressor.h \
	$$PWD/../../soundlib/plugins/dmo/DMOPlugin.h \
	$$PWD/../../soundlib/plugins/dmo/Distortion.h \
	$$PWD/../../soundlib/plugins/dmo/Echo.h \
	$$PWD/../../soundlib/plugins/dmo/Flanger.h \
	$$PWD/../../soundlib/plugins/dmo/Gargle.h \
	$$PWD/../../soundlib/plugins/dmo/I3DL2Reverb.h \
	$$PWD/../../soundlib/plugins/dmo/ParamEq.h \
	$$PWD/../../soundlib/plugins/dmo/WavesReverb.h \
	$$PWD/../../soundlib/tuning.h \
	$$PWD/../../soundlib/tuningbase.h \
	$$PWD/../../soundlib/tuningcollection.h

SOURCES += 	$$PWD/../../common/ComponentManager.cpp \
	$$PWD/../../common/FileReader.cpp \
	$$PWD/../../common/Logging.cpp \
	$$PWD/../../common/Profiler.cpp \
	$$PWD/../../common/misc_util.cpp \
	$$PWD/../../common/mptCPU.cpp \
	$$PWD/../../common/mptFileIO.cpp \
	$$PWD/../../common/mptIO.cpp \
	$$PWD/../../common/mptLibrary.cpp \
	$$PWD/../../common/mptOS.cpp \
	$$PWD/../../common/mptPathString.cpp \
	$$PWD/../../common/mptRandom.cpp \
	$$PWD/../../common/mptString.cpp \
	$$PWD/../../common/mptStringFormat.cpp \
	$$PWD/../../common/mptStringParse.cpp \
	$$PWD/../../common/mptTime.cpp \
	$$PWD/../../common/mptUUID.cpp \
	$$PWD/../../common/mptWine.cpp \
	$$PWD/../../common/serialization_utils.cpp \
	$$PWD/../../common/stdafx.cpp \
	$$PWD/../../common/typedefs.cpp \
	$$PWD/../../common/version.cpp \
	$$PWD/../../libopenmpt/libopenmpt_c.cpp \
	$$PWD/../../libopenmpt/libopenmpt_cxx.cpp \
	$$PWD/../../libopenmpt/libopenmpt_ext_impl.cpp \
	$$PWD/../../libopenmpt/libopenmpt_impl.cpp \
	$$PWD/../../sounddsp/AGC.cpp \
	$$PWD/../../sounddsp/DSP.cpp \
	$$PWD/../../sounddsp/EQ.cpp \
	$$PWD/../../sounddsp/Reverb.cpp \
	$$PWD/../../soundlib/AudioCriticalSection.cpp \
	$$PWD/../../soundlib/ContainerMMCMP.cpp \
	$$PWD/../../soundlib/ContainerPP20.cpp \
	$$PWD/../../soundlib/ContainerUMX.cpp \
	$$PWD/../../soundlib/ContainerXPK.cpp \
	$$PWD/../../soundlib/Dither.cpp \
	$$PWD/../../soundlib/Dlsbank.cpp \
	$$PWD/../../soundlib/Fastmix.cpp \
	$$PWD/../../soundlib/ITCompression.cpp \
	$$PWD/../../soundlib/ITTools.cpp \
	$$PWD/../../soundlib/InstrumentExtensions.cpp \
	$$PWD/../../soundlib/Load_669.cpp \
	$$PWD/../../soundlib/Load_amf.cpp \
	$$PWD/../../soundlib/Load_ams.cpp \
	$$PWD/../../soundlib/Load_dbm.cpp \
	$$PWD/../../soundlib/Load_digi.cpp \
	$$PWD/../../soundlib/Load_dmf.cpp \
	$$PWD/../../soundlib/Load_dsm.cpp \
	$$PWD/../../soundlib/Load_dtm.cpp \
	$$PWD/../../soundlib/Load_far.cpp \
	$$PWD/../../soundlib/Load_gdm.cpp \
	$$PWD/../../soundlib/Load_imf.cpp \
	$$PWD/../../soundlib/Load_it.cpp \
	$$PWD/../../soundlib/Load_itp.cpp \
	$$PWD/../../soundlib/Load_mdl.cpp \
	$$PWD/../../soundlib/Load_med.cpp \
	$$PWD/../../soundlib/Load_mid.cpp \
	$$PWD/../../soundlib/Load_mo3.cpp \
	$$PWD/../../soundlib/Load_mod.cpp \
	$$PWD/../../soundlib/Load_mt2.cpp \
	$$PWD/../../soundlib/Load_mtm.cpp \
	$$PWD/../../soundlib/Load_okt.cpp \
	$$PWD/../../soundlib/Load_plm.cpp \
	$$PWD/../../soundlib/Load_psm.cpp \
	$$PWD/../../soundlib/Load_ptm.cpp \
	$$PWD/../../soundlib/Load_s3m.cpp \
	$$PWD/../../soundlib/Load_sfx.cpp \
	$$PWD/../../soundlib/Load_stm.cpp \
	$$PWD/../../soundlib/Load_stp.cpp \
	$$PWD/../../soundlib/Load_uax.cpp \
	$$PWD/../../soundlib/Load_ult.cpp \
	$$PWD/../../soundlib/Load_wav.cpp \
	$$PWD/../../soundlib/Load_xm.cpp \
	$$PWD/../../soundlib/MIDIEvents.cpp \
	$$PWD/../../soundlib/MIDIMacros.cpp \
	$$PWD/../../soundlib/MPEGFrame.cpp \
	$$PWD/../../soundlib/Message.cpp \
	$$PWD/../../soundlib/MixFuncTable.cpp \
	$$PWD/../../soundlib/MixerLoops.cpp \
	$$PWD/../../soundlib/MixerSettings.cpp \
	$$PWD/../../soundlib/ModChannel.cpp \
	$$PWD/../../soundlib/ModInstrument.cpp \
	$$PWD/../../soundlib/ModSample.cpp \
	$$PWD/../../soundlib/ModSequence.cpp \
	$$PWD/../../soundlib/OggStream.cpp \
	$$PWD/../../soundlib/Paula.cpp \
	$$PWD/../../soundlib/RowVisitor.cpp \
	$$PWD/../../soundlib/S3MTools.cpp \
	$$PWD/../../soundlib/SampleFormatFLAC.cpp \
	$$PWD/../../soundlib/SampleFormatMP3.cpp \
	$$PWD/../../soundlib/SampleFormatMediaFoundation.cpp \
	$$PWD/../../soundlib/SampleFormatOpus.cpp \
	$$PWD/../../soundlib/SampleFormatVorbis.cpp \
	$$PWD/../../soundlib/SampleFormats.cpp \
	$$PWD/../../soundlib/SampleIO.cpp \
	$$PWD/../../soundlib/Snd_flt.cpp \
	$$PWD/../../soundlib/Snd_fx.cpp \
	$$PWD/../../soundlib/Sndfile.cpp \
	$$PWD/../../soundlib/Sndmix.cpp \
	$$PWD/../../soundlib/SoundFilePlayConfig.cpp \
	$$PWD/../../soundlib/Tables.cpp \
	$$PWD/../../soundlib/Tagging.cpp \
	$$PWD/../../soundlib/UMXTools.cpp \
	$$PWD/../../soundlib/UpgradeModule.cpp \
	$$PWD/../../soundlib/WAVTools.cpp \
	$$PWD/../../soundlib/WindowedFIR.cpp \
	$$PWD/../../soundlib/XMTools.cpp \
	$$PWD/../../soundlib/load_j2b.cpp \
	$$PWD/../../soundlib/mod_specifications.cpp \
	$$PWD/../../soundlib/modcommand.cpp \
	$$PWD/../../soundlib/modsmp_ctrl.cpp \
	$$PWD/../../soundlib/pattern.cpp \
	$$PWD/../../soundlib/patternContainer.cpp \
	$$PWD/../../soundlib/plugins/DigiBoosterEcho.cpp \
	$$PWD/../../soundlib/plugins/LFOPlugin.cpp \
	$$PWD/../../soundlib/plugins/PlugInterface.cpp \
	$$PWD/../../soundlib/plugins/PluginManager.cpp \
	$$PWD/../../soundlib/plugins/dmo/Chorus.cpp \
	$$PWD/../../soundlib/plugins/dmo/Compressor.cpp \
	$$PWD/../../soundlib/plugins/dmo/DMOPlugin.cpp \
	$$PWD/../../soundlib/plugins/dmo/Distortion.cpp \
	$$PWD/../../soundlib/plugins/dmo/Echo.cpp \
	$$PWD/../../soundlib/plugins/dmo/Flanger.cpp \
	$$PWD/../../soundlib/plugins/dmo/Gargle.cpp \
	$$PWD/../../soundlib/plugins/dmo/I3DL2Reverb.cpp \
	$$PWD/../../soundlib/plugins/dmo/ParamEq.cpp \
	$$PWD/../../soundlib/plugins/dmo/WavesReverb.cpp \
	$$PWD/../../soundlib/tuning.cpp \
	$$PWD/../../soundlib/tuningCollection.cpp \
	$$PWD/../../soundlib/tuningbase.cpp
