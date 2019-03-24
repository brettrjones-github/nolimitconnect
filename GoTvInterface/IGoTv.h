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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "config_gotvcore.h"
#include "GoTvInterface/IGoTvDefs.h"
#include "GoTvInterface/OsInterface/OsInterface.h"
#include "GoTvInterface/ILog.h"
#include "GoTvInterface/IToGui.h"
#include <GoTvInterface/IFromGui.h>
#include <GoTvInterface/IGoTvRender.h>
#include <GoTvInterface/IGoTvEvents.h>

#include <string>

class OsInterface;
class CAppParamParser;
class CApplication;
class VxPeerMgr;
class P2PEngine;
class AppCommon;
// media player
class CRenderBuffer;


class IGoTv : public IToGui, public IGoTvRender, public IGoTvEvents
{
public:
    IGoTv();
    virtual  ~IGoTv();

    static IGoTv&               getIGoTv();
    ILog&                       getILog()               { return m_ILog; }
    OsInterface&                getOsInterface()        { return m_OsInterface; }
    VxPeerMgr&                  getPeerMgr()            { return m_VxPeerMgr; }
    P2PEngine&                  getPtoP();
    CApplication&               getKodi()               { return m_Kodi; }
    AppCommon&                  getAppCommon();

    //============================================================================
    //=== starup/shutdown ===//
    //============================================================================

    //=== stages of create ===//
    bool                        doPreStartup();
    bool                        doStartup();

    //=== stages of run ===//
    bool                        initRun( const CAppParamParser& params );
    bool                        doRun( EAppModule appModule );

    //=== stages of destroy ===//
    void                        doPreShutdown();
    void                        doShutdown();

    //=== running states ===//
    void                        setIsAppModuleRunning( EAppModule appModule, bool isRunning )   { m_IsRunning[appModule] = isRunning; }
    bool                        getIsAppModuleRunning( EAppModule appModule )                   { return m_IsRunning[appModule]; }

    //=== interface ===//
    void 				        playGoTvMedia( AssetInfo * assetInfo ) override;

    //============================================================================
    //=== to gotv events ===//
    //============================================================================
    void                        fromGuiKeyPressEvent( int moduleNum, int key, int mod ) override;
    void                        fromGuiKeyReleaseEvent( int moduleNum, int key, int mod ) override;

    void                        fromGuiMousePressEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton ) override;
    void                        fromGuiMouseReleaseEvent( int moduleNum, int mouseXPos, int mouseYPos, int mouseButton ) override;
    void                        fromGuiMouseMoveEvent( int moduleNum, int mouseXPos, int mouseYPos ) override;

	void                        fromGuiResizeBegin( int moduleNum, int winWidth, int winHeight ) override;
	void                        fromGuiResizeEvent( int moduleNum, int winWidth, int winHeight ) override;
	void                        fromGuiResizeEnd( int moduleNum, int winWidth, int winHeight ) override;

    void                        fromGuiCloseEvent( int moduleNum ) override;
    void                        fromGuiVisibleEvent( int moduleNum, bool isVisible ) override;

    //============================================================================
    //=== to gui media/render ===//
    //============================================================================

    //=== textures ===//
    void                        setActiveGlTexture( unsigned int activeTextureNum  /* 0 == GL_TEXTURE0 , 1 == GL_TEXTURE1 etc*/ ) override;

    void                        createTextureObject( CQtTexture * texture ) override;
    void                        destroyTextureObject( CQtTexture * texture ) override;
    bool                        loadToGPU( CQtTexture * texture ) override;
    void                        bindToUnit( CQtTexture * texture, unsigned int unit ) override;

    void                        beginGuiTexture( CGUITextureQt * guiTexture, GoTvColor color ) override;
    void                        drawGuiTexture( CGUITextureQt * textrue, float * x, float * y, float * z, const GoTvRect& texture, const GoTvRect& diffuse, int orientation ) override;
    void                        endGuiTexture( CGUITextureQt * guiTexture ) override;
    void                        drawQuad( const GoTvRect &rect, GoTvColor color, CBaseTexture * texture, const GoTvRect * texCoords ) override;

    bool                        firstBegin( CGUIFontTTFQt * font )  override;
    void                        lastEnd( CGUIFontTTFQt * font ) override;
    CVertexBuffer               createVertexBuffer( CGUIFontTTFQt * font, const std::vector<SVertex>& vertices )  override;
    void                        destroyVertexBuffer( CGUIFontTTFQt * font, CVertexBuffer& vertBuffer ) override;

    virtual void                deleteHardwareTexture( CGUIFontTTFQt * font )  override;
    virtual void                createStaticVertexBuffers( CGUIFontTTFQt * font )   override;
    virtual void                destroyStaticVertexBuffers( CGUIFontTTFQt * font )   override;

    //=== remder ===//
    void                        captureScreen( CScreenshotSurface * screenCaptrue, GoTvRect& captureArea ) override;

    void                        toGuiRenderVideoFrame( int textureIdx, CRenderBuffer* videoBuffer );
    bool                        initRenderSystem() override;
    bool                        destroyRenderSystem() override;
    bool                        resetRenderSystem( int width, int height ) override;

    int                         getMaxTextureSize() override;

    bool                        beginRender() override;
    bool                        endRender() override;
    void                        presentRender( bool rendered, bool videoLayer ) override;
    bool                        clearBuffers( GoTvColor color ) override;
    bool                        isExtSupported( const char* extension )  override;

    void                        setVSync( bool vsync );
    void                        resetVSync() {  }

    void                        setViewPort( const GoTvRect& viewPort ) override;
    void                        getViewPort( GoTvRect& viewPort ) override;

    bool                        scissorsCanEffectClipping() override;
    GoTvRect                    clipRectToScissorRect( const GoTvRect &rect ) override;
    void                        setScissors( const GoTvRect& rect ) override;
    void                        resetScissors() override;

    void                        captureStateBlock() override;
    void                        applyStateBlock() override;

    void                        setCameraPosition( const GoTvPoint& camera, int screenWidth, int screenHeight, float stereoFactor = 0.0f ) override;

    void                        applyHardwareTransform( const TransformMatrix &matrix ) override;
    void                        restoreHardwareTransform() override;
    bool                        supportsStereo( RENDER_STEREO_MODE mode ) const { return false; }

    bool                        testRender() override;

    void                        project( float &x, float &y, float &z ) override;

    //=== shaders ===//
    std::string                 getShaderPath( const std::string &filename ) override { return ""; }

    void                        initialiseShaders() override;
    void                        releaseShaders() override;
    bool                        enableShader( ESHADERMETHOD method ) override;
    bool                        isShaderValid( ESHADERMETHOD method ) override;
    void                        disableShader( ESHADERMETHOD method ) override;
    void                        disableGUIShader() override;

    int                         shaderGetPos( )  override;
    int                         shaderGetCol()  override;
    int                         shaderGetModel()  override;
    int                         shaderGetCoord0( )  override;
    int                         shaderGetCoord1( )  override;
    int                         shaderGetUniCol( )  override;

    // yuv shader
    virtual void                shaderSetField( ESHADERMETHOD shader, int field )   override;
    virtual void                shaderSetWidth( ESHADERMETHOD shader, int w )   override;
    virtual void                shaderSetHeight( ESHADERMETHOD shader, int h )  override;

    virtual void                shaderSetBlack( ESHADERMETHOD shader, float black ) override;
    virtual void                shaderSetContrast( ESHADERMETHOD shader, float contrast ) override;
    virtual void                shaderSetConvertFullColorRange( ESHADERMETHOD shader, bool convertFullRange ) override;

    virtual int                 shaderGetVertexLoc( ESHADERMETHOD shader ) override;
    virtual int                 shaderGetYcoordLoc( ESHADERMETHOD shader ) override;
    virtual int                 shaderGetUcoordLoc( ESHADERMETHOD shader ) override;
    virtual int                 shaderGetVcoordLoc( ESHADERMETHOD shader ) override;

    virtual void                shaderSetMatrices( ESHADERMETHOD shader, const float *p, const float *m ) override;
    virtual void                shaderSetAlpha( ESHADERMETHOD shader, float alpha ) override;

    virtual void                shaderSetFlags( ESHADERMETHOD shader, unsigned int flags ) override;
    virtual void                shaderSetFormat( ESHADERMETHOD shader, EShaderFormat format ) override;
    virtual void                shaderSourceTexture( ESHADERMETHOD shader, int ytex ) override;
    virtual void                shaderSetStepX( ESHADERMETHOD shader, float stepX ) override;
    virtual void                shaderSetStepY( ESHADERMETHOD shader, float stepY )  override;

    // filter shader
    virtual bool                shaderGetTextureFilter( ESHADERMETHOD shader, int& filter ) override;
    virtual int                 shaderGetcoordLoc( ESHADERMETHOD shader ) override;

    // renderqt
    virtual int                 shaderVertexAttribPointer( ESHADERMETHOD shader, unsigned int index, int size, int type, bool normalized, int stride, const void * pointer ) override;
    virtual void                shaderEnableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) override;
    virtual void                shaderDisableVertexAttribArray( ESHADERMETHOD shader, int arrayId ) override;

    // frame buffers
    virtual void                frameBufferGen( int bufCount, unsigned int* fboId ) override;
    virtual void                frameBufferDelete( int bufCount, unsigned int* fboId ) override;
    virtual void                frameBufferTexture2D( int target, unsigned int texureId )  override;
    virtual void                frameBufferBind( unsigned int fboId ) override;
    virtual bool                frameBufferStatus( ) override;

    //============================================================================
    //=== end to gui media/render ===//
    //============================================================================

    //============================================================================
    //=== to gui ===//
    //============================================================================
    void				        toGuiLog( int logFlags, const char * pMsg ) override;
    void				        toGuiAppErr( EAppErr eAppErr, const char* errMsg = "" ) override;
    void				        toGuiStatusMessage( const char* statusMsg ) override;

    virtual void				toGuiWantMicrophoneRecording( bool wantMicInput ) override;
    virtual void				toGuiWantSpeakerOutput( bool wantSpeakerOutput ) override;
    virtual void				toGuiPlayAudio( int16_t * pu16PcmData, int pcmDataLenInBytes ) override;

    virtual int  				toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes ) override;
    virtual double  			toGuiGetAudioDelaySeconds() override;
    virtual double				toGuiGetAudioCacheTotalSeconds() override;
    virtual int				    toGuiGetAudioCacheFreeSpace( EAppModule appModule ) override;

    virtual void				toGuiWantVideoCapture( bool wantVidCapture ) override;
    virtual void				toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgDataLen, int motion0To100000 ) override;
    virtual int				    toGuiPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight ) override;

    virtual void				toGuiNetworkState( ENetworkStateType eNetworkState, const char* stateMsg = "" ) override;
    virtual void				toGuiMyRelayStatus( EMyRelayStatus eRelayStatus, const char * msg = "" ) override;
    virtual void				toGuiAnchorStatus( EAnchorTestStatus eAnchorStatus, const char * msg = "" ) override;
    virtual void				toGuiIsPortOpenStatus( EIsPortOpenStatus eIsPortOpenStatus, const char * msg = "" ) override;
    virtual void				toGuiPhoneShakeStatus( EPhoneShakeStatus ePhoneShakeStatus, const char * msg = "" ) override;

    virtual void				toGuiContactOnline( VxNetIdent * netIdent, bool newContact = false ) override;
    virtual void				toGuiContactOffline( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactNearby( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactNotNearby( VxNetIdent * netIdent ) override;

    //! called when contact changes
    virtual void				toGuiContactNameChange( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactDescChange( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactMyFriendshipChange( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactHisFriendshipChange( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactSearchFlagsChange( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactConnectionChange( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactAnythingChange( VxNetIdent * netIdent ) override;
    virtual void				toGuiContactLastSessionTimeChange( VxNetIdent * netIdent ) override;

    virtual void				toGuiPluginPermissionChange( VxNetIdent * netIdent ) override;

    //! called from engine when need to update identity
    virtual void				toGuiUpdateMyIdent( VxNetIdent * netIdent ) override;
    virtual void				fromGuiSetIdentHasTextOffers( VxGUID& onlineId, bool hasTextOffers );

    //! add offer to notify list
    virtual void				toGuiRxedPluginOffer( VxNetIdent *		netIdent,
                                                      EPluginType		ePluginType,
                                                      const char *	pMsg,
                                                      int				pvUserData,
                                                      const char *	pFileName = NULL,
                                                      uint8_t *			fileHashData = 0,
                                                      VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
                                                      VxGUID&			rmtSessionId = VxGUID::nullVxGUID() ) override;
    //! response to offer
    virtual void				toGuiRxedOfferReply( VxNetIdent *	netIdent,
                                                     EPluginType		ePluginType,
                                                     int				pvUserData,
                                                     EOfferResponse	eOfferResponse,
                                                     const char *	pFileName = NULL,
                                                     uint8_t *			fileHashData = 0,
                                                     VxGUID&			lclSessionId = VxGUID::nullVxGUID(),
                                                     VxGUID&			rmtSessionId = VxGUID::nullVxGUID() ) override;

    virtual void				toGuiPluginSessionEnded( VxNetIdent *	netIdent,
                                                         EPluginType		ePluginType,
                                                         int				pvUserData,
                                                         EOfferResponse	eOfferResponse,
                                                         VxGUID&			lclSessionId = VxGUID::nullVxGUID() ) override;

    virtual void				toGuiPluginStatus( EPluginType		ePluginType,
                                                   int				statusType,
                                                   int				statusValue ) override;

    virtual void				toGuiInstMsg( VxNetIdent *	netIdent,
                                              EPluginType		ePluginType,
                                              const char *	pMsg ) override;

    virtual void				toGuiFileListReply( VxNetIdent *	netIdent,
                                                    EPluginType		ePluginType,
                                                    uint8_t				u8FileType,
                                                    uint64_t				u64FileLen,
                                                    const char *	pFileName,
                                                    uint8_t *			fileHashData ) override;

    virtual void				toGuiStartUpload( VxNetIdent *	netIdent,
                                                  EPluginType		ePluginType,
                                                  VxGUID&			fileInstanceId,
                                                  uint8_t				u8FileType,
                                                  uint64_t				u64FileLen,
                                                  const char *	pFileName,
                                                  uint8_t *			fileHashData ) override;

    virtual void				toGuiStartDownload( VxNetIdent *	netIdent,
                                                    EPluginType		ePluginType,
                                                    VxGUID&			fileInstanceId,
                                                    uint8_t				u8FileType,
                                                    uint64_t				u64FileLen,
                                                    const char *	pFileName,
                                                    uint8_t *			fileHashData ) override;

    virtual void				toGuiFileDownloadComplete( VxGUID& lclSessionId, const char * newFileName, EXferError xferError ) override;
    virtual void				toGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError ) override;

    virtual void				toGuiFileXferState( VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 ) override;

    //=== scan ===//
    virtual void				toGuiScanSearchComplete( EScanType eScanType ) override;
    virtual void				toGuiSearchResultSuccess( EScanType eScanType, VxNetIdent *	netIdent ) override;
    virtual void				toGuiSearchResultError( EScanType eScanType, VxNetIdent * netIdent, int errCode ) override;

    virtual void				toGuiSearchResultProfilePic( VxNetIdent *	netIdent,
                                                             uint8_t *      pu8JpgData,
                                                             uint32_t       u32JpgDataLen ) override;

    virtual void				toGuiSearchResultFileSearch( VxNetIdent *	netIdent,
                                                             VxGUID&        fileInstanceId,
                                                             uint8_t		u8FileType,
                                                             uint64_t       u64FileLen,
                                                             const char *	pFileName ) override;

    virtual bool				toGuiSetGameValueVar( EPluginType	ePluginType,
                                                      VxGUID&	oOnlineId,
                                                      int32_t			s32VarId,
                                                      int32_t			s32VarValue ) override;

    virtual bool				toGuiSetGameActionVar( EPluginType	ePluginType,
                                                       VxGUID&	oOnlineId,
                                                       int32_t			s32VarId,
                                                       int32_t			s32VarValue ) override;

    virtual void				toGuiFileList( const char *	fileName,
                                               uint64_t				fileLen,
                                               uint8_t				fileType,
                                               bool			isShared,
                                               bool			isInLibrary,
                                               uint8_t *			fileHashId = 0 ) override;

    virtual void				toGuiAssetAdded( AssetInfo * assetInfo ) override;
    virtual void				toGuiSessionHistory( AssetInfo * assetInfo ) override;
    virtual void				toGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 ) override;
    virtual void				toGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 ) override;

	/// a module has changed state
	virtual void				toGuiModuleState( int moduleNum, int moduleState )  override;

    //============================================================================
    //=== from gui ===//
    //============================================================================
    virtual void				fromGuiSetupContext( void );

    virtual void				fromGuiSetIsAppCommercial( bool isCommercial );
    virtual bool				fromGuiGetIsAppCommercial( void );
    virtual uint16_t			fromGuiGetAppVersionBinary( void );
    const char *				fromGuiGetAppVersionString( void );
    virtual const char *		fromGuiGetAppName( void );
    virtual const char *		fromGuiGetAppNameNoSpaces( void );

    virtual void				fromGuiAppStartup( const char * assetDir, const char * rootDataDir );
    virtual void				fromGuiKickWatchdog( void );

    virtual void				fromGuiSetUserXferDir( const char * userDir );
    virtual void				fromGuiSetUserSpecificDir( const char * userDir );
    virtual uint64_t			fromGuiGetDiskFreeSpace( void );
    virtual void				fromGuiAppShutdown( void );

    virtual void				fromGuiAppIdle( void );
    virtual void				fromGuiAppPauseOrResume( bool isPaused );

    virtual bool				fromGuiOrientationEvent( float f32RotX, float f32RotY, float f32RotZ );
    virtual bool				fromGuiMouseEvent( EMouseButtonType eMouseButType, EMouseEventType eMouseEventType, int iMouseXPos, int iMouseYPos );
    virtual bool				fromGuiMouseWheel( float f32MouseWheelDist );
    virtual bool				fromGuiKeyEvent( EKeyEventType eKeyEventType, EKeyCode eKey, int iFlags = 0 );

    virtual void				fromGuiNativeGlInit( void );
    virtual void				fromGuiNativeGlResize( int width, int height );
    virtual int					fromGuiNativeGlRender( void );
    virtual void				fromGuiNativeGlPauseRender( void );
    virtual void				fromGuiNativeGlResumeRender( void );
    virtual void				fromGuiNativeGlDestroy( void );

    virtual void				fromGuiNeedMorePlayData( int16_t * retAudioSamples,
                                                         int deviceReqDataLen );
    virtual void				fromGuiMicrophoneData( int16_t * pcmData, int pcmDataLenInBytes );
    virtual void				fromGuiMuteMicrophone( bool mute );
    virtual void				fromGuiMuteSpeaker( bool mute );
    virtual void				fromGuiEchoCancelEnable( bool enableEchoCancel );
    virtual bool				fromGuiIsMicrophoneMuted( void );
    virtual bool				fromGuiIsSpeakerMuted( void );
    virtual bool				fromGuiIsEchoCancelEnabled( void );

    virtual void				fromGuiWantMediaInput( VxGUID& id, EMediaInputType eMediaType, bool bWantInput );
    virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t * pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation );
    virtual bool				fromGuiMovieDone( void );

    virtual void				fromGuiUserLoggedOn( VxNetIdent * netIdent );
    virtual void				fromGuiOnlineNameChanged( const char * newOnlineName );
    virtual void				fromGuiMoodMessageChanged( const char * newMoodMessage );
    virtual void				fromGuiSetUserHasProfilePicture( bool haveProfilePick );
    virtual void				fromGuiUpdateMyIdent( VxNetIdent * netIdent, bool permissionAndStatsOnly );
    virtual void				fromGuiQueryMyIdent( VxNetIdent * poRetIdent );

    virtual void				fromGuiGetNetSettings( NetSettings& netSettings );
    virtual void				fromGuiSetNetSettings( NetSettings& netSettings );
    virtual void				fromGuiNetworkSettingsChanged( void );

    virtual void				fromGuiNetworkAvailable( const char * lclIp = NULL, bool isCellularNetwork = false );
    virtual void				fromGuiNetworkLost( void );

    virtual void				fromGuiVerifyAnchorSettings( void );
    virtual void				fromGuiRunIsPortOpenTest( uint16_t port );

    virtual void				fromGuiGetFileShareSettings( FileShareSettings& fileShareSettings );
    virtual void				fromGuiSetFileShareSettings( FileShareSettings& fileShareSettings );

    virtual void				fromGuiUpdateWebPageProfile( const char *	pProfileDir,	// directory containing user profile
                                                             const char *	strGreeting,	// greeting text
                                                             const char *	aboutMe,		// about me text
                                                             const char *	url1,			// favorite url 1
                                                             const char *	url2,			// favorite url 2
                                                             const char *	url3 );			// favorite url 3

    virtual void 				fromGuiSetPluginPermission( EPluginType ePluginType, int eFriendState );
    virtual int					fromGuiGetPluginPermission( EPluginType ePluginType );
    virtual int					fromGuiGetPluginServerState( EPluginType ePluginType );

    virtual void				fromGuiStartPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
    virtual void				fromGuiStopPluginSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
    virtual bool				fromGuiIsPluginInSession( EPluginType ePluginType, VxGUID& oOnlineId, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

    virtual bool				fromGuiMakePluginOffer( EPluginType		ePluginType,
                                                        VxGUID&			oOnlineId,
                                                        int				pvUserData,
                                                        const char *	pOfferMsg,
                                                        const char *	pFileName,
                                                        VxGUID&			lclSessionId );

    virtual bool				fromGuiToPluginOfferReply( EPluginType		ePluginType,
                                                           VxGUID&			oOnlineId,
                                                           int				pvUserData,
                                                           int				iOfferResponse,
                                                           VxGUID&			lclSessionId );

    virtual int					fromGuiPluginControl( EPluginType		ePluginType,
                                                      VxGUID&			oOnlineId,
                                                      const char *	pControl,
                                                      const char *	pAction,
                                                      uint32_t				u32ActionData,
                                                      VxGUID&			lclSessionId,
                                                      uint8_t *			fileHashData );

    virtual bool				fromGuiInstMsg( EPluginType		ePluginType,
                                                VxGUID&			oOnlineId,
                                                const char *	pMsg );

    virtual bool				fromGuiChangeMyFriendshipToHim( VxGUID&			oOnlineId,
                                                                EFriendState	eMyFriendshipToHim,
                                                                EFriendState	eHisFriendshipToMe );
    virtual void				fromGuiSendContactList( EFriendViewType eFriendView, int maxContactsToSend );
    virtual void				fromGuiRefreshContactList( int maxContactsToSend );

    virtual void				fromGuiRequireRelay( bool bRequireRelay );
    virtual void				fromGuiUseRelay( VxGUID& oOnlineId, bool bUseAsRelay = true );
    virtual void				fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount );

    virtual void				fromGuiStartScan( EScanType eScanType, uint8_t searchFlags, uint8_t fileTypeFlags, const char * pSearchPattern = "" );
    virtual void				fromGuiNextScan( EScanType eScanType );
    virtual void				fromGuiStopScan( EScanType eScanType );

    virtual InetAddress			fromGuiGetMyIPv4Address( void );
    virtual InetAddress			fromGuiGetMyIPv6Address( void );

    virtual void				fromGuiUserModifiedStoryboard( void );

    virtual void				fromGuiCancelDownload( VxGUID&			fileInstanceId );
    virtual void				fromGuiCancelUpload( VxGUID&			fileInstanceId );

    virtual bool				fromGuiSetGameValueVar( EPluginType	ePluginType,
                                                        VxGUID&		oOnlineId,
                                                        int32_t			s32VarId,
                                                        int32_t			s32VarValue );

    virtual bool				fromGuiSetGameActionVar( EPluginType	ePluginType,
                                                         VxGUID&		oOnlineId,
                                                         int32_t			s32VarId,
                                                         int32_t			s32VarValue );

    virtual bool				fromGuiTestCmd( IFromGui::ETestParam1		eTestParam1,
                                                int							testParam2 = 0,
                                                const char *				testParam3 = NULL );
    virtual void				fromGuiMulitcastPkt( uint8_t * pktData, int dataLen );
    virtual void				fromGuiDebugSettings( uint32_t u32LogFlags, const char *	pLogFileName = NULL );
    virtual void				fromGuiSendLog( uint32_t u32LogFlags );

    virtual bool				fromGuiBrowseFiles( const char * dir, bool lookupShareStatus, uint8_t fileFilterMask = VXFILE_TYPE_ALLNOTEXE | VXFILE_TYPE_DIRECTORY );
    virtual bool				fromGuiGetSharedFiles( uint8_t fileTypeFilter );
    virtual bool				fromGuiSetFileIsShared( const char * fileName, bool isShared );
    virtual bool				fromGuiGetIsFileShared( const char * fileName );

    virtual int					fromGuiGetFileDownloadState( uint8_t * fileHashId );
    virtual bool				fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashId = 0 );
    virtual void				fromGuiGetFileLibraryList( uint8_t fileTypeFilter );
    virtual bool				fromGuiGetIsFileInLibrary( const char * fileName );
    virtual bool				fromGuiIsMyP2PWebVideoFile( const char * fileName );
    virtual bool				fromGuiIsMyP2PWebAudioFile( const char * fileName );

    virtual int					fromGuiDeleteFile( const char * fileName, bool shredFile );

    virtual bool				fromGuiAssetAction( EAssetAction assetAction, AssetInfo& assetInfo, int pos0to100000 = 0 );
    virtual bool				fromGuiAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 = 0 );
    virtual bool				fromGuiSendAsset( AssetInfo& assetInfo );

    virtual bool				fromGuiVideoRecord( EVideoRecordState eRecState, VxGUID& feedId, const char * fileName );
    virtual bool				fromGuiSndRecord( ESndRecordState eRecState, VxGUID& feedId, const char * fileName );

    virtual void				fromGuiQuerySessionHistory( VxGUID& historyId );
    virtual bool				fromGuiMultiSessionAction( EMSessionAction mSessionAction, VxGUID& onlineId, int pos0to100000 );

  
    //============================================================================
    //=== utilities ===//
    //============================================================================

    //=== utilities ===//
    bool                        initDirectories();
    void                        createUserDirs() const;

    //=== ffmpeg ===//
    void                        startupFfmpeg();
    void                        shutdownFfmpeg( );

    //=== fopen ssl ===//
    void                        setSslCertFile( std::string certFile );
    std::string                 getSslCertFile( ) { return m_SslCertFile; }


private:
    ILog                        m_ILog; // must be first in initializer list
    OsInterface&                m_OsInterface;
    VxPeerMgr&	                m_VxPeerMgr;
    CApplication&               m_Kodi;   

    bool                        m_IsRunning[ eMaxAppModule ];

    std::string                 m_SslCertFile;
};

// convenience defines
#define GetIGoTv()              IGoTv::getIGoTv()
#define GetILog()               IGoTv::getIGoTv().getILog()
#define GetOsInterface()        IGoTv::getIGoTv().getOsInterface()
#define GetPeerMgr()            IGoTv::getIGoTv().getPeerMgr()
#define GetPtoP()               IGoTv::getIGoTv().getPtoP()
#define GetKodi()               IGoTv::getIGoTv().getKodi()
