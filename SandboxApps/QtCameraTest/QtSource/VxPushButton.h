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

#include "SoundDefs.h"
#include "MyIconsDefs.h"

#include <QPushButton>

class AppCommon;
class VxAppTheme;
class MyIcons;
class QTimer;

class VxPushButton : public QPushButton
{
	Q_OBJECT
public:
	VxPushButton(QWidget *parent=0);
	VxPushButton(const QString &text, QWidget *parent=0);
	virtual ~VxPushButton() = default;

    virtual int					heightForWidth( int width ) const override;
    virtual QSize				sizeHint() const override;
    virtual QSize				getMinimumSizeHint( void ) { return sizeHint(); }
    virtual void				setFixedSize( const QSize & fixedSize );
    virtual void				setFixedSize( int width, int height );
    virtual void				setFixedWidth( int width );
    virtual void				setFixedHeight( int height );
    virtual void				setFixedSizeAbsolute( const QSize & fixedSize );
    virtual void				setFixedSizeAbsolute( int width, int height );
    virtual void				setMinimumSize( const QSize & minSize );
    virtual void				setMinimumSize( int minw, int minh );
    virtual void				setMaximumSize( const QSize & maxSize );
    virtual void				setMaximumSize( int maxw, int maxh );


	AppCommon&					getMyApp( void )                                    { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						setEnabled( bool enabled );
	void						setUseTheme( bool useTheme )						{ m_UseTheme = useTheme; }
	bool						getUseTheme( void )									{ return m_UseTheme; }
	void						setNotifyEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNotifyOverlay );
	bool						getNotifyEnabled( void )							{ return m_NotifyEnabled; }

	void						setIcon( EMyIcons myIcon );
	void						setIconColor( QColor iconColor );
	void						setDrawButtonBorder( bool drawBoarder )				{ m_DrawButtonBorder = drawBoarder; }
	bool						getDrawButtonBorder( void )							{ return m_DrawButtonBorder; }

	void						setIsToggleButton( bool isToggleButton )			{ m_IsToggleButton = isToggleButton; }
	bool						getIsToggleButton( void )							{ return m_IsToggleButton; }
	void						setToggleState( bool stateOn )						{ m_ToggleStateOn = stateOn;  }
	bool						getToggleState( void )								{ return m_ToggleStateOn; }

	void						setIsSlideLeftButton( bool isSlideLeftButton )		{ m_IsSlideLeftButton = isSlideLeftButton; }
	bool						getIsSlideLeftButton( void )						{ return m_IsSlideLeftButton; }

	void						setPressedSound( ESndDef sndDef )					{ m_ESndDefPressed = sndDef; }

	void						setIcons(	EMyIcons	normalIcon, 
											EMyIcons	overlay1Icon		= eMyIconNone );

    virtual void                setAppIcon( EMyIcons appletIcon, QWidget * parentAppFrame );

signals:
	void						buttonToggleState( bool isOn );
	void						slideLeftCompleted( void );
    void                        signalAppIconSpecialClick( void );

protected slots:
	void						slotBlinkTimeout( void );

protected:
	void						initQButtonPro( void );

	virtual void				mousePressEvent( QMouseEvent * event ) override;
	virtual void				mouseReleaseEvent( QMouseEvent * event ) override;
	virtual void				mouseMoveEvent( QMouseEvent * event ) override;
    virtual void				paintEvent( QPaintEvent* ev ) override;
    void                        drawBorder( VxAppTheme& appTheme, QPainter& painter );

	//=== vars ===//
	AppCommon&					m_MyApp;
	QColor						m_DefaultIconColor;
    bool						m_IconColorWasSet{ false };
	bool						m_DrawButtonBorder{ false };
	bool						m_UseTheme{ true };
	bool						m_NotifyEnabled{ false };

	EMyIcons					m_MyIcon;
    EMyIcons					m_MyIconLast;
	QPixmap						m_IconImage;
	QColor						m_LastIconColor;
	QSize						m_LastIconSize;

	EMyIcons					m_NotifyIcon;
	EMyIcons					m_LastNotifyIcon;
	QPixmap						m_NotifyIconImage;
	QColor						m_NotifyLastIconColor;
	QSize						m_NotifyLastIconSize;

	EMyIcons					m_OverlayIcon;
	EMyIcons					m_LastOverlayIcon;
	QPixmap						m_OverlayIconImage;
	QColor						m_OverlayLastIconColor;
	QSize						m_OverlayLastIconSize;

	bool						m_ToggleStateOn{ false };
	bool						m_IsToggleButton{ false };
	bool						m_IsSlideLeftButton{ false };
	bool						m_InSlideLeftMode{ false };
	ESndDef						m_ESndDefPressed;
	QPoint						m_MousePressedPoint;
	QPoint						m_ButtonPressedPos;
	QPoint						m_MouseCurPoint;

	QTimer *					m_BlinkTimer;
    int							m_BlinkState{ 0 };
    QWidget *					m_AppFrame{ nullptr };
    int							m_AppClickCount{ 0 };
    int64_t                     m_AppClickTime{ 0 };
};

