
#NOTE1: This project assumes Python has been built and the python dlls are in the correct assets sub folder

TEMPLATE     = subdirs
CONFIG += no_docs_target
#NOTE2: For Android be sure Enviroment ANDROID_NDK_PLATFORM is set to android-21 instead of android-16 ( required for some cpu detection etc )
#NOTE3: sometimes crashes QCreator on windows.. may need to build the projects below seperately

SUBDIRS += $$PWD/Build7_NlcAppOnly.pro



