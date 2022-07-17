#pragma once

class AppCommon;

class EventsQtToNlc
{
public:
    EventsQtToNlc(  AppCommon& myApp );
    virtual ~EventsQtToNlc() = default;

    bool                        fromGuiKeyPressEvent( int key );
    bool                        fromGuiKeyReleaseEvent( int key );

    bool                        fromGuiMousePressEvent( int mouseXPos, int mouseYPos, int mouseButton );
    bool                        fromGuiMouseReleaseEvent( int mouseXPos, int mouseYPos, int mouseButton );
    bool                        fromGuiMouseMoveEvent( int mouseXPos, int mouseYPos );

	void                        fromGuiResizeBegin( int winWidth, int winHeight );
	void                        fromGuiResizeEvent( int winWidth, int winHeight );
	void                        fromGuiResizeEnd( int winWidth, int winHeight );

    void                        fromGuiCloseEvent( );
    void                        fromGuiVisibleEvent( bool isVisible );

    AppCommon&				    m_MyApp;
};
