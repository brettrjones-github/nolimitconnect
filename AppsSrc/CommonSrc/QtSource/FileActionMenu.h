#pragma once
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

#include "ActivityBase.h"
#include "ui_FileActionMenu.h"
#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>

class VxNetIdent;
class AppCommon;

enum EFileMenuAction
{
	eFileMenuActionNone,
	eFileMenuActionOpen,
	eFileMenuActionSendToFriend,
	eFileMenuActionAddShare,
	eFileMenuActionRemoveShare,
	eFileMenuActionAddToLibrary,
	eFileMenuActionRemoveFromLibrary,
	eFileMenuActionDelete,
	eFileMenuActionShred,

	eMaxFileMenuAction
};

class FileMenuItemAction
{
public:
	FileMenuItemAction( int menuIndex, EFileMenuAction action )
	: m_MenuIndex( menuIndex )
	, m_MenuAction( action )
	{
	}

	FileMenuItemAction( const FileMenuItemAction& rhs )
	{
		*this = rhs;
	}

	FileMenuItemAction& operator =( const FileMenuItemAction& rhs )
	{
		m_MenuIndex				= rhs.m_MenuIndex;
		m_MenuAction			= rhs.m_MenuAction;
		return *this;
	}

	int							getMenuIndex( void )			{ return m_MenuIndex; }
	EFileMenuAction				getMenuAction( void )			{ return m_MenuAction; }

	int							m_MenuIndex;
	EFileMenuAction				m_MenuAction;
};

class FileActionMenu : public ActivityBase
{
	Q_OBJECT
public:
	FileActionMenu( AppCommon&	app, 
					QWidget*		parent, 
					FileInfo&	fileInfo, 
					bool			isShared, 
					bool			isInLibrary, 
					VxNetIdent*	selectedFriend = 0 );
	virtual ~FileActionMenu();

	void						setTitle( QString strTitle );
	void						addMenuItem( int iItemId, QIcon& oIcon, QString strMenuItemText );

private slots:
    void						slotHomeButtonClicked( void ) override;
	void						itemClicked(QListWidgetItem*item);

protected:
	void						setupFileInfo( void );
	void						setupMenuItems( void );
	bool						confirmDeleteFile( bool shredFile );

	//=== vars ===//
	Ui::FileActionMenuClass		ui;
	FileInfo					m_FileInfo;
	bool						m_IsShared;
	bool						m_IsInLibrary;
	VxNetIdent*					m_SelectedFriend;
	int							m_iMenuItemHeight;
	std::vector<FileMenuItemAction>	m_MenuList;
};


