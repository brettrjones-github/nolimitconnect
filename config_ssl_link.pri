
android:{
# include(config_version.pri) # not needed unless building kodi
include(config_os_detect.pri)
include(config_compiler.pri)
include(config_link.pri)
    message(ssl link $${SHARED_LIB_BIN_DIR}libssl_$${TARGET_ARCH_NAME}.so)
    LIBS +=  $${SHARED_LIB_BIN_DIR}libssl_$${TARGET_ARCH_NAME}.so
}

!android:{
    message(ssl link not android)
    CONFIG(debug, debug|release){
        LIBS +=  $${SHARED_LIB_BIN_DIR}libssl_d.so
    }

    CONFIG(release, debug|release){
        LIBS +=  $${SHARED_LIB_BIN_DIR}/libssl_d.so
    }
}
