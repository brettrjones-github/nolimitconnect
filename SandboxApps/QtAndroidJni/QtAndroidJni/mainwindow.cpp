#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Context.h"
#include "ApplicationInfo.h"
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <errno.h>
#include "VxFileUtil.h"
#include "VxDebug.h"
#include "JNIFile.h"
#include "guid.h"

#if defined( TARGET_OS_ANDROID )
static int android_read(void* cookie, char* buf, int size) {
    return AAsset_read((AAsset*)cookie, buf, size);
}

static int android_write(void* /*cookie*/, const char* /*buf*/, int /*size*/) {
    return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
    return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int android_close(void* cookie) {
    AAsset_close((AAsset*)cookie);
    return 0;
}

// must be established by someone else...
static AAssetManager* android_asset_manager = NULL;
void android_fopen_set_asset_manager(AAssetManager* manager) {
    android_asset_manager = manager;
}

FILE* android_fopen(const char* fname, const char* mode) {
    if(mode[0] == 'w') return NULL;

    AAsset* asset = AAssetManager_open(android_asset_manager, fname, 0);
    if(!asset) return NULL;

    return funopen(asset, android_read, android_write, android_seek, android_close);
}


// android:theme=”@android:style/Theme.NoTitleBar.Fullscreen”
bool CopyIfRequiredApkFile( std::string apkFileName, std::string destFile )
{
    bool result = false;
    if( !VxFileUtil::fileExists( destFile.c_str() ) )
    {
        std::string destDir = VxFileUtil::getJustPath( destFile );
        if( !VxFileUtil::directoryExists( destDir.c_str() ) )
        {
            VxFileUtil::makeDirectory( destDir.c_str() );
        }

        AAssetManager* mgr = android_asset_manager;
        assert(NULL != mgr);
        if( mgr )
        {
            AAsset* apkAssetFile = AAssetManager_open(mgr, apkFileName.c_str(), AASSET_MODE_UNKNOWN);
            assert( apkAssetFile );

            if ( apkAssetFile )
            {
                size_t assetLength = AAsset_getLength( apkAssetFile );

                __android_log_print(ANDROID_LOG_DEBUG, "Asset File", "Asset file size: %d\n", assetLength);

                char* buffer = (char*) malloc(assetLength + 1);
                AAsset_read( apkAssetFile, buffer, assetLength);
                buffer[ assetLength ] = 0;
                result = ( 0 == VxFileUtil::writeWholeFile( destFile.c_str(), buffer, assetLength ) );
                AAsset_close( apkAssetFile );
                free(buffer);
                if( !VxFileUtil::fileExists( destFile.c_str() ) )
                {
                    LogMsg( LOG_DEBUG, "Could not create file %s len %d", destFile.c_str(), assetLength );
                }
                else
                {
                    result = true;
                }
            }
            else
            {
                LogMsg( LOG_ERROR,  "Asset Manager Cannot open file %s", destFile.c_str() );
            }
        }
    }
    else
    {
        result = true;
    }

    return result;
}

bool CopyIfRequiredApkDirectory( std::string apkFileDir, std::string destDir )
{
    bool result = false;
    VxFileUtil::assureTrailingDirectorySlash( destDir );

    AAssetManager* assetMgr = android_asset_manager;
    AAssetDir* assetDir = AAssetManager_openDir( assetMgr, apkFileDir.c_str() );
    if( assetDir )
    {
        const char* fileName;
        while ((fileName = AAssetDir_getNextFileName(assetDir)) != NULL)
        {
            //__android_log_print(ANDROID_LOG_DEBUG, "Debug", filename);
            CopyIfRequiredApkFile( apkFileDir + "/" + fileName, destDir + fileName );
        }
    }
    else
    {
        LogMsg( LOG_DEBUG, "Could not open apk dir %s", apkFileDir.c_str() );
    }

    return result;
}
#endif // defined( TARGET_OS_ANDROID )

bool RunFileCopyTest()
{
    CJNIContext& jniContext = CJNIContext::getJniContext();
    AAssetManager* assetManager = jniContext.getAssetManager();
    android_fopen_set_asset_manager( assetManager );

    std::string apkResourceDir = jniContext.getPackageResourcePath();

    std::string cacheDir = jniContext.getCacheDir().getAbsolutePath();
    std::string cacheExternResDir = cacheDir + "/apk";

    std::string externalDir;
    std::string packageName = jniContext.getPackageName();
    CJNIFile androidPath = jniContext.getExternalFilesDir("");
    if (!androidPath)
      androidPath = jniContext.getDir(packageName.c_str(), 1);

    if (androidPath)
      externalDir = androidPath.getAbsolutePath();

    std::string settingsFile = "/assets/kodi/system/settings/settings.xml";
    //VxFileUtil::deleteFile( (externalDir + settingsFile).c_str() );
    bool result = CopyIfRequiredApkFile( "kodi/system/settings/settings.xml", externalDir + settingsFile );

    result &= CopyIfRequiredApkDirectory( "gotv/gui", externalDir + "/assets/nolimit/gui" );
    result &= CopyIfRequiredApkDirectory( "gotv/profile", externalDir + "/assets/nolimit/profile" );
    result &= CopyIfRequiredApkDirectory( "gotv/textures", externalDir + "/assets/nolimit/textures" );
    return result;
}


int testGuid( GuidGenerator generator, std::ostream &outStream )
{
  auto r1 = generator.newGuid();

  outStream << r1 << std::endl;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CJNIContext::getJniContext().mainWindowInialized();
    bool testResult = RunFileCopyTest();
    std::stringstream guidStream;
    testGuid( GuidGenerator( CJNIContext::getJniContext().getJNIEnv() ), guidStream);
    std::string guidStr =  guidStream.str();

    std::string strExecutablePath = CJNIContext::getJniContext().getApplicationInfo().nativeLibraryDir;
    char textBuf[16000];
    sprintf( textBuf, "nativeLib dir %s sizeof void * %d\n, int %d\n long %d\n longlong %d\n float %d\n double %d\nguid %s\ncopy apk test %s",
             strExecutablePath.c_str(),
             sizeof(void *),
             sizeof(int),
             sizeof(long),
             sizeof(long long),
             sizeof(float),
             sizeof(double),
             guidStr.c_str(),
             testResult ? "SUCCESS" : "FAIL"
             );
    ui->labelTextOut->setText(textBuf);
}

MainWindow::~MainWindow()
{
    delete ui;
}
