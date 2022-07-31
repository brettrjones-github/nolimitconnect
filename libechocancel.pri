
    INCLUDEPATH += $$PWD/DependLibs/openssl-1.0.2p
    INCLUDEPATH += $$PWD/DependLibs/openssl-1.0.2p/include

win32{
    INCLUDEPATH += $$PWD/sysheaders
}

unix{
    INCLUDEPATH += $$PWD/DependLibs/openssl-1.0.2p
    INCLUDEPATH += $$PWD/DependLibs/openssl-1.0.2p/include
}

INCLUDEPATH += $$PWD/kodi_src/xbmc/xbmc/
INCLUDEPATH += $$PWD/DependLibs/



HEADERS += 	 \
    $$PWD/DependLibs/libechocancel/EchoCancel.h \
    $$PWD/DependLibs/libechocancel/EchoUtil.h \
    $$PWD/DependLibs/libechocancel/WebRtcTypeDefs.h \
    $$PWD/DependLibs/libechocancel/aec_common.h \
    $$PWD/DependLibs/libechocancel/aec_core.h \
    $$PWD/DependLibs/libechocancel/aec_core_internal.h \
    $$PWD/DependLibs/libechocancel/aec_logging.h \
    $$PWD/DependLibs/libechocancel/aec_logging_file_handling.h \
    $$PWD/DependLibs/libechocancel/aec_rdft.h \
    $$PWD/DependLibs/libechocancel/aec_resampler.h \
    $$PWD/DependLibs/libechocancel/array_view.h \
    $$PWD/DependLibs/libechocancel/arraysize.h \
    $$PWD/DependLibs/libechocancel/atomic32.h \
    $$PWD/DependLibs/libechocancel/atomicops.h \
    $$PWD/DependLibs/libechocancel/audio_device.h \
    $$PWD/DependLibs/libechocancel/audio_device_buffer.h \
    $$PWD/DependLibs/libechocancel/audio_device_config.h \
    $$PWD/DependLibs/libechocancel/audio_device_defines.h \
    $$PWD/DependLibs/libechocancel/audio_device_generic.h \
    $$PWD/DependLibs/libechocancel/audio_processing.h \
    $$PWD/DependLibs/libechocancel/audio_util.h \
    $$PWD/DependLibs/libechocancel/base64.h \
    $$PWD/DependLibs/libechocancel/basictypes.h \
    $$PWD/DependLibs/libechocancel/block_mean_calculator.h \
    $$PWD/DependLibs/libechocancel/byteorder.h \
    $$PWD/DependLibs/libechocancel/checks.h \
    $$PWD/DependLibs/libechocancel/common_types.h \
    $$PWD/DependLibs/libechocancel/constructormagic.h \
    $$PWD/DependLibs/libechocancel/cpu_features_wrapper.h \
    $$PWD/DependLibs/libechocancel/critical_section_wrapper.h \
    $$PWD/DependLibs/libechocancel/criticalsection.h \
    $$PWD/DependLibs/libechocancel/delay_estimator.h \
    $$PWD/DependLibs/libechocancel/delay_estimator_internal.h \
    $$PWD/DependLibs/libechocancel/delay_estimator_wrapper.h \
    $$PWD/DependLibs/libechocancel/echo_cancellation.h \
    $$PWD/DependLibs/libechocancel/echo_cancellation_internal.h \
    $$PWD/DependLibs/libechocancel/echo_common.h \
    $$PWD/DependLibs/libechocancel/event.h \
    $$PWD/DependLibs/libechocancel/event_timer_posix.h \
    $$PWD/DependLibs/libechocancel/event_timer_win.h \
    $$PWD/DependLibs/libechocancel/event_wrapper.h \
    $$PWD/DependLibs/libechocancel/fake_audio_device.h \
    $$PWD/DependLibs/libechocancel/fileutils.h \
    $$PWD/DependLibs/libechocancel/fine_audio_buffer.h \
    $$PWD/DependLibs/libechocancel/format_macros.h \
    $$PWD/DependLibs/libechocancel/logging.h \
    $$PWD/DependLibs/libechocancel/module.h \
    $$PWD/DependLibs/libechocancel/module_common_types.h \
    $$PWD/DependLibs/libechocancel/pathutils.h \
    $$PWD/DependLibs/libechocancel/platform_file.h \
    $$PWD/DependLibs/libechocancel/platform_thread.h \
    $$PWD/DependLibs/libechocancel/platform_thread_types.h \
    $$PWD/DependLibs/libechocancel/random.h \
    $$PWD/DependLibs/libechocancel/real_fft.h \
    $$PWD/DependLibs/libechocancel/ring_buffer.h \
    $$PWD/DependLibs/libechocancel/safe_conversions.h \
    $$PWD/DependLibs/libechocancel/safe_conversions_impl.h \
    $$PWD/DependLibs/libechocancel/scoped_ptr.h \
    $$PWD/DependLibs/libechocancel/scoped_ref_ptr.h \
    $$PWD/DependLibs/libechocancel/scopedptrcollection.h \
    $$PWD/DependLibs/libechocancel/sec_buffer.h \
    $$PWD/DependLibs/libechocancel/signal_processing_library.h \
    $$PWD/DependLibs/libechocancel/spl_inl.h \
    $$PWD/DependLibs/libechocancel/spl_inl_armv7.h \
    $$PWD/DependLibs/libechocancel/spl_inl_mips.h \
    $$PWD/DependLibs/libechocancel/stringencode.h \
    $$PWD/DependLibs/libechocancel/stringutils.h \
    $$PWD/DependLibs/libechocancel/template_util.h \
    $$PWD/DependLibs/libechocancel/thread_annotations.h \
    $$PWD/DependLibs/libechocancel/thread_checker.h \
    $$PWD/DependLibs/libechocancel/thread_checker_impl.h \
    $$PWD/DependLibs/libechocancel/tick_util.h \
    $$PWD/DependLibs/libechocancel/timeutils.h \
    $$PWD/DependLibs/libechocancel/urlencode.h \
    $$PWD/DependLibs/libechocancel/wav_file.h \
    $$PWD/DependLibs/libechocancel/wav_header.h \
    $$PWD/DependLibs/libechocancel/win32.h \
    $$PWD/DependLibs/libechocancel/win32filesystem.h


SOURCES += \
    $$PWD/DependLibs/libechocancel/EchoCancel.cpp \
    $$PWD/DependLibs/libechocancel/EchoUtil.cpp \
    $$PWD/DependLibs/libechocancel/aec_core.cpp \
    $$PWD/DependLibs/libechocancel/aec_core_mips.cpp \
    $$PWD/DependLibs/libechocancel/aec_core_neon.cpp \
    $$PWD/DependLibs/libechocancel/aec_core_sse2.cpp \
    $$PWD/DependLibs/libechocancel/aec_logging_file_handling.cpp \
    $$PWD/DependLibs/libechocancel/aec_rdft.cpp \
    $$PWD/DependLibs/libechocancel/aec_rdft_mips.cpp \
    $$PWD/DependLibs/libechocancel/aec_rdft_neon.cpp \
    $$PWD/DependLibs/libechocancel/aec_rdft_sse2.cpp \
    $$PWD/DependLibs/libechocancel/aec_resampler.cpp \
    $$PWD/DependLibs/libechocancel/atomic32_posix.cpp \
    $$PWD/DependLibs/libechocancel/atomic32_win.cpp \
    $$PWD/DependLibs/libechocancel/audio_device_buffer.cpp \
    $$PWD/DependLibs/libechocancel/audio_device_generic.cpp \
    $$PWD/DependLibs/libechocancel/audio_util.cpp \
    $$PWD/DependLibs/libechocancel/base64.cpp \
    $$PWD/DependLibs/libechocancel/block_mean_calculator.cpp \
    $$PWD/DependLibs/libechocancel/checks.cpp \
    $$PWD/DependLibs/libechocancel/common_types.cpp \
    $$PWD/DependLibs/libechocancel/cpu_features.cpp \
    $$PWD/DependLibs/libechocancel/cpu_features_android.cpp \
    $$PWD/DependLibs/libechocancel/cpu_features_linux.cpp \
    $$PWD/DependLibs/libechocancel/criticalsection.cpp \
    $$PWD/DependLibs/libechocancel/delay_estimator.cpp \
    $$PWD/DependLibs/libechocancel/delay_estimator_wrapper.cpp \
    $$PWD/DependLibs/libechocancel/echo_cancellation.cpp \
    $$PWD/DependLibs/libechocancel/event.cpp \
    $$PWD/DependLibs/libechocancel/event_timer_posix.cpp \
    $$PWD/DependLibs/libechocancel/event_timer_win.cpp \
    $$PWD/DependLibs/libechocancel/event_wrapper.cpp \
    $$PWD/DependLibs/libechocancel/fileutils.cpp \
    $$PWD/DependLibs/libechocancel/fine_audio_buffer.cpp \
    $$PWD/DependLibs/libechocancel/logging.cpp \
    $$PWD/DependLibs/libechocancel/pathutils.cpp \
    $$PWD/DependLibs/libechocancel/platform_file.cpp \
    $$PWD/DependLibs/libechocancel/platform_thread.cpp \
    $$PWD/DependLibs/libechocancel/random.cpp \
    $$PWD/DependLibs/libechocancel/randomization_functions.cpp \
    $$PWD/DependLibs/libechocancel/ring_buffer.cpp \
    $$PWD/DependLibs/libechocancel/stringencode.cpp \
    $$PWD/DependLibs/libechocancel/stringutils.cpp \
    $$PWD/DependLibs/libechocancel/thread_checker_impl.cpp \
    $$PWD/DependLibs/libechocancel/tick_util.cpp \
    $$PWD/DependLibs/libechocancel/timeutils.cpp \
    $$PWD/DependLibs/libechocancel/urlencode.cpp \
    $$PWD/DependLibs/libechocancel/wav_file.cpp \
    $$PWD/DependLibs/libechocancel/wav_header.cpp \
    $$PWD/DependLibs/libechocancel/win32.cpp \
    $$PWD/DependLibs/libechocancel/win32filesystem.cpp

