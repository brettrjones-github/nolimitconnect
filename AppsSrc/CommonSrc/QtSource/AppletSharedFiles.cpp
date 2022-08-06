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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletSharedFiles.h"
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
AppletSharedFiles::AppletSharedFiles( AppCommon& app, QWidget * parent, QString launchParam )
    : AppletBase( OBJNAME_APPLET_SHARED_FILES_LIST, app, parent )
    , m_ePluginType( ePluginTypeInvalid )
    , m_IsSelectAFileMode( !launchParam.isEmpty() ? true : false )
    , m_FileWasSelected( false )
    , m_SelectedFileType( 0 )
    , m_SelectedFileName( "" )
    , m_SelectedFileLen( 0 )
    , m_SelectedFileIsShared( false )
    , m_SelectedFileIsInSharedFiles( false )
    , m_eFileFilterType( eFileFilterAll )
    , m_FileFilterMask( VXFILE_TYPE_ALLNOTEXE )
{
    m_eFileFilterType = GuiParams::fileFilterToEnum( launchParam );

    setAppletType( eAppletSharedFiles );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    ui.m_DoubleTapInstructionLabel->setVisible( m_IsSelectAFileMode );

    connect( ui.m_FileItemList, SIGNAL( itemClicked( QListWidgetItem * ) ), this, SLOT( slotListItemClicked( QListWidgetItem * ) ) );
    connect( ui.m_FileItemList, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), this, SLOT( slotListItemDoubleClicked( QListWidgetItem * ) ) );

    connect( this, SIGNAL( signalToGuiFileList( VxMyFileInfo& ) ), this, SLOT( slotToGuiFileList( VxMyFileInfo& ) ) );
    connect( ui.m_AddFilesButton, SIGNAL( clicked() ), this, SLOT( slotAddFilesButtonClicked() ) );

    setFileFilter( m_eFileFilterType );
    connect( ui.m_FileFilterComboBox, SIGNAL( signalApplyFileFilter( unsigned char ) ), this, SLOT( slotApplyFileFilter( unsigned char ) ) );
    statusMsg( "Requesting SharedFiles File List " );
    m_MyApp.wantToGuiFileXferCallbacks( this, true );
    slotApplyFileFilter( ui.m_FileFilterComboBox->getCurrentFileFilterMask() );
    connectBarWidgets();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletSharedFiles::~AppletSharedFiles()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletSharedFiles::statusMsg( QString strMsg )
{
    //LogMsg( LOG_INFO, strMsg.toStdString().c_str() );
    ui.m_StatusMsgLabel->setText( strMsg );
}

//============================================================================
void AppletSharedFiles::showEvent( QShowEvent * ev )
{
    AppletBase::showEvent( ev );
    m_MyApp.wantToGuiFileXferCallbacks( this, true );
    slotRequestFileList();
}

//============================================================================
void AppletSharedFiles::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiFileXferCallbacks( this, false );
    AppletBase::hideEvent( ev );
}

//============================================================================
void AppletSharedFiles::toGuiFileList( VxMyFileInfo& fileInfo )
{
    if( fileInfo.getFullFileName().isEmpty() )
    {
        //setActionEnable( true );
        statusMsg( "List Get Completed" );
    }
    else if( fileInfo.getIsShared() )
    {
        addFile( fileInfo, fileInfo.getIsShared(), fileInfo.getIsInLibrary() );
    }
}

//============================================================================
void AppletSharedFiles::setFileFilter( EFileFilterType eFileFilter )
{
    m_eFileFilterType = eFileFilter;
    m_FileFilterMask = ui.m_FileFilterComboBox->getMaskFromFileFilterType( m_eFileFilterType );
    ui.m_FileFilterComboBox->setFileFilter( eFileFilter );
}

//============================================================================
void AppletSharedFiles::slotApplyFileFilter( unsigned char fileTypeMask )
{
    m_FileFilterMask = fileTypeMask;
    slotRequestFileList();
}

//============================================================================
void AppletSharedFiles::slotRequestFileList( void )
{
    clearFileList();
    m_FromGui.fromGuiGetFileLibraryList( m_FileFilterMask );
}

//============================================================================
FileShareItemWidget * AppletSharedFiles::fileToWidget( VxMyFileInfo& fileInfo, bool isShared, bool isInSharedFiles )
{
    FileShareItemWidget * item = new FileShareItemWidget( ui.m_FileItemList );
    item->setSizeHint( QSize( (int)(GuiParams::getGuiScale() * 200), GuiParams::getFileListEntryHeight() ) );

    FileItemInfo * poItemInfo = new FileItemInfo( fileInfo, 0, isShared, isInSharedFiles );
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
void AppletSharedFiles::slotListFileIconClicked( QListWidgetItem * item )
{
    slotListItemClicked( item );
}

//============================================================================
void AppletSharedFiles::slotListShareFileIconClicked( QListWidgetItem * item )
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
void AppletSharedFiles::slotListLibraryIconClicked( QListWidgetItem * item )
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
void AppletSharedFiles::slotListPlayIconClicked( QListWidgetItem * item )
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
                m_MyApp.launchApplet( appletType, m_MyApp.getCentralWidget(), "", assetId);
            }

            this->playFile( poInfo->getFullFileName(), assetId );
        }
    }
}

//============================================================================
void AppletSharedFiles::slotListShredIconClicked( QListWidgetItem * item )
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
bool AppletSharedFiles::confirmDeleteFile( QString fileName, bool shredFile )
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
FileItemInfo * AppletSharedFiles::widgetToFileItemInfo( FileShareItemWidget * item )
{
    return ( FileItemInfo * )item->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
FileShareItemWidget * AppletSharedFiles::findListEntryWidget( VxMyFileInfo& fileInfo )
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

    return nullptr;
}

//============================================================================
void AppletSharedFiles::slotAddFilesButtonClicked( void )
{
    ActivityBrowseFiles dlg( m_MyApp, eFileFilterAll, this );
    dlg.exec();
    clearFileList();
    statusMsg( "Requesting SharedFiles File List " );
    m_FromGui.fromGuiGetFileLibraryList( m_FileFilterMask );
}

//============================================================================
void AppletSharedFiles::addFile( VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary )
{
    FileShareItemWidget* existingItem = findItemByFileName( fileInfo.getFullFileName() );
    if( existingItem )
    {
        FileItemInfo* poItemInfo = existingItem->getFileItemInfo();
        if( poItemInfo )
        {
            poItemInfo->setIsInLibrary( isInLibrary );
            poItemInfo->setIsShared( isShared );
            existingItem->update();
        }
    }

    if( !existingItem )
    {
        FileShareItemWidget* item = fileToWidget( fileInfo, isShared, isInLibrary );
        if( item )
        {
            //LogMsg( LOG_INFO, "AppletSharedFiles::addFile: adding widget\n");
            ui.m_FileItemList->addItem( item );
            ui.m_FileItemList->setItemWidget( item, item );
        }
    }
}

//============================================================================
void AppletSharedFiles::slotHomeButtonClicked( void )
{
    closeApplet();
}

//============================================================================
//! user selected menu item
void AppletSharedFiles::slotListItemClicked( QListWidgetItem * item )
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
            m_SelectedFileIsInSharedFiles = poInfo->getIsInLibrary();
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
void AppletSharedFiles::slotListItemDoubleClicked( QListWidgetItem * item )
{
    slotListItemClicked( item );
}

//============================================================================
void AppletSharedFiles::clearFileList( void )
{
    ui.m_FileItemList->clear();
}

//============================================================================
FileShareItemWidget* AppletSharedFiles::findItemByFileName( QString& fileName )
{
    int iIdx = 0;
    FileShareItemWidget* poWidget;
    while( iIdx < ui.m_FileItemList->count() )
    {
        poWidget = (FileShareItemWidget*)ui.m_FileItemList->item( iIdx );
        if( poWidget )
        {
            FileItemInfo* poFileInfo = (FileItemInfo*)poWidget->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
            if( poFileInfo && (poFileInfo->getFullFileName() == fileName) )
            {
                return poWidget;
            }
        }

        iIdx++;
    }

    return nullptr;
}