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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "VxLabel.h"

class AssetInfo;

class VxTagLabel : public VxLabel
{
	Q_OBJECT
public:
	VxTagLabel(QWidget *parent=0, Qt::WindowFlags f=0);
	VxTagLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
	virtual ~VxTagLabel();

	void						setAssetInfo( AssetInfo * assetInfo )			{ m_AssetInfo = assetInfo; }
	AssetInfo *					getAssetInfo( void )							{ return m_AssetInfo; }

protected slots:
	void						slotOnClicked( void );


protected:
	void						setupQTagLabel( void );

	AssetInfo *					m_AssetInfo;
};

