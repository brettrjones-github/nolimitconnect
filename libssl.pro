# Global
TEMPLATE = lib

CONFIG -= staticlib
CONFIG += sharedlib

DEFINES -= LIB_STATIC_LIB
DEFINES += LIB_SHARED_LIB

include(config_version.pri)
include(config_os_detect.pri)
include(config_compiler.pri)
include(config_opensslp_include.pri)

include(config_link.pri)
# normally would just include config_link.pri but it adds a extra lib prefix so can be loaded with -l but since the project is named libssl it comes out liblibssl so set target here
android:{
 TARGET = ssl # dont use _d for android
}

!android{
  CONFIG(debug, debug|release){
   TARGET = ssl_d
  }

  CONFIG(release, debug|release){
   TARGET = ssl
  }
}

PRE_TARGETDEPS += $$PWD/libcorelib.pro

DESTDIR = $${DEST_EXE_DIR}

include(libssl.pri)

unix:!android:{
    #give linux the path of where to load shared libraries from
    LIBS += -L$${DEST_SHARED_SSL_LIBS_DIR}
}

LIBS +=  $${STATIC_LIB_PREFIX}gnu$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}bz2$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}depends$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}corelib$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}



