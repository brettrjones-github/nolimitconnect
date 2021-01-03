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
// http://www.nolimitconnect.com
//============================================================================

#include <GoTvInterface/IDefs.h>
#include <QProgressBar>

class AppCommon;

class VxProgressBar : public QProgressBar
{
	Q_OBJECT
public:
    VxProgressBar(QWidget *parent=0);

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

    virtual QSize				sizeHint() const override;

protected slots:
	void						slotProgressValue( EAppModule appModule, int peekVal0to32768 );

protected:
	//=== vars ===//
	AppCommon&					m_MyApp;
};

