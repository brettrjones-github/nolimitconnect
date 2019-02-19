LOCAL_PATH:= $(RENDERSCRIPT_TOOLCHAIN_PREBUILT_ROOT)/platform

# Prebuilt libRSSupport.so
include $(CLEAR_VARS)
LOCAL_MODULE:= RSSupport
LOCAL_SRC_FILES:= $(TARGET_ARCH)/lib$(LOCAL_MODULE)$(TARGET_SONAME_EXTENSION)
include $(PREBUILT_SHARED_LIBRARY)

# Prebuilt libRSSupportIO.so
include $(CLEAR_VARS)
LOCAL_MODULE:= RSSupportIO
LOCAL_SRC_FILES:= $(TARGET_ARCH)/lib$(LOCAL_MODULE)$(TARGET_SONAME_EXTENSION)
include $(PREBUILT_SHARED_LIBRARY)

# Prebuilt libblasV8.so
include $(CLEAR_VARS)
LOCAL_MODULE:= blasV8
LOCAL_SRC_FILES:= $(TARGET_ARCH)/lib$(LOCAL_MODULE)$(TARGET_SONAME_EXTENSION)
include $(PREBUILT_SHARED_LIBRARY)

# Prebuilt libRScpp_static.a
include $(CLEAR_VARS)
LOCAL_MODULE:= RScpp_static
LOCAL_SRC_FILES:= $(TARGET_ARCH)/lib$(LOCAL_MODULE)$(TARGET_LIB_EXTENSION)
include $(PREBUILT_STATIC_LIBRARY)