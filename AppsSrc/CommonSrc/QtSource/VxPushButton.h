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
#include "GuiParams.h"

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
    virtual void				setFixedSize( EButtonSize buttonSize );
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

	void						setUseTheme( bool useTheme )						{ m_UseTheme = useTheme; }
	bool						getUseTheme( void )									{ return m_UseTheme; }

    void						setNotifyJoinEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNotifyJoinOverlay );
    bool						getNotifyJoinEnabled( void )						{ return m_NotifyJoinEnabled; }

	void						setNotifyOnlineEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNotifyOnlineOverlay );
    bool						getNotifyOnlineEnabled( void )                      { return m_NotifyOnlineEnabled; }
    void						setNotifyOnlineColor( QColor onlineColor )          { m_NotifyIconOnlineColor = onlineColor; }
    QColor						getNotifyOnlineColor( void )                        { return m_NotifyIconOnlineColor; }
    void                        setNotifyOnlineVisible( bool visible );

    void						setNotifyDirectConnectEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNotifyDirectConnectOverlay );
    bool						getNotifyDirectConnectEnabled( void )               { return m_NotifyDirectConnectEnabled; }
    void						setNotifyDirectConnectColor( QColor onlineColor )   { m_NotifyIconnDirectConnectColor = onlineColor; }
    QColor						getNotifyDirectConnectColor( void )                 { return m_NotifyIconnDirectConnectColor; }

    void						setNotifyInGroupEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNotifyInGroupOverlay );
    bool						getNotifyInGroupEnabled( void )                     { return m_NotifyInGroupEnabled; }
    void						setNotifyOfferEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNotifyOfferOverlay );
    bool						getNotifyOfferEnabled( void )						{ return m_NotifyOfferEnabled; }
    void						setNotifyForbiddenEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNotifyForbbidenOverlay );
    bool						getNotifyForbiddenEnabled( void )					{ return m_NotifyOfferEnabled; }
    void						setNotifyNlcFavoriteEnabled( bool enabled, EMyIcons eNotifyIcon = eMyIconNlcFavoriteOverlay );
    bool						getNotifyNlcFavoritenabled( void )					{ return m_NotifyNlcFavoriteEnabled; }

	void						setIcon( EMyIcons myIcon );
	void						setIconOverrideColor( QColor iconColor );
    void						setIconOverrideImage( QImage& iconImage );
    void                        clearIconOverrideImage( void );
    bool                        hasImage( void )                                    { return !m_IconOverrideImage.isNull(); }

    void						setOverlayIcon( EMyIcons myIcon );
    void						setOverlayColor( QColor iconColor );

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

    void                        setSquareButtonSize( EButtonSize buttonSize );
    EButtonSize                 getSquareButtonSize( void )                         { return m_SquareButtonSize;  }

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
	QColor						m_IconOverrideColor;
    bool						m_IconOverrideColorWasSet{ false };

    QImage						m_IconOverrideImage;
    bool						m_IconOverrideImageWasSet{ false };
    bool                        m_IconOverrideImageWasDrawn{ false };
    QSize						m_LastOverrideImageSize;

	bool						m_DrawButtonBorder{ false };
	bool						m_UseTheme{ true };

	EMyIcons					m_MyIcon;
    EMyIcons					m_MyIconLast;
	QPixmap						m_IconImage;
	QColor						m_LastIconColor;
	QSize						m_LastIconSize;

    bool						m_NotifyOnlineEnabled{ false };
    bool                        m_NotifyOnlineVisible{ true };
	EMyIcons					m_NotifyOnlineIcon{ eMyIconNone };
	EMyIcons					m_LastNotifyOnlineIcon{ eMyIconNone };
	QPixmap						m_NotifyIconOnlineImage;
    QColor						m_NotifyIconOnlineColor;
	QColor						m_NotifyLastIconOnlineColor;
	QSize						m_NotifyLastIconOnlineSize{ 0, 0 };

    bool						m_NotifyDirectConnectEnabled{ false };
    EMyIcons					m_NotifyDirectConnectIcon{ eMyIconNone };
    EMyIcons					m_LastNotifyDirectConnectIcon{ eMyIconNone };
    QPixmap						m_NotifyIconDirectConnectImage;
    QColor						m_NotifyIconnDirectConnectColor;
    QColor						m_NotifyLastIconDirectConnectColor;
    QSize						m_NotifyLastIconDirectConnectSize{ 0, 0 };

    bool						m_NotifyInGroupEnabled{ false };
    EMyIcons					m_NotifyInGroupIcon{ eMyIconNone };
    EMyIcons					m_LastNotifyInGroupIcon{ eMyIconNone };
    QPixmap						m_NotifyIconInGroupImage;
    QColor						m_NotifyLastIconInGroupColor;
    QSize						m_NotifyLastIconInGroupSize{ 0, 0 };

    bool						m_NotifyJoinEnabled{ false };
    EMyIcons					m_NotifyJoinIcon{ eMyIconNone };
    EMyIcons					m_LastNotifyJoinIcon{ eMyIconNone };
    QPixmap						m_NotifyIconJoinImage;
    QColor						m_NotifyLastIconJoinColor;
    QSize						m_NotifyLastIconJoinSize{ 0, 0 };

    bool						m_NotifyOfferEnabled{ false };
    EMyIcons					m_NotifyOfferIcon{ eMyIconNone };
    EMyIcons					m_LastNotifyOfferIcon{ eMyIconNone };
    QPixmap						m_NotifyIconOfferImage;
    QColor						m_NotifyLastIconOfferColor;
    QSize						m_NotifyLastIconOfferSize{ 0, 0 };

    bool						m_NotifyForbiddenEnabled{ false };
    EMyIcons					m_NotifyForbiddenIcon{ eMyIconNone };
    EMyIcons					m_LastNotifyForbiddenIcon{ eMyIconNone };
    QPixmap						m_NotifyIconForbiddenImage;
    QColor						m_NotifyLastIconForbiddenColor;
    QSize						m_NotifyLastIconForbiddenSize;

    bool						m_NotifyNlcFavoriteEnabled{ false };
    EMyIcons					m_NlcFavoriteIcon{ eMyIconNone };
    EMyIcons					m_LastNlcFavoriteIcon{ eMyIconNone };
    QPixmap						m_NlcFavoriteImage;
    QSize						m_LastIconNlcFavoriteSize;

	EMyIcons					m_OverlayIcon{ eMyIconNone };
	EMyIcons					m_LastOverlayIcon{ eMyIconNone };
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
    EButtonSize                 m_SquareButtonSize{ eButtonSizeSmall };
};

