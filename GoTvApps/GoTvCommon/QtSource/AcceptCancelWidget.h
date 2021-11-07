#pragma once
#include "ui_AcceptCancelWidget.h"

#include <QFrame>

class AcceptCancelWidget : public QFrame
{
	Q_OBJECT
public:
	AcceptCancelWidget( QWidget *parent = 0 );
	virtual ~AcceptCancelWidget() {};

	void							setAcceptCancelWidgetSize( EButtonSize buttonSize );

signals:
	void							signalAccepted( void );
	void							signalCanceled( void );

protected slots:
	void							slotAccepted( void );
	void							slotCanceled( void );

protected:


	Ui::AcceptCancelWidgetClass		ui;
};

