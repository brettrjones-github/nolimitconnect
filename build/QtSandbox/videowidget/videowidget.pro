TEMPLATE = app
TARGET = videowidget

versionAtMost(QT_VERSION, 5.15.2){
    QT += multimedia
    QT += multimediawidgets
}

HEADERS = \
    videoplayer.h

SOURCES = \
    main.cpp \
    videoplayer.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/videowidget
INSTALLS += target

QT+=widgets
