//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
// http://www.gotvptop.com
//============================================================================

#include "ActivityChooseTestWebsiteUrl.h"
#include "AppCommon.h"

#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>

#include <QMessageBox>

//============================================================================
ActivityChooseTestWebsiteUrl::ActivityChooseTestWebsiteUrl(	AppCommon& app, QWidget * parent )
: ActivityBase(  OBJNAME_ACTIVITY_CHOOSE_TEST_WEBSITE_URL, app, parent )
, m_MyApp( app )
, m_Engine( app.getEngine() )
{
	ui.setupUi(this);

	connect( ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(reject()) );
	connect( ui.CancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
	connect( ui.OkButton, SIGNAL(clicked()), this, SLOT(applyResultsAndExit()) );

	updateValues();
}

//============================================================================
ActivityChooseTestWebsiteUrl::~ActivityChooseTestWebsiteUrl()
{
}

//============================================================================
void ActivityChooseTestWebsiteUrl::updateValues()
{
	std::string strWebsiteUrl;
	m_Engine.getEngineSettings().getNetServiceWebsiteUrl(strWebsiteUrl);
	QString strUrl = strWebsiteUrl.c_str();
	ui.TestWebsiteEdit->setText(strUrl);
}

//============================================================================
void ActivityChooseTestWebsiteUrl::applyResultsAndExit()
{
	QString strResult = ui.TestWebsiteEdit->text();
	std::string strWebsiteUrl = strResult.toUtf8().constData();
	m_Engine.getEngineSettings().setNetServiceWebsiteUrl(strWebsiteUrl);
	accept();
}

