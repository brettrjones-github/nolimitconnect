# Global
TEMPLATE = lib
QT       -= gui
CONFIG += staticlib
CONFIG -= sharedlib

TARGET_NAME = kodi

include(config_static_dependlib.pri)

android:{
    versionAtMost(QT_VERSION, 5.15.2){
        QT += androidextras
    }
}

#kodi fails because command line limit in windoz is 32,768.. so
#short obj path so does not overflow windows command line limit "make (e=87): The parameter is incorrect"
CONFIG(debug, debug|release){
    OBJECTS_DIR=.d
}

CONFIG(release, debug|release){
    OBJECTS_DIR=.r
}

include(libkodi.pri)
