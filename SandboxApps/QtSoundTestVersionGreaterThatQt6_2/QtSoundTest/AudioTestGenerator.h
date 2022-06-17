#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

//#include "config_gotvapps.h"
#include <QIODevice>
#include <QtGlobal>
#if QT_VERSION >= 0x050000
    #include <QAudioOutput>
    #include <QAudioFormat>
#else
    #include <QtMultimedia>
#endif


class AudioTestGenerator : public QIODevice
{
    Q_OBJECT

public:
    AudioTestGenerator( const QAudioFormat &format, qint64 durationUs, int sampleRate, QObject *parent );
    ~AudioTestGenerator() = default;

    void                        start();
    void                        stop();

    qint64                      readData( char *data, qint64 maxlen ) override;
    qint64                      readData( QByteArray& retData, qint64 maxlen );
    qint64                      readDataNoPositionUpdate( char *data, qint64 len );
    qint64                      writeData( const char *data, qint64 len ) override;
    qint64                      bytesAvailable() const override;

private:
    void                        generateData( const QAudioFormat &format, qint64 durationUs, int sampleRate );

    qint64                      m_pos;
    QByteArray                  m_buffer;
};

