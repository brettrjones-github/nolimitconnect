#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value 53 is outside the bounds

#include <CoreLib/VxGUID.h>

class AppCommon;
class AssetBaseInfo;
class GuiPlayerCallback;

class GuiPlayerMgr : public QObject
{
	Q_OBJECT;
public:
	GuiPlayerMgr() = delete;
	GuiPlayerMgr( AppCommon& app );
	GuiPlayerMgr( const GuiPlayerMgr& rhs ) = delete;
	virtual ~GuiPlayerMgr() = default;

	void						playerMgrStartup( void );

	bool						playFile( QString fullFileName, int pos0to100000 = 0 );
	bool						playMedia( AssetBaseInfo& assetInfo, int pos0to100000 = 0 );

	void                        wantPlayVideoCallbacks( GuiPlayerCallback* client, bool enable );

	void						toGuiPlayVideoFrame( VxGUID& feedOnlineId, uint8_t* pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 );
	virtual int				    toGuiPlayVideoFrame( VxGUID& feedOnlineId, uint8_t* picBuf, uint32_t picBufLen, int picWidth, int picHeight );

signals:
	void						signalInternalPlayVideoFrame( VxGUID feedOnlineId, QImage vidFrame );
	void						signalInternalPlayMotionVideoFrame( VxGUID feedOnlineId, QImage vidFrame, int motion0To100000 );

protected slots:
	void						slotInternalPlayVideoFrame( VxGUID feedOnlineId, QImage vidFrame );
	void						slotInternalPlayMotionVideoFrame( VxGUID feedOnlineId, QImage vidFrame, int motion0To100000 );

protected:
	AppCommon&					m_MyApp;

	std::vector<GuiPlayerCallback*>  m_VideoPlayClients;
	QAtomicInt					m_BehindFrameCnt;
};
