
include(os_detect.pri)
include(version.pri)

DEFINES += QT_NO_CAST_TO_ASCII
# Fast concatenation (Qt >= 4.6)
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS
win32: DEFINES += NOMINMAX

CONFIG(debug, debug|release){
 message(Project is built in DEBUG mode.)
}

CONFIG(release, debug|release){
 message(Project is built in RELEASE mode.)
}

CONFIG(release, debug|release){
    DEFINES += _DEBUG
    linux:!android: message(Disabling UNIX debug output.)
    win32: message(Disabling WIN32 debug output.)
    android: message(Disabling ANDROID debug output.)
    DEFINES += QT_NO_DEBUG_OUTPUT
}

CONFIG(debug, debug|release){
    DEFINES += NDEBUG
    linux:!android: message(Enabling UNIX debug output.)
    win32: message(Enabling WIN32 debug output.)
    android: message(Enabling ANDROID debug output.)
}

TARGET=$${TARGET_NAME}$${TARGET_OS_NAME}$${TARGET_LIB_APPEND}

OBJECTS_DIR = $$PWD/objs/$${TARGET_NAME}$${TARGET_OS_NAME}/.obj
MOC_DIR = $$PWD/generated/$${TARGET_NAME}$${TARGET_OS_NAME}/.moc
RCC_DIR = $$PWD/generated/$${TARGET_NAME}$${TARGET_OS_NAME}/.qrc
UI_DIR = $$PWD/generated/$${TARGET_NAME}$${TARGET_OS_NAME}/.ui

android: {
INCLUDEPATH +=  $$PWD/../../../../../android/android-ndk-r10e/platforms/android-18/arch-arm/usr/include
}

INCLUDEPATH += $$PWD/../../kodi_src/xbmc
INCLUDEPATH += $$PWD/../../DependLibs/ffmpeg
INCLUDEPATH += $$PWD/../../DependLibs/openssl-1.0.1f/inc32
INCLUDEPATH += $$PWD/../../DependLibs/
INCLUDEPATH += $$PWD/../../kodi_src
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/../../

QMAKE_CFLAGS = -DANDROID_NDK -DBUILD_OGLES2 -DDISABLE_IMPORTGL
QMAKE_CFLAGS_YACC   = -Wno-unused -Wno-unused-parameter -Wno-parentheses
QMAKE_CXXFLAGS_RTTI_OFF = -fno-rtti
QMAKE_CXXFLAGS_EXCEPTIONS_OFF = -fno-exceptions
QMAKE_CXXFLAGS += -Wno-unused -Wno-unused-parameter -Wno-missing-field-initializers -Wno-parentheses -Wno-attributes  -Wno-ignored-qualifiers

#include(app/app.pri)
#include(base/base.pri)
#!nowebui: include(webui/webui.pri)
#!nogui: include(gui/gui.pri)

# Resource files
QMAKE_RESOURCE_FLAGS += -compress 9 -threshold 5
RESOURCES += \
    $$PWD/appresources/commonresources.qrc

# Translations
#TRANSLATIONS += $$files(lang/qbittorrent_*.ts)
