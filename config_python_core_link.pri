
android:{
include(config_version.pri)
include(config_os_detect.pri)
include(config_compiler.pri)
include(config_link.pri)
    message(Python Core $${DEST_SHARED_LIBS_DIR}libpythoncore$${SHARED_SSL_LIB_SUFFIX})
    LIBS +=  $${DEST_SHARED_LIBS_DIR}libpythoncore$${SHARED_SSL_LIB_SUFFIX}
}

!android:{
    CONFIG(debug, debug|release){
        LIBS +=  $${DEST_SHARED_LIBS_DIR}/libpythoncore_d.so
    }

    CONFIG(release, debug|release){
        LIBS +=  $${DEST_SHARED_LIBS_DIR}/libpythoncore.so
    }
}
