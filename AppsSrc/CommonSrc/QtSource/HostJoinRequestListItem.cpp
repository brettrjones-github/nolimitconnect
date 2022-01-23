//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "AppCommon.h"
#include "HostJoinRequestListItem.h"
#include "GuiHostJoinSession.h"
#include "GuiParams.h"

//============================================================================
HostJoinRequestListItem::HostJoinRequestListItem(QWidget *parent  )
: IdentLogicInterface( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    setupIdentLogic();
    connect( ui.m_AcceptButton,		  SIGNAL( clicked() ),	this, SLOT(slotAcceptButtonPressed()) );
    connect( ui.m_RejectButton,       SIGNAL( clicked() ), this, SLOT( slotRejectButtonPressed() ) );
   
    ui.m_AvatarButton->setFixedSize( eButtonSizeLarge );
    ui.m_AvatarButton->setIcon( eMyIconAvatarImage );
    ui.m_FriendshipButton->setFixedSize( eButtonSizeLarge );
    ui.m_FriendshipButton->setIcon( eMyIconAnonymous );
    ui.m_MenuButton->setFixedSize( eButtonSizeLarge );
    ui.m_MenuButton->setIcon( eMyIconMenu );

    ui.m_AcceptButton->setFixedSize( eButtonSizeTiny );
    ui.m_AcceptButton->setIcon( eMyIconAcceptNormal );
    ui.m_RejectButton->setFixedSize( eButtonSizeTiny );
    ui.m_RejectButton->setIcon( eMyIconCancelNormal );
    QSize sizeHint( 200, GuiParams::getButtonSize( eButtonSizeLarge ).height() + 4 );
    setSizeHint( sizeHint );
    setFixedHeight( sizeHint.height() );
}

//============================================================================
HostJoinRequestListItem::~HostJoinRequestListItem()
{
    GuiHostJoinSession * hostSession = (GuiHostJoinSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
    if( hostSession && !hostSession->parent() )
    {
        delete hostSession;
    }
}

//============================================================================
MyIcons& HostJoinRequestListItem::getMyIcons( void )
{
    return m_MyApp.getMyIcons();
}

//============================================================================
void HostJoinRequestListItem::resizeEvent(QResizeEvent* resizeEvent)
{
    QWidget::resizeEvent(resizeEvent);
    updateWidgetFromInfo();
}

//============================================================================
void HostJoinRequestListItem::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    emit signalHostJoinRequestListItemClicked( this );
}

//============================================================================
void HostJoinRequestListItem::setHostSession( GuiHostJoinSession* hostSession )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)hostSession) );
}

//============================================================================
GuiHostJoinSession * HostJoinRequestListItem::getHostSession( void )
{
    return (GuiHostJoinSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void HostJoinRequestListItem::onIdentAvatarButtonClicked()
{
    LogMsg( LOG_DEBUG, "HostJoinRequestListItem::slotAvatarButtonClicked" );
	emit signalAvatarButtonClicked( this );
}


//============================================================================
void HostJoinRequestListItem::onIdentMenuButtonClicked( void )
{
	emit signalMenuButtonClicked( this );
}

//============================================================================
void HostJoinRequestListItem::updateWidgetFromInfo( void )
{
    GuiHostJoinSession* hostSession = getHostSession();
    if( !hostSession || !hostSession->getGuiUser() )
    {
        LogMsg( LOG_ERROR, "HostJoinRequestListItem::updateWidgetFromInfo null ident" );
        return;
    }

    GuiHostJoin* hostIdent = hostSession->getHostJoin();
    if( hostIdent )
    {
        updateIdentity( hostIdent );
    }

    if( m_MyApp.getEngine().getMyOnlineId() == hostIdent->getUser()->getMyOnlineId() )
    {
        ui.m_TitlePart2->setText( QObject::tr( "Hosted By Me") );
        ui.m_FriendshipButton->setIcon( eMyIconAdministrator );
    }

    if( !ui.m_AvatarButton->hasImage() )
    {
        VxGUID thumbId = hostSession->getHostThumbId();
        if( !thumbId.isVxGUIDValid() )
        {
            thumbId = hostIdent->getUser()->getHostThumbId( hostSession->getHostType(), true );
        }
       
        if( thumbId.isVxGUIDValid() )
        {
            QImage thumbImage;
            if( m_MyApp.getThumbMgr().getThumbImage( thumbId, thumbImage ) )
            {
                ui.m_AvatarButton->setIconOverrideImage( thumbImage );
            }
        }
    }

    // set text of line 2
    std::string strDesc = hostSession->getHostDescription();
    if( strDesc.empty() )
    {
        strDesc = hostIdent->getUser()->getOnlineDescription();
    }

    if( !strDesc.empty() )
    {
        ui.m_DescPart2->setText( strDesc.c_str() );
    }
}

//============================================================================
void HostJoinRequestListItem::setJoinedState( EJoinState joinState )
{
    switch( joinState )
    {
    case eJoinStateJoinAccepted:
        showAcceptButton( false );
        showRejectButton( true );
        break;
    case eJoinStateJoinRequested:
        showAcceptButton( true );
        showRejectButton( true );
        break;
    case eJoinStateJoinDenied:
        showAcceptButton( true );
        showRejectButton( false );
        break;

    case eJoinStateSending:
    case eJoinStateSendFail:
    case eJoinStateSendAcked:
    case eJoinStateNone:
    default:
        showAcceptButton( true );
        showRejectButton( true );
        break;
    }
}

//============================================================================
void HostJoinRequestListItem::showAcceptButton( bool makeVisible )
{
    ui.m_AcceptButton->setVisible( makeVisible );
    ui.m_AcceptLabel->setVisible( makeVisible );
}

//============================================================================
void HostJoinRequestListItem::showRejectButton( bool makeVisible )
{
    ui.m_RejectButton->setVisible( makeVisible );
    ui.m_RejectLabel->setVisible( makeVisible );
}

//============================================================================
void HostJoinRequestListItem::slotAcceptButtonPressed( void )
{
    emit signalAcceptButtonClicked( this );
}

//============================================================================
void HostJoinRequestListItem::slotRejectButtonPressed( void )
{
    emit signalRejectButtonClicked( this );
}
