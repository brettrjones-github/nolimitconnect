
DEFINES += "STATICLIB"
DEFINES += "SQLITE_OMIT_LOAD_EXTENSION"

HEADERS += 	$$PWD/AppCompilerConfig.h \
    $$PWD/AppCommonConfig.h \
    $$PWD/AppCpuArchDefines.h \
    $$PWD/AppDependLibrariesConfig.h \
    $$PWD/AppTargetOsConfig.h \
    $$PWD/DependLibs/CoreLib/AppErr.h \
    $$PWD/DependLibs/CoreLib/AssetDefs.h \
	$$PWD/DependLibs/CoreLib/BinaryBlob.h \
    $$PWD/DependLibs/CoreLib/Blowfish.h \
    $$PWD/DependLibs/CoreLib/DbBase.h \
    $$PWD/DependLibs/CoreLib/DbDataset.h \
    $$PWD/DependLibs/CoreLib/DbQueryData.h \
    $$PWD/DependLibs/CoreLib/dirent_sim.h \
    $$PWD/DependLibs/CoreLib/Invite.h \
    $$PWD/DependLibs/CoreLib/IsBigEndianCpu.h \
    $$PWD/DependLibs/CoreLib/MediaCallbackInterface.h \
    $$PWD/DependLibs/CoreLib/ObjectCommon.h \
    $$PWD/DependLibs/CoreLib/OsDetect.h \
    $$PWD/DependLibs/CoreLib/rsa.h \
    $$PWD/DependLibs/CoreLib/SHA1.h \
    $$PWD/DependLibs/CoreLib/Sha1ClientInfo.h \
    $$PWD/DependLibs/CoreLib/Sha1GeneratorCallback.h \
    $$PWD/DependLibs/CoreLib/Sha1GeneratorMgr.h \
    $$PWD/DependLibs/CoreLib/Sha1Info.h \
    $$PWD/DependLibs/CoreLib/StdOutRedirect.h \
	$$PWD/DependLibs/CoreLib/StringListBinary.h \
	$$PWD/DependLibs/CoreLib/TimeIntervalEstimator.h \
    $$PWD/DependLibs/CoreLib/VarToString.h \
    $$PWD/DependLibs/CoreLib/VxAppInfo.h \
    $$PWD/DependLibs/CoreLib/VxBuffer.h \
    $$PWD/DependLibs/CoreLib/VxChop.h \
    $$PWD/DependLibs/CoreLib/VxChopMap.h \
    $$PWD/DependLibs/CoreLib/VxColorUtil.h \
    $$PWD/DependLibs/CoreLib/VxCrypto.h \
    $$PWD/DependLibs/CoreLib/VxDebug.h \
    $$PWD/DependLibs/CoreLib/VxDefs.h \
    $$PWD/DependLibs/CoreLib/VxFileCopier.h \
    $$PWD/DependLibs/CoreLib/VxFileCrc32.h \
    $$PWD/DependLibs/CoreLib/VxFileInfo.h \
    $$PWD/DependLibs/CoreLib/VxFileIsTypeFunctions.h \
    $$PWD/DependLibs/CoreLib/VxFileLists.h \
    $$PWD/DependLibs/CoreLib/VxFileShredder.h \
    $$PWD/DependLibs/CoreLib/VxFileShredderDb.h \
    $$PWD/DependLibs/CoreLib/VxFileTypeMasks.h \
    $$PWD/DependLibs/CoreLib/VxFileUtil.h \
    $$PWD/DependLibs/CoreLib/VxGUID.h \
    $$PWD/DependLibs/CoreLib/VxGUIDList.h \
    $$PWD/DependLibs/CoreLib/VxGuidPairList.h \
    $$PWD/DependLibs/CoreLib/VxGlobals.h \
    $$PWD/DependLibs/CoreLib/VxKeyDefs.h \
    $$PWD/DependLibs/CoreLib/VxLinuxOnly.h \
    $$PWD/DependLibs/CoreLib/VxMacros.h \
    $$PWD/DependLibs/CoreLib/VxMathDef.h \
    $$PWD/DependLibs/CoreLib/VxMathVec2f.h \
    $$PWD/DependLibs/CoreLib/VxMathVec3f.h \
    $$PWD/DependLibs/CoreLib/VxMathVec4f.h \
    $$PWD/DependLibs/CoreLib/VxMutex.h \
    $$PWD/DependLibs/CoreLib/VxParse.h \
    $$PWD/DependLibs/CoreLib/VxProfile.h \
    $$PWD/DependLibs/CoreLib/VxPtopUrl.h \
    $$PWD/DependLibs/CoreLib/VxRefCount.h \
    $$PWD/DependLibs/CoreLib/VxSemaphore.h \
    $$PWD/DependLibs/CoreLib/VxSettings.h \
    $$PWD/DependLibs/CoreLib/VxSha1Hash.h \
    $$PWD/DependLibs/CoreLib/VxShortcut.h \
    $$PWD/DependLibs/CoreLib/VxSingleton.h \
    $$PWD/DependLibs/CoreLib/VxStringUtils.h \
    $$PWD/DependLibs/CoreLib/VxSupportedFiles.h \
    $$PWD/DependLibs/CoreLib/VxTextStreamReader.h \
    $$PWD/DependLibs/CoreLib/VxThread.h \
    $$PWD/DependLibs/CoreLib/VxTimeDefs.h \
    $$PWD/DependLibs/CoreLib/VxTimeUtil.h \
    $$PWD/DependLibs/CoreLib/VxTime.h \
    $$PWD/DependLibs/CoreLib/VxTimer.h \
    $$PWD/DependLibs/CoreLib/VxValuePair.h \
    $$PWD/DependLibs/CoreLib/VxXml.h \
    $$PWD/DependLibs/CoreLib/VxMd5.h \
    $$PWD/DependLibs/CoreLib/VxUrl.h \
    $$PWD/DependLibs/CoreLib/VxLinuxOnly.h \
    $$PWD/DependLibs/CoreLib/VxJava.h

SOURCES += 	\
    $$PWD/DependLibs/CoreLib/AppErr_corelib.cpp \
    $$PWD/DependLibs/CoreLib/AssetDefs.cpp \
	$$PWD/DependLibs/CoreLib/BinaryBlob.cpp \
    $$PWD/DependLibs/CoreLib/Blowfish_corelib.cpp \
    $$PWD/DependLibs/CoreLib/DbBase_corelib.cpp \
    $$PWD/DependLibs/CoreLib/DbDataset_corelib.cpp \
    $$PWD/DependLibs/CoreLib/DbQueryData_corelib.cpp \
    $$PWD/DependLibs/CoreLib/ObjectCommon_corelib.cpp \
    $$PWD/DependLibs/CoreLib/OsDetect_corelib.cpp \
    $$PWD/DependLibs/CoreLib/rsa_eay_corelib.c \
    $$PWD/DependLibs/CoreLib/rsa_gen_corelib.c \
    $$PWD/DependLibs/CoreLib/SHA1_corelib.cpp \
    $$PWD/DependLibs/CoreLib/Sha1ClientInfo.cpp \
    $$PWD/DependLibs/CoreLib/Sha1GeneratorMgr.cpp \
    $$PWD/DependLibs/CoreLib/Sha1Info.cpp \
    $$PWD/DependLibs/CoreLib/sqlite3_corelib.c \
    $$PWD/DependLibs/CoreLib/sqlitedataset_corelib.cpp \
    $$PWD/DependLibs/CoreLib/StdOutRedirect_corelib.cpp \
	$$PWD/DependLibs/CoreLib/StringListBinary.cpp \
    $$PWD/DependLibs/CoreLib/TimeIntervalEstimator.cpp \
    $$PWD/DependLibs/CoreLib/VxAppInfo.cpp \
    $$PWD/DependLibs/CoreLib/Invite.cpp \
    $$PWD/DependLibs/CoreLib/VxBuffer.cpp \
    $$PWD/DependLibs/CoreLib/VxChop.cpp \
    $$PWD/DependLibs/CoreLib/VxChopMap.cpp \
    $$PWD/DependLibs/CoreLib/VxColorUtil.cpp \
    $$PWD/DependLibs/CoreLib/VxCrypto.cpp \
    $$PWD/DependLibs/CoreLib/VxDebug.cpp \
    $$PWD/DependLibs/CoreLib/VxDefs.cpp \
    $$PWD/DependLibs/CoreLib/VxFileCopier.cpp \
    $$PWD/DependLibs/CoreLib/VxFileCrc32.cpp \
    $$PWD/DependLibs/CoreLib/VxFileInfo.cpp \
    $$PWD/DependLibs/CoreLib/VxFileIsTypeFunctions.cpp \
    $$PWD/DependLibs/CoreLib/VxFileLists.cpp \
    $$PWD/DependLibs/CoreLib/VxFileShredder.cpp \
    $$PWD/DependLibs/CoreLib/VxFileShredderDb.cpp \
    $$PWD/DependLibs/CoreLib/VxFileUtil.cpp \
    $$PWD/DependLibs/CoreLib/VxFunctionsMissingInPosix.cpp \
    $$PWD/DependLibs/CoreLib/VxFunctionsMissingInWindows.cpp \
    $$PWD/DependLibs/CoreLib/VxGUID.cpp \
    $$PWD/DependLibs/CoreLib/VxGUIDList.cpp \
    $$PWD/DependLibs/CoreLib/VxGuidPairList.cpp \
    $$PWD/DependLibs/CoreLib/VxGlobals.cpp \
    $$PWD/DependLibs/CoreLib/VxLinuxOnly.cpp \
    $$PWD/DependLibs/CoreLib/VxMathDef.cpp \
    $$PWD/DependLibs/CoreLib/VxMathVec3f.cpp \
    $$PWD/DependLibs/CoreLib/VxMutex.cpp \
    $$PWD/DependLibs/CoreLib/VxParse.cpp \
    $$PWD/DependLibs/CoreLib/VxProfile.cpp \
    $$PWD/DependLibs/CoreLib/VxPtopUrl.cpp \
    $$PWD/DependLibs/CoreLib/VxRefCount.cpp \
    $$PWD/DependLibs/CoreLib/VxSemaphore.cpp \
    $$PWD/DependLibs/CoreLib/VxSettings.cpp \
    $$PWD/DependLibs/CoreLib/VxSha1Hash.cpp \
    $$PWD/DependLibs/CoreLib/VxShortcut.cpp \
    $$PWD/DependLibs/CoreLib/VxStringUtils.cpp \
    $$PWD/DependLibs/CoreLib/VxTextStreamReader.cpp \
    $$PWD/DependLibs/CoreLib/VxThread.cpp \
    $$PWD/DependLibs/CoreLib/VxTimeUtil.cpp \
    $$PWD/DependLibs/CoreLib/VxTime.cpp \
    $$PWD/DependLibs/CoreLib/VxTimer.cpp \
    $$PWD/DependLibs/CoreLib/VxValuePair.cpp \
    $$PWD/DependLibs/CoreLib/VxXml.cpp \
    $$PWD/DependLibs/CoreLib/VxUrl.cpp \
    $$PWD/DependLibs/CoreLib/VxJava.cpp \
    $$PWD/DependLibs/CoreLib/VxFunctionsMissingInAndroid.cpp \
    $$PWD/DependLibs/CoreLib/VxMd5.cpp \



