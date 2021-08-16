TEMPLATE = app
TARGET = customvideowidget

QT += widgets
versionAtMost(QT_VERSION, 5.15.2){
    QT += multimedia
    QT += multimediawidgets
}

HEADERS = \
    videoplayer.h \
    videowidget.h \
    videowidgetsurface.h

SOURCES = \
    main.cpp \
    videoplayer.cpp \
    videowidget.cpp \
    videowidgetsurface.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/customvideosurface/customvideowidget
INSTALLS += target
