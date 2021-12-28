#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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
// http://www.nolimitconnect.com
//============================================================================

#include "MyIconsDefs.h"

#include <GuiInterface/IDefs.h>
#include <QIcon>


class AppCommon;
class QIcon;
class QPainter;

//! stores all icons
class MyIcons
{
public:
	MyIcons( AppCommon& appCommon );
	~MyIcons() = default;

	AppCommon&					getMyApp( void )			{ return m_MyApp; }
	//! load all icons
	void						myIconsStartup( void );

	//! get preloaded icon
	QIcon&						getIcon( EMyIcons eMyIcon );
	//! get pixmap of given icon, size and color
	QPixmap						getIconPixmap( EMyIcons myIcon, QSize iconSize, QColor& iconColor );
	//! get pixmap of given icon and size but do not colorize
	QPixmap						getIconPixmap( EMyIcons myIcon, QSize iconSize );
	//! get icon for give friendship
	EMyIcons					getFriendshipIcon( EFriendState eFriendship );
    //! get setting icon for given plugin
    EMyIcons					getPluginSettingsIcon( EPluginType ePluginType );
	//! get icon for given plugin
	EMyIcons					getPluginIcon( EPluginType ePluginType, EPluginAccess ePluginAccess = ePluginAccessOk );
    //! get setup icon for given plugin
    EMyIcons                    getPluginSetupIcon( EPluginType ePluginType );
	//! get locked plugin icon
	EMyIcons					getLockedPluginIcon( EPluginType ePluginType );
	//! get disabled plugin icon
	EMyIcons					getDisabledPluginIcon( EPluginType ePluginType );
	//! get disabled plugin icon
	EMyIcons					getIgnoredPluginIcon( EPluginType ePluginType );
	//! get icon of given file type
	EMyIcons					getFileIcon( uint8_t u8FileType );

	//! get path to icon file
	QString						getIconFile( EMyIcons eMyIcon );
	bool						isSvgFile( QString& iconFileName );

    //! draw a icon with given color
    void                        drawIcon( EMyIcons eIcon, QPainter* painter, const QRect& rect, QColor& iconColor );

	std::vector< QIcon >		m_aoIcons;

private:
	AppCommon&					m_MyApp;
};

