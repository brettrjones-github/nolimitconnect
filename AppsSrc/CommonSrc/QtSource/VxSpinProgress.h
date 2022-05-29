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

#include <QLabel>

class QTimer;

class VxSpinProgress : public QLabel 
{
	Q_OBJECT
public:
	VxSpinProgress(QWidget *parent=0, Qt::WindowFlags f=Qt::Widget);
	VxSpinProgress(const QString &text, QWidget *parent=0, Qt::WindowFlags f=Qt::Widget);
	virtual ~VxSpinProgress() = default;

	QSize						sizeHint() const;
	void						setValue( int progressPercent );
	void						setImage( int width, int height, QString resourceUrl );

signals:
	void						clicked();

protected slots:
	void						slotAnimationTickTock( void );

protected:
	virtual void				mousePressEvent(QMouseEvent * event);
	void						hideEvent( QHideEvent * hideEvent );
	void						showEvent( QShowEvent * showEvent );

	void						initQSpinProgress( void );
	void						updateImage( void );

	QTimer *					m_AnimationTimer;
	QImage						m_ProgressPixmap;
	int							m_Angle;
	QString						m_PercentText;
};

