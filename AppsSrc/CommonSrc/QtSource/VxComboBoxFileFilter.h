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

#include <QComboBox>

#include <CoreLib/VxFileInfo.h>

class VxComboBoxFileFilter : public QComboBox
{
	Q_OBJECT
public:
	VxComboBoxFileFilter(QWidget*parent=0);
	virtual ~VxComboBoxFileFilter();

	void						setFileFilter( EFileFilterType eFileFilter );
	uint8_t						getMaskFromFileFilterType( EFileFilterType eFileFilterType );

	unsigned char				getCurrentFileFilterMask( void );
	EFileFilterType				getCurrentFileFilterType( void );
    EFileFilterType             getFileFilterTypeFromIndex( int selectionIdx );

signals:
	void						signalApplyFileFilter(unsigned char fileTypeMask);
	void						signalFileFilterSelectionChanged( EFileFilterType eFileFilterType );

protected slots:
	void						handleSelectionChanged( int idx );
    int						    getSelectionIndexFromFilterType( EFileFilterType eFileFilter );

private:
	unsigned char				m_CurFileMask;
    EFileFilterType             m_FileFilterType;
};

