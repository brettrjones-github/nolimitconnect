//============================================================================
// Copyright (C) 2015 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppGlobals.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include "FileActionMenu.h"
#include "ActivityYesNoMsgBox.h"
#include "ActivityMessageBox.h"
#include "MyIcons.h"
#include "GuiHelpers.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxDebug.h>

//============================================================================
FileActionMenu::FileActionMenu( AppCommon&	app, 
								QWidget*		parent, 
								FileInfo&	fileInfo, 
								bool			isShared, 
								bool			isInLibrary,
								VxNetIdent*	selectedFriend )
: ActivityBase( OBJNAME_FILE_ACTION_MENU, app, parent, eAppletMessengerFrame, true )
, m_FileInfo( fileInfo )
, m_IsShared( isShared )
, m_IsInLibrary( isInLibrary )
, m_SelectedFriend( selectedFriend )
, m_iMenuItemHeight( 34 )
{
	ui.setupUi(this);
	slotRepositionToParent();

    connect(ui.m_ExitButton, SIGNAL(clicked()), this, SLOT(slotHomeButtonClicked()));
    connect(ui.m_MenuItemList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));

	setupFileInfo();
	setupMenuItems();
}

//============================================================================
FileActionMenu::~FileActionMenu()
{
}

//============================================================================
void FileActionMenu::setTitle( QString strTitle )
{
	ui.m_TitleLabel->setText(strTitle);
}

//============================================================================
void FileActionMenu::setupFileInfo( void )
{
	ui.m_FileIconButton->setIcon( getMyIcons().getFileIcon( m_FileInfo.getFileType() ) );

	QString fileName = m_FileInfo.getFullFileName().c_str();
	QString path = m_FileInfo.getFilePath().c_str();

	ui.m_FileNameLabel->setText( fileName );
	ui.m_FileSizeLabel->setText( GuiParams::describeFileLength( m_FileInfo.getFileLength() ) );
	ui.m_FilePathLabel->setTextBreakAnywhere( path, 3 );
}

//============================================================================
void FileActionMenu::setupMenuItems( void )
{
	int menuIndex = 0;
	addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconPlayNormal ), QObject::tr("Open File") );
	m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionOpen ) );
	menuIndex++;
	if( m_SelectedFriend )
	{
		if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypePersonFileXfer ) )
		{
			QString menuText = "Send File To ";
			menuText += m_SelectedFriend->getOnlineName();
			addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconSendFileNormal ), menuText );
			m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionSendToFriend ) );
		}
		else
		{
			addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconSendFileDisabled ), QObject::tr("Insufficient Permission To Send File") );
			m_MenuList.push_back( FileMenuItemAction( menuIndex, eFileMenuActionNone ) );
		}

		menuIndex++;
	}

	if( m_MyApp.getEngine().getMyPktAnnounce().getPluginPermission( ePluginTypeFileShareServer ) )
	{
		addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconShareFilesCancel ), QObject::tr("File Sharing Is Disabled") );
		m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionNone ) );
	}
	else if( m_IsShared )
	{
		addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconShareFilesNormal ), QObject::tr("Stop Sharing This File")  );
		m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionRemoveShare ) );
	}
	else
	{
		addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconShareFilesCancel), QObject::tr("Start Sharing This File")  );
		m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionAddShare ) );
	}

	menuIndex++;
	if( m_IsInLibrary )
	{
		addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconLibraryNormal ), QObject::tr("Remove From My Library")  );
		m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionRemoveFromLibrary ) );
	}
	else
	{
		addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconLibraryCancel ), QObject::tr("Add To My Library") );
		m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionAddToLibrary ) );
	}

	menuIndex++;
	addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconTrash ), QObject::tr("Delete File") );
	m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionDelete ) );

	menuIndex++;
	addMenuItem( menuIndex, getMyIcons().getIcon( eMyIconShredderNormal ), QObject::tr("Shred File") );
	m_MenuList.push_back( FileMenuItemAction(menuIndex, eFileMenuActionShred ) );
}

//============================================================================
void FileActionMenu::addMenuItem( int iItemId, QIcon& oIcon, QString strMenuItemText )
{
	QListWidgetItem* poMenuItem = new QListWidgetItem( strMenuItemText );
	poMenuItem->setIcon( oIcon );
	poMenuItem->setData( Qt::UserRole, iItemId );
	ui.m_MenuItemList->addItem( poMenuItem );
}

//============================================================================
//! clicked the upper right x button
void FileActionMenu::slotHomeButtonClicked( void )
{
	closeApplet();
}

//============================================================================
void FileActionMenu::itemClicked(QListWidgetItem*item)
{
	// resignal with id set by user
	int iItemId = item->data(Qt::UserRole).toInt();
	std::vector<FileMenuItemAction>::iterator iter;
	for( iter = m_MenuList.begin(); iter != m_MenuList.end(); ++iter )
	{
		if( iItemId != (*iter).getMenuIndex() )
		{
			continue;
		}

		EFileMenuAction menuAction = (*iter).getMenuAction();
		switch( menuAction )
		{
		case eFileMenuActionOpen:
			m_MyApp.getPlayerMgr().playFile( m_FileInfo.getFullFileName().c_str() );
			break;

		case eFileMenuActionSendToFriend:
			if( m_SelectedFriend->isMyAccessAllowedFromHim( ePluginTypePersonFileXfer ) )
			{
				if( false == m_FromGui.fromGuiMakePluginOffer(	ePluginTypePersonFileXfer, 
																m_SelectedFriend->getMyOnlineId(), 
                                                                0,
																"", 
																m_FileInfo.getFullFileName().c_str(),
                                                                nullptr ) )
				{
					ActivityMessageBox errMsgBox( m_MyApp, this, LOG_INFO, "Could Not Offer file because %s is offline", m_SelectedFriend->getOnlineName() );
					errMsgBox.exec();
				}
			}
			break;

		case eFileMenuActionAddShare:
			m_Engine.fromGuiSetFileIsShared( m_FileInfo, true );
			break;

		case eFileMenuActionRemoveShare:
			m_Engine.fromGuiSetFileIsShared( m_FileInfo, false );
			break;

		case eFileMenuActionAddToLibrary:
			m_Engine.fromGuiSetFileIsInLibrary( m_FileInfo, true );
			break;

		case eFileMenuActionRemoveFromLibrary:
			m_Engine.fromGuiSetFileIsInLibrary( m_FileInfo, false );
			break;

		case eFileMenuActionDelete:
			if( confirmDeleteFile( false ) )
			{
				int result = m_Engine.fromGuiDeleteFile( m_FileInfo.getFullFileName(), false );
				if( result )
				{
					QString title = "Could Not Delete File";
					QString bodyText =  QString("OS returned error %1 accessing file %2 for deletion").arg(result).arg(m_FileInfo.getFullFileName().c_str());
					ActivityYesNoMsgBox dlg( m_MyApp, &m_MyApp, title, bodyText );
					dlg.hideCancelButton();
					dlg.exec();
				}
			}
			break;

		case eFileMenuActionShred:
			if( confirmDeleteFile( true ) )
			{
				int result = m_Engine.fromGuiDeleteFile( m_FileInfo.getFullFileName(), true );
				if( result )
				{
					QString title = "Could Not Shred File";
					QString bodyText =  QString("OS returned error %1 accessing file %2 for shredding").arg(result).arg(m_FileInfo.getFullFileName().c_str());
					ActivityYesNoMsgBox dlg( m_MyApp, &m_MyApp, title, bodyText );
					dlg.hideCancelButton();
					dlg.exec();
				}
			}
			break;
		default:
			break;
		}

		break;
	}

	closeApplet();
}

//============================================================================
bool FileActionMenu::confirmDeleteFile( bool shredFile )
{
	bool acceptAction = true;
	bool isConfirmDisabled = m_MyApp.getAppSettings().getIsConfirmDeleteDisabled();
	if( false == isConfirmDisabled )
	{
		QString title = shredFile ?  "Confirm Shred File" : "Confirm Delete File";
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
		if( false == (QDialog::Accepted == dlg.exec()))
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
