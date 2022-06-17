#ifndef SOUND_TEST_THREAD_H
#define SOUND_TEST_THREAD_H

#include <QWidget>
#include <QMutex>
#include <QThread>

class SoundTestLogic;
class AudioIoMgr;
class AudioTestGenerator;

class SoundTestThread : public QThread
{
	Q_OBJECT
public:
    SoundTestThread( SoundTestLogic& renderLogic );

    void                        startSoundTestThread();
    void                        stopSoundTestThread();

	void						setThreadShouldRun(bool shouldRun)      { m_ShouldRun = shouldRun; }
    void                        pauseSound( bool pause )                { m_PauseSound = pause; }

protected:
    virtual void                run() override;

    SoundTestLogic&				m_SoundTestLogic;
    AudioIoMgr&                 m_AudioIoMgr;
    AudioTestGenerator *        m_AudioGenOut = nullptr;

	bool						m_ShouldRun = true;
    bool						m_PauseSound = false;
};

#endif // SOUND_TEST_THREAD_H
