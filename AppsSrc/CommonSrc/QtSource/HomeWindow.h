#pragma once

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppDefs.h"

#include "ui_HomeWindow.h"
#include <QDialog>
#include <QQueue>

class AppCommon;
class VxAppDisplay;
class AppletLaunchPage;
class PageMediaPlayer;
class MessengerPage;
class AppletMultiMessenger;
class VxFrame;
class AppletBase;

class QDialogButtonBox;
class QGroupBox;
class QComboBox;
class QSettings;

class HomeWindow : public QDialog 
{
	Q_OBJECT
public:
	HomeWindow( AppCommon&	appCommon, QString title );
    virtual ~HomeWindow() override = default;

	AppCommon&					getMyApp( void ) { return m_MyApp; }
	MyIcons&					getMyIcons( void );

	void						initializeHomePage( void );
    void                        restoreHomeWindowGeometry( void );
    void                        saveHomeWindowGeometry( void );

	void						launchApplet( EApplet applet );
	void						switchWindowFocus( QWidget * appIconButton );

    void						setIsMaxScreenSize( bool isMessagerFrame, bool isFullSizeWindow );
    bool						getIsMaxScreenSize( bool isMessagerFrame );

	QFrame *					getAppletFrame( EApplet applet );
	QFrame *					getMessengerParentFrame( void );
    QFrame *					getLaunchPageFrame( void );

    AppletLaunchPage *          getLaunchPage( void ) { return m_AppletLaunchPage; }

signals:
	void						signalMainWindowResized( void );
    void						signalMainWindowMoved( void );

	void						signalDeviceOrientationChanged( int qtOrientation ); // pass on signal that orientation has changed

private slots:
	void						slotDeviceOrientationChanged( int qtOrientation ); // from m_AppDisplay
	void						help( void );
    void						slotHomeButtonClicked( void );
	void						slotFrameResized( void );
    void                        accept() override;
    void                        reject() override;

protected:
    void                        showEvent( QShowEvent * ev ) override;
    void						resizeEvent( QResizeEvent * ) override;
    void						moveEvent( QMoveEvent * ) override;
    void						closeEvent( QCloseEvent * ) override;

	void						initializeNlcDynamicLayout( void );
	void						createAppletLaunchPage( void );
	void						createMessengerPage( void );
    void                        updateAndroidGeomety();
    AppletBase *                findActiveApplet( void );

	//=== vars ===//
	Ui::HomeWindowClass			ui;
	AppCommon&					m_MyApp;
    VxAppDisplay&               m_AppDisplay;
	QString						m_AppTitle;
    QSettings *                 m_WindowSettings{nullptr};

    Qt::Orientation				m_Orientation;
	EHomeLayout					m_LastHomeLayout;
    QGridLayout *				m_MainLayout{nullptr};
    VxFrame *					m_HomeFrameUpperLeft{nullptr};
    VxFrame *					m_HomeFrameRight{nullptr};
    VxFrame *					m_HomeFrameBottom{nullptr};

    PageMediaPlayer *			m_MediaPlayerPage{nullptr};
    AppletLaunchPage *			m_AppletLaunchPage{nullptr};
    QWidget *					m_MessengerParent{nullptr};
    MessengerPage *				m_MessengerPage{nullptr};
    AppletMultiMessenger*       m_AppletMultiMessenger{ nullptr };

    bool						m_MessengerIsFullSize{false};
    bool						m_HomeFrameFullSize{false};

	//=== demo mode vars ===//
    QGroupBox *					rotableGroupBox{nullptr};
    QQueue<QWidget *>			rotableWidgets;

    QGroupBox *					optionsGroupBox{nullptr};
    QLabel *					buttonsOrientationLabel{nullptr};
    QComboBox *					buttonsOrientationComboBox{nullptr};

    QDialogButtonBox *			buttonBox{nullptr};
    QPushButton *				closeButton{nullptr};
    QPushButton *				helpButton{nullptr};
    QPushButton *				rotateWidgetsButton{nullptr};

    QGridLayout *				rotableLayout{nullptr};
    QGridLayout *				optionsLayout{nullptr};
    bool                        m_EngineInitialized{false};
};

