# Global
TEMPLATE = lib
TARGET_NAME = pythoncore

include(config_python_lib.pri)

include(python_pythoncore.pri)

LIBS +=  $${STATIC_LIB_PREFIX}libeay$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}ssleay$${STATIC_LIB_SUFFIX}

LIBS +=  $${STATIC_LIB_PREFIX}gnu$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}iconv$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}bz2$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}depends$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}curl$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}netlib$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}pktlib$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}corelib$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}


#LIBS +=  $${SHARED_LIB_PREFIX}intl$${SHARED_LIB_SUFFIX}
#LIBS +=  $${SHARED_LIB_PREFIX}expat$${SHARED_LIB_SUFFIX}
#LIBS +=  $${SHARED_LIB_PREFIX}ffi$${SHARED_LIB_SUFFIX}
#LIBS +=  $${SHARED_LIB_PREFIX}expat$${SHARED_LIB_SUFFIX}
#LIBS +=  $${SHARED_LIB_PREFIX}util$${SHARED_LIB_SUFFIX}
#LIBS +=  $${SHARED_LIB_PREFIX}ssl$${SHARED_LIB_SUFFIX}
#LIBS +=  $${SHARED_LIB_PREFIX}crypto$${SHARED_LIB_SUFFIX}


#copy to local directory so can easily be linked to
CONFIG(debug, debug|release){
    copydata.commands = $(COPY_DIR) $$shell_path($$OUT_PWD/*.so) $$shell_path($$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/debug)
}

CONFIG(release, debug|release){
    copydata.commands = $(COPY_DIR) $$shell_path($$OUT_PWD/*.so) $$shell_path($$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/release)
 }

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

CONFIG(debug, debug|release){
    OBJECTS_DIR=.objs/$${TARGET_NAME}/debug
}

CONFIG(release, debug|release){
    OBJECTS_DIR=.objs/$${TARGET_NAME}/release
}



