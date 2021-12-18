//============================================================================
// Copyright (C) 2019 Brett R. Jones
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include "AppletLibrary.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AppCommon.h"	
#include "AppSettings.h"

#include "AppletDownloads.h"
#include "ActivityBrowseFiles.h"
#include "ActivityYesNoMsgBox.h"

#include "FileShareItemWidget.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "FileItemInfo.h"
#include "FileActionMenu.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <PktLib/VxSearchDefs.h>
#include <NetLib/VxFileXferInfo.h>
#include <CoreLib/VxFileInfo.h>
#include <CoreLib/VxDebug.h>

//============================================================================
AppletLibrary::AppletLibrary( AppCommon& app, QWidget * parent, int launchParam )
    : AppletBase( OBJNAME_APPLET_LIBRARY, app, parent )
    , m_ePluginType( ePluginTypeInvalid )
    , m_IsSelectAFileMode( launchParam ? true : false )
    , m_FileWasSelected( false )
    , m_SelectedFileType( 0 )
    , m_SelectedFileName( "" )
    , m_SelectedFileLen( 0 )
    , m_SelectedFileIsShared( false )
    , m_SelectedFileIsInLibrary( false )
    , m_eFileFilterType( eFileFilterAll )
    , m_FileFilterMask( VXFILE_TYPE_ALLNOTEXE )
{
    if( launchParam )
    {
        switch( launchParam )
        {
        case VXFILE_TYPE_PHOTO:
            m_eFileFilterType = eFileFilterPhoto;
            break;
        case VXFILE_TYPE_AUDIO:
            m_eFileFilterType = eFileFilterAudio;
            break;
        case VXFILE_TYPE_VIDEO:
            m_eFileFilterType = eFileFilterVideo;
            break;
        case VXFILE_TYPE_DOC:
            m_eFileFilterType = eFileFilterDocuments;
            break;
        case VXFILE_TYPE_ARCHIVE_OR_CDIMAGE:
            m_eFileFilterType = eFileFilterArchive;
            break;
        case VXFILE_TYPE_OTHER:
            m_eFileFilterType = eFileFilterOther;
            break;
        }
    }

    setAppletType( eAppletLibrary );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    ui.m_DoubleTapInstructionLabel->setVisible( m_IsSelectAFileMode );

    connect( ui.m_FileItemList, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( slotListItemClicked( QListWidgetItem * ) ) );
    connect( ui.m_FileItemList, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), this, SLOT( slotListItemDoubleClicked( QListWidgetItem * ) ) );

    connect( this, SIGNAL( signalToGuiFileList( VxMyFileInfo& ) ), this, SLOT( slotToGuiFileList( VxMyFileInfo& ) ) );
    connect( ui.m_AddFilesButton, SIGNAL( clicked() ), this, SLOT( slotAddFilesButtonClicked() ) );

    setFileFilter( m_eFileFilterType );
    connect( ui.m_FileFilterComboBox, SIGNAL( signalApplyFileFilter( unsigned char ) ), this, SLOT( slotApplyFileFilter( unsigned char ) ) );
    statusMsg( "Requesting Library File List " );
    m_MyApp.wantToGuiFileXferCallbacks( this, this, true );
    slotApplyFileFilter( ui.m_FileFilterComboBox->getCurrentFileFilterMask() );
    connectBarWidgets();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletLibrary::~AppletLibrary()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletLibrary::statusMsg( QString strMsg )
{
    //LogMsg( LOG_INFO, strMsg.toStdString().c_str() );
    ui.m_StatusMsgLabel->setText( strMsg );
}

//============================================================================
void AppletLibrary::showEvent( QShowEvent * ev )
{
    AppletBase::showEvent( ev );
    m_MyApp.setIsLibraryActivityActive( true );
    m_MyApp.wantToGuiFileXferCallbacks( this, this, true );
    slotRequestFileList();
}

//============================================================================
void AppletLibrary::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiFileXferCallbacks( this, this, false );
    AppletBase::hideEvent( ev );
    m_MyApp.setIsLibraryActivityActive( false );
}

//============================================================================
void AppletLibrary::toGuiFileList( void * userData, VxMyFileInfo& fileInfo )
{
    emit signalToGuiFileList( fileInfo );
}

//============================================================================
void AppletLibrary::slotToGuiFileList( VxMyFileInfo& fileInfo )
{
    if( fileInfo.getFullFileName().isEmpty() )
    {
        //setActionEnable( true );
        statusMsg( "List Get Completed" );
    }
    else
    {
        addFile( fileInfo, fileInfo.getIsShared(), fileInfo.getIsInLibrary() );
    }
}

//============================================================================
void AppletLibrary::setFileFilter( EFileFilterType eFileFilter )
{
    m_eFileFilterType = eFileFilter;
    m_FileFilterMask = ui.m_FileFilterComboBox->getMaskFromFileFilterType( m_eFileFilterType );
    ui.m_FileFilterComboBox->setFileFilter( eFileFilter );
}

//============================================================================
void AppletLibrary::slotApplyFileFilter( unsigned char fileTypeMask )
{
    m_FileFilterMask = fileTypeMask;
    slotRequestFileList();
}

//============================================================================
void AppletLibrary::slotRequestFileList( void )
{
    clearFileList();
    m_FromGui.fromGuiGetFileLibraryList( m_FileFilterMask );
}

//============================================================================
FileShareItemWidget * AppletLibrary::fileToWidget( VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary )
{
    FileShareItemWidget * item = new FileShareItemWidget( ui.m_FileItemList );
    item->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    FileItemInfo * poItemInfo = new FileItemInfo( fileInfo, 0, isShared, isInLibrary );
    item->QListWidgetItem::setData( Qt::UserRole + 1, QVariant( ( quint64 )poItemInfo ) );
    connect( item, SIGNAL( signalFileShareItemClicked( QListWidgetItem* ) ), this, SLOT( slotItemClicked( QListWidgetItem* ) ) );

    connect( item,
             SIGNAL( signalFileShareItemClicked( QListWidgetItem* ) ),
             this,
             SLOT( slotListItemClicked( QListWidgetItem* ) ) );

    connect( item,
             SIGNAL( signalFileIconClicked( QListWidgetItem* ) ),
             this,
             SLOT( slotListFileIconClicked( QListWidgetItem* ) ) );

    connect( item,
             SIGNAL( signalPlayButtonClicked( QListWidgetItem* ) ),
             this,
             SLOT( slotListPlayIconClicked( QListWidgetItem* ) ) );

    connect( item,
             SIGNAL( signalLibraryButtonClicked( QListWidgetItem* ) ),
             this,
             SLOT( slotListLibraryIconClicked( QListWidgetItem* ) ) );

    connect( item,
             SIGNAL( signalFileShareButtonClicked( QListWidgetItem* ) ),
             this,
             SLOT( slotListShareFileIconClicked( QListWidgetItem* ) ) );

    connect( item,
             SIGNAL( signalShredButtonClicked( QListWidgetItem* ) ),
             this,
             SLOT( slotListShredIconClicked( QListWidgetItem* ) ) );

    item->updateWidgetFromInfo();
    return item;
}

//============================================================================
void AppletLibrary::slotListFileIconClicked( QListWidgetItem * item )
{
    slotListItemClicked( item );
}

//============================================================================
void AppletLibrary::slotListShareFileIconClicked( QListWidgetItem * item )
{
    FileItemInfo * poInfo = ( ( FileShareItemWidget * )item )->getFileItemInfo();
    if( poInfo )
    {
        if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
        {
        }
        else
        {
            // is file
            poInfo->toggleIsShared();
            ( ( FileShareItemWidget* )item )->updateWidgetFromInfo();
            m_Engine.fromGuiSetFileIsShared( poInfo->getFullFileName().toUtf8().constData(), poInfo->getIsShared() );
        }
    }
}

//============================================================================
void AppletLibrary::slotListLibraryIconClicked( QListWidgetItem * item )
{
    FileItemInfo * poInfo = ( ( FileShareItemWidget * )item )->getFileItemInfo();
    if( poInfo )
    {
        if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
        {
        }
        else
        {
            // is file
            poInfo->toggleIsInLibrary();
            ( ( FileShareItemWidget* )item )->updateWidgetFromInfo();
            m_Engine.fromGuiAddFileToLibrary( poInfo->getFullFileName().toUtf8().constData(),
                                              poInfo->getIsInLibrary(),
                                              poInfo->getFileHashId().getHashData() );
        }
    }
}

//============================================================================
void AppletLibrary::slotListPlayIconClicked( QListWidgetItem * item )
{
    FileItemInfo * poInfo = ( ( FileShareItemWidget * )item )->getFileItemInfo();
    if( poInfo )
    {
        if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
        {
        }
        else
        {
            // determine asset id for this file.. may be different than the one given
            VxGUID assetId = poInfo->getAssetId();
            std::string assetFileName = poInfo->getFullFileName().toUtf8().constData();
            AssetBaseInfo* assetInfo = m_MyApp.getEngine().getAssetMgr().findAsset( assetFileName );
            if( assetInfo && assetInfo->getAssetUniqueId().isVxGUIDValid() )
            {
                assetId = assetInfo->getAssetUniqueId();
                poInfo->setAssetId( assetId );
            }

            EApplet appletType = GuiHelpers::getAppletThatPlaysFile( m_MyApp, poInfo->getFileType(), poInfo->getFullFileName(), assetId );
            if( appletType != eAppletUnknown && assetId.isVxGUIDValid() )
            {
                // launch the applet that plays this file
                m_MyApp.launchApplet( appletType, m_MyApp.getCentralWidget(), assetId );
            }

            this->playFile( poInfo->getFullFileName(), assetId );
        }
    }
}

//============================================================================
void AppletLibrary::slotListShredIconClicked( QListWidgetItem * item )
{
    FileItemInfo * poInfo = ( ( FileShareItemWidget * )item )->getFileItemInfo();
    if( poInfo )
    {
        if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
        {
        }
        else
        {
            // shred file
            QString fileName = poInfo->getFullFileName();
            if( confirmDeleteFile( fileName, true ) )
            {
                ui.m_FileItemList->removeItemWidget( item );
                m_Engine.fromGuiDeleteFile( fileName.toUtf8().constData(), true );
            }
        }
    }
}

//============================================================================
bool AppletLibrary::confirmDeleteFile( QString fileName, bool shredFile )
{
    bool acceptAction = true;
    bool isConfirmDisabled = m_MyApp.getAppSettings().getIsConfirmDeleteDisabled();
    if( false == isConfirmDisabled )
    {
        QString title = shredFile ? "Confirm Shred File" : "Confirm Delete File";
        QString bodyText = "";
        if( shredFile )
        {
            bodyText = "Are You Sure You Want To Write Random Data Into The File Then Delete From The Device?";
        }
        else
        {
            bodyText = "Are You Sure To Delete The File From The Device?";
        }

        ActivityYesNoMsgBox dlg( m_MyApp, &m_MyApp, title, bodyText );
        dlg.makeNeverShowAgainVisible( true );
        if( false == ( QDialog::Accepted == dlg.exec() ) )
        {
            acceptAction = false;
        }

        if( dlg.wasNeverShowAgainChecked() )
        {
            m_MyApp.getAppSettings().setIsConfirmDeleteDisabled( true );
        }
    }

    return acceptAction;
}

//============================================================================
//!	get friend from QListWidgetItem data
FileItemInfo * AppletLibrary::widgetToFileItemInfo( FileShareItemWidget * item )
{
    return ( FileItemInfo * )item->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
FileShareItemWidget * AppletLibrary::findListEntryWidget( VxMyFileInfo& fileInfo )
{
    int iIdx = 0;
    FileShareItemWidget * poWidget;
    while( iIdx < ui.m_FileItemList->count() )
    {
        poWidget = ( FileShareItemWidget * )ui.m_FileItemList->item( iIdx );
        if( poWidget )
        {
            FileItemInfo * poFileInfo = ( FileItemInfo * )poWidget->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
            if( poFileInfo && ( poFileInfo->getFullFileName() == fileInfo.getFullFileName() ) )
            {
                return poWidget;
            }
        }

        iIdx++;
    }

    return NULL;
}

//============================================================================
void AppletLibrary::slotAddFilesButtonClicked( void )
{
    ActivityBrowseFiles dlg( m_MyApp, eFileFilterAll, this );
    dlg.exec();
    clearFileList();
    statusMsg( "Requesting Library File List " );
    m_FromGui.fromGuiGetFileLibraryList( m_FileFilterMask );
}

//============================================================================
void AppletLibrary::addFile( VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary )
{
    FileShareItemWidget * item = fileToWidget( fileInfo, isShared, isInLibrary );
    if( item )
    {
        //LogMsg( LOG_INFO, "AppletLibrary::addFile: adding widget\n");
        ui.m_FileItemList->addItem( item );
        ui.m_FileItemList->setItemWidget( item, item );
    }
}

//============================================================================
void AppletLibrary::slotHomeButtonClicked( void )
{
    close();
}

//============================================================================
//! user selected menu item
void AppletLibrary::slotListItemClicked( QListWidgetItem * item )
{
    FileItemInfo * poInfo = ( FileItemInfo * )item->data( Qt::UserRole + 1 ).toLongLong();
    if( poInfo )
    {
        VxMyFileInfo& fileInfo = poInfo->getMyFileInfo();
        if( m_IsSelectAFileMode )
        {
            m_FileWasSelected = true;
            m_SelectedFileType = fileInfo.getFileType();
            m_SelectedFileName = fileInfo.getFullFileName().toUtf8().constData();
            m_SelectedFileLen = fileInfo.getFileLength();
            m_SelectedFileIsShared = poInfo->getIsShared();
            m_SelectedFileIsInLibrary = poInfo->getIsInLibrary();
            accept();
        }
        else
        {
            //FileActionMenu fileActionMenuDialog(	m_MyApp, 
            //										this, 
            //										poInfo->getMyFileInfo(),
            //										poInfo->getIsShared(),
            //										poInfo->getIsInLibrary() );
            //fileActionMenuDialog.exec();
            playFile( fileInfo.getFullFileName() );
        }
    }
}

//============================================================================
//! user double clicked menu item
void AppletLibrary::slotListItemDoubleClicked( QListWidgetItem * item )
{
    slotListItemClicked( item );
}

//============================================================================
void AppletLibrary::clearFileList( void )
{
    ui.m_FileItemList->clear();
}
