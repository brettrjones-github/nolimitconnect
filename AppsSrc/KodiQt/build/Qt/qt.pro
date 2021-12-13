HEADERS       = $$PWD/../../AppSource/mainwindow.h \
                $$PWD/../../AppSource/svgview.h \
                $$PWD/../../AppSource/exportdialog.h

RESOURCES     = $$PWD/../../../CommonSrc/NoLimitConnect.qrc

SOURCES       = $$PWD/../../AppSource/main.cpp \
                $$PWD/../../AppSource/mainwindow.cpp \
                $$PWD/../../AppSource/svgview.cpp \
                $$PWD/../../AppSource/exportdialog.cpp
QT           += widgets svg

qtHaveModule(opengl): QT += opengl

CONFIG += console

# install
target.path = $$PWD/../../../bin/svg/svgviewer
INSTALLS += target
