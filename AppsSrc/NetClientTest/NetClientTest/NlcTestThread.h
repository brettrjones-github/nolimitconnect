#pragma once

#include "NlcTestCommon.h"
#include "AppLogic.h"

#include <QWidget>
#include <QMutex>

class NlcTestThread : public QThread
{
	Q_OBJECT
public:
    explicit NlcTestThread( AppLogic& appLogic );


	void						setRenderThreadShouldRun(bool shouldRun) { m_ShouldRun = shouldRun; }

protected:
    AppLogic&				    m_AppLogic;
	bool						m_ShouldRun = true;
};

