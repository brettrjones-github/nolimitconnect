
# a subset of GoTvAll.pro to avoid rebuilding rarely changed libs

TEMPLATE     = subdirs
CONFIG += no_docs_target


# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered


TEMPLATE     = subdirs
CONFIG += no_docs_target
#NOTE: For Android be sure Enviroment ANDROID_NDK_PLATFORM is set to android-21 instead of android-16 ( required for some cpu detection etc )
#NOTE2: sometimes crashes QCreator on windows.. may need to build the projects below seperately

# build the project sequentially as listed in SUBDIRS !
# order is important so dependent libraries are available

SUBDIRS += $$PWD/Build4_Core.pro

SUBDIRS += $$PWD/Build5_AppOnly.pro








