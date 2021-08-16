#this is just a minimal app to test if your build enviroment is working

TARGET = nolimitmintestapp
TEMPLATE = app

QT += gui core concurrent widgets network multimedia opengl xml svg multimediawidgets
android:{
    versionAtMost(QT_VERSION, 5.15.2){
        QT += androidextras
    }
}

DEFINES += QT_SVG_LIB QT_OPENGL_LIB QT_WIDGETS_LIB QT_GUI_LIB QT_CORE_LIB QT_MULTIMEDIA_LIB

#CONFIG += qt thread silent
# C++11 support
CONFIG += c++11

win32{
    DEFINES +=TARGET_OS_WINDOWS
}

android{
    DEFINES +=TARGET_OS_ANDROID
    DEFINES +=__ANDROID_API__=21
    DEFINES += BIONIC_IOCTL_NO_SIGNEDNESS_OVERLOAD
}

unix:!android{
    DEFINES +=TARGET_OS_LINUX
}

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

CONFIG(debug, debug|release){
# message(Link in DEBUG mode.)
    android{
        #if do not turn off optimization then android NDK std::string values will get optimized out and not viewable
        # sigh if optimization is turned off then the debugger gets hung up so may have to live with not being able to view strings.. also ndk strings sometimes are still optimized out
        QMAKE_CXXFLAGS -= -O1
        QMAKE_CXXFLAGS -= -O2
        QMAKE_CXXFLAGS -= -O3
        QMAKE_CXXFLAGS -= -O4
        QMAKE_CXXFLAGS += -O0
    #message(cxx flasgs $${QMAKE_CXXFLAGS})
    }
}

INCLUDEPATH += $$PWD/../../../../

HEADERS       = $$PWD/../../AppSource/mainwindow.h \
                $$PWD/../../AppSource/svgview.h \
                $$PWD/../../AppSource/exportdialog.h

RESOURCES     = $$PWD/../../nolimitmintestapp.qrc

SOURCES       = $$PWD/../../AppSource/main.cpp \
                $$PWD/../../AppSource/mainwindow.cpp \
                $$PWD/../../AppSource/svgview.cpp \
                $$PWD/../../AppSource/exportdialog.cpp
QT           += widgets svg

qtHaveModule(opengl): QT += opengl

CONFIG += console

android:{
    CONFIG += mobility
    MOBILITY =

    #shared libs
#    SHARED_LIB_PREFIX = $$PWD/bin-Android/lib/armeabi-v7a/
#    ANDROID_EXTRA_LIBS = \
#            $${SHARED_LIB_PREFIX}libpythoncore$${SHARED_PYTHON_LIB_SUFFIX}
#            $${SHARED_LIB_PREFIX}libssl$${SHARED_PYTHON_LIB_SUFFIX}
    ANDROID_LIBS = $$PWD/../../../../bin-Android/libs/armeabi-v7a
#    ANDROID_EXTRA_LIBS += \
#        $${ANDROID_LIBS}/libpythoncore$${SHARED_PYTHON_LIB_SUFFIX} \

    LIBS +=  -ldl -lm -lEGL -lGLESv2  -lc -lstdc++ -llog -ljnigraphics -landroid
}

# install
target.path = $$PWD/../../../bin/NoLimitMinTestApp/
INSTALLS += target
