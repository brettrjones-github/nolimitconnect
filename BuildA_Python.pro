#NOTE: this project should be built before Build_NolimitConnect.pro
#if Build_NolimitConnect.pro is built first the only symptom will be that kodi will not run correctly

TEMPLATE     = subdirs
CONFIG += no_docs_target
#NOTE: For Android be sure Enviroment ANDROID_NDK_PLATFORM is set to android-21 instead of android-16 ( required for some cpu detection etc )
#NOTE2: sometimes crashes QCreator on windows.. may need to build the projects below seperately

# build the project sequentially as listed in SUBDIRS !
# order is important so dependent libraries are available

SUBDIRS += $$PWD/Build1_PythonDepends.pro

SUBDIRS += $$PWD/Build2_PythonCoreLib.pro

SUBDIRS += $$PWD/Build3_PythonDllLibs.pro

SUBDIRS += $$PWD/Build4_MovePythonDlls.pro

SUBDIRS += $$PWD/Build5_NlcDependLibs.pro

SUBDIRS += $$PWD/Build6_NlcCore.pro

SUBDIRS += $$PWD/Build7_NlcAppOnly.pro

#optional build of most often changed files
#SUBDIRS += $$PWD/Build6_AppAndCore_Optional.pro

Build7_NlcAppOnly.pro.depends = Build6_NlcCore.pro
Build6_NlcCore.pro.depends = Build5_NlcDependLibs.pro
Build5_NlcDependLibs.depend = Build4_MovePythonDlls.pro
Build4_MovePythonDlls.pro.depends = Build3_PythonDllLibs.pro
Build3_PythonDllLibs.pro.depends = Build2_PythonCoreLib.pro
Build2_PythonCoreLib.pro.depends = Build1_PythonDepends.pro
