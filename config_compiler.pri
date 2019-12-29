
#even if not building lib these need defined so included headers are defined correctly
DEFINES += LIB_STATIC HAVE_CONFIG_H

DEFINES += FT_DEBUG_LEVEL_ERROR=1
DEFINES += T_DEBUG_LEVEL_TRACE=1
DEFINES += FT2_BUILD_LIBRARY=1
DEFINES += FT_MAKE_OPTION_SINGLE_OBJECT=1

DEFINES +=_FILE_OFFSET_BITS=64

CONFIG(debug, debug|release){
    DEFINES += _DEBUG
    DEFINES += DEBUG
}

CONFIG(release, debug|release){
    DEFINES += NDEBUG
    DEFINES += RELEASE
    DEFINES += QT_NO_DEBUG_OUTPUT
}

INCLUDEPATH += $$PWD/DependLibs

android{
    DEFINES +=__ANDROID_API__=21
    DEFINES += BIONIC_IOCTL_NO_SIGNEDNESS_OVERLOAD
    #CONFIG(debug, debug|release){
        #if do not turn off optimization then android NDK std::string values will get optimized out and not viewable
        # sigh if optimization is turned off then the debugger gets hung up so have to live with not being able to view strings
        #QMAKE_CXXFLAGS += -O0
        #QMAKE_CFLAGS += -O0
        #QMAKE_CFLAGS_DEBUG -= -g
        #QMAKE_CFLAGS_DEBUG -= -Og
        #QMAKE_CFLAGS_DEBUG += -O0
        #QMAKE_CXXFLAGS_DEBUG -= -g
        #QMAKE_CXXFLAGS_DEBUG -= -Og
        #QMAKE_CXXFLAGS_DEBUG += -O0
    #}
}

win32{
    INCLUDEPATH += $$PWD/sysheaders
}

#QMAKE_EXTRA_COMPILERS += yasm
#NASMEXTRAFLAGS = -f elf64 -g -F dwarf
#OTHER_FILES += $$NASM_SOURCES
#nasm.output = ${QMAKE_FILE_BASE}.o
#nasm.commands = yasm $$NASMEXTRAFLAGS -o ${QMAKE_FILE_BASE}.o ${QMAKE_FILE_NAME}
#nasm.input = NASM_SOURCES



unix:QMAKE_CXXFLAGS += -std=c++11
unix:QMAKE_CXXFLAGS += -fpermissive
unix:QMAKE_CXXFLAGS += -Wno-unused-variable
unix:QMAKE_CXXFLAGS += -Wno-unused-function
unix:QMAKE_CXXFLAGS += -Wno-attributes
unix:QMAKE_CXXFLAGS += -Wno-sign-compare
unix:QMAKE_CXXFLAGS += -Wno-parentheses
unix:QMAKE_CXXFLAGS += -Wno-missing-field-initializers
unix:QMAKE_CXXFLAGS += -Wno-switch
#unix:!android:QMAKE_CXXFLAGS += -Wno-#pragma-messages

unix:QMAKE_CFLAGS += -Wno-unused-variable
unix:QMAKE_CFLAGS += -Wno-unused-function
unix:QMAKE_CFLAGS += -Wno-attributes
unix:QMAKE_CFLAGS += -Wno-sign-compare
unix:QMAKE_CFLAGS += -Wno-parentheses
unix:QMAKE_CFLAGS += -Wno-pointer-sign
unix:QMAKE_CFLAGS += -Wno-missing-field-initializers
unix:QMAKE_CFLAGS += -Wno-switch

#QMAKE_CXXFLAGS_WARN_ON += -Wno-overload-virtual

#unix:!android:QMAKE_CFLAGS += -Wno-#pragma-messages

unix:{
QMAKE_CFLAGS_WARN_OFF -= -Wunused-parameter
QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
QMAKE_CXXFLAGS_WARN_OFF -= -Winconsistent-missing-override
#QMAKE_CXXFLAGS_WARN_ON += -Wno-inconsistent-missing-override
QMAKE_CFLAGS_WARN_OFF -= -Wmissing-braces
QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-braces
QMAKE_CFLAGS_WARN_OFF -= -Wmissing-field-initializers
QMAKE_CXXFLAGS_WARN_OFF -= -Wmissing-field-initializers
QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-field-initializers
}


