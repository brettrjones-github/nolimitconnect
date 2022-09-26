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
// http://www.nolimitconnect.org
//============================================================================

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppDefs.h"

#include <GuiInterface/IDefs.h>
#include <CoreLib/VxGUID.h>

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
    ActivityBase *				launchApplet( EApplet applet, QWidget * parent = nullptr, QString launchParam = "", VxGUID assetId = VxGUID::nullVxGUID());
	void						activityStateChange( ActivityBase * activity, bool isCreated );

	ActivityBase*				findAppletDialog( EApplet applet );

    RenderGlWidget *            getRenderConsumer( void );

protected:
	void						addApplet( ActivityBase * activity );
	void						removeApplet( EApplet applet );
    void						removeApplet( ActivityBase * activity );

	void						bringAppletToFront( ActivityBase * appletDialog );
    ActivityBase *              findAppletDialog( ActivityBase * activity );

    void                        makeMessengerFullSized( void );

	QWidget *					getActiveWindow( void );

	bool						viewMyServerAllowed( EApplet applet );
	bool						launchAppletAllowed( EApplet applet );
	bool						isServiceEnabled( EPluginType pluginType );

	AppCommon&					m_MyApp;
    QVector<AppletLaunchWidget *>		m_AppletList;
	QVector<ActivityBase *>		m_ActivityList;
};

