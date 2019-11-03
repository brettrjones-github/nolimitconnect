#pragma once
//============================================================================
// Copyright (C) 2017 Brett R. Jones 
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

#include "AppDefs.h"

#include <QWidget>


class ActivityBase;
class AppCommon;
class HomeWindow;
class QFrame;
class RenderGlWidget;
class AppletLaunchWidget;

class AppletMgr : public QWidget
{
	Q_OBJECT
public:
	AppletMgr( AppCommon& myMpp, QWidget * parent );
	virtual ~AppletMgr(){}

    QFrame *					getAppletFrame( EApplet applet );
    ActivityBase *				launchApplet( EApplet applet );
    ActivityBase *				launchApplet( EApplet applet, QWidget * parent );
	void						activityStateChange( ActivityBase * activity, bool isCreated );

    RenderGlWidget *            getRenderConsumer( void );

protected:
	void						addApplet( ActivityBase * activity );
	void						removeApplet( EApplet applet );
    void						removeApplet( ActivityBase * activity );

	void						bringAppletToFront( ActivityBase * appletDialog );
	ActivityBase *				findAppletDialog( EApplet applet );
    ActivityBase *              findAppletDialog( ActivityBase * activity );

    void                        makeMessengerFullSized( void );


	QWidget *					getActiveWindow( void );

	AppCommon&					m_MyApp;
    QVector<AppletLaunchWidget *>		m_AppletList;
	QVector<ActivityBase *>		m_ActivityList;
};

