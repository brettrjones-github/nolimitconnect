TEMPLATE = app
TARGET = customvideoitem

QT += widgets
versionAtMost(QT_VERSION, 5.15.2){
    QT += multimedia
    QT += multimediawidgets
}

qtHaveModule(opengl): QT += opengl

HEADERS   += videoplayer.h \
             videoitem.h

SOURCES   += main.cpp \
             videoplayer.cpp \
             videoitem.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/customvideosurface/customvideoitem
INSTALLS += target
