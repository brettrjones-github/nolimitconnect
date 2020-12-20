

TEMPLATE     = subdirs
CONFIG += no_docs_target
#NOTE: For Android be sure Enviroment ANDROID_NDK_PLATFORM is set to android-21 instead of android-16 ( required for some cpu detection etc )
#NOTE2: sometimes crashes QCreator on windows.. may need to build the projects below seperately

# build the project sequentially as listed in SUBDIRS !
# order is important so dependent libraries are available

SUBDIRS += $$PWD/Build1_PythonDepends.pro

SUBDIRS += $$PWD/Build2_PythonLib.pro

SUBDIRS += $$PWD/Build3_DependLibs.pro

SUBDIRS += $$PWD/Build4_Core.pro

SUBDIRS += $$PWD/Build5_AppOnly.pro

#optional build of most often changed files
#SUBDIRS += $$PWD/Build6_AppAndCore_Optional.pro

Build5_AppOnly.pro.depends = Build4_Core.pro
Build4_Core.pro.depends = Build3_DependLibs.pro
Build3_DependLibs.pro.depends = Build2_PythonLib.pro
Build2_PythonLib.pro.depends = Build1_PythonDepends.pro
