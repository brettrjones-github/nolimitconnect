#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletBase.h"
#include <GuiInterface/IToGui.h>
#include <PktLib/SearchParams.h>

#include "ui_SearchParamsWidget.h"

#include "ThumbnailChooseWidget.h"

class SearchParamsWidget : public QWidget
{
	Q_OBJECT
public:
    SearchParamsWidget( QWidget * parent );
	virtual ~SearchParamsWidget() = default;

    void						setupSearchParamsWidget( EApplet applet, EPluginType pluginType );

    virtual QLineEdit *         getSearchTextEdit()         { return ui.m_SearchTextEdit; }

    virtual QLabel *            getSearchDescriptionLabel() { return ui.m_SearchDescriptionLabel; }

    virtual QPushButton *       getStartButton()            { return ui.m_StartButton; }
    virtual QPushButton *       getStopButton()             { return ui.m_StopButton; }

    virtual QComboBox *         getSearchTypeComboBox()     { return ui.m_SearchTypeComboBox; }
    virtual QComboBox *         getAgeComboBox()            { return ui.m_AgeComboBox; }
    virtual QComboBox *         getContentRatingComboBox()  { return ui.m_ContentRatingComboBox; }
    virtual QComboBox *         getGenderComboBox()         { return ui.m_GenderComboBox; }
    virtual QComboBox *         getLanguageComboBox()       { return ui.m_LanguageComboBox; }

    virtual bool                toSearchParams( SearchParams& params );
    virtual bool                fromSearchParams( SearchParams& params );

    virtual void                setSearchListAll( bool listAll )    { m_SearchParams.setSearchListAll( listAll ); }
    virtual bool                getSearchListAll( void )            { return m_SearchParams.getSearchListAll(); }

signals:
    void                        signalSearchState(bool searchStarted);

public slots:
    virtual void                slotSearchCancel();
    virtual void                slotSearchComplete();

protected slots:
    virtual void                slotStartSearch();
    virtual void                slotStopSearch();

protected:
    EPluginType                 getPluginType() { return m_PluginType; }

    //=== vars ===//
    Ui::SearchParamsUi          ui;
    AppCommon&                  m_MyApp;
    EApplet                     m_EAppletType{ eAppletUnknown };
    EPluginType                 m_PluginType{ ePluginTypeInvalid };
    AppletBase*                 m_ParentApplet{ nullptr };
    int                         m_SubPluginType{ 0 };
    bool                        m_OrigPermissionIsSet{ false };
    EFriendState                m_OrigPermissionLevel{ eFriendStateIgnore };
    bool                        m_PermissionsConnected{ false };
    SearchParams                m_SearchParams;
    bool                        m_SearchStarted{ false };
};


