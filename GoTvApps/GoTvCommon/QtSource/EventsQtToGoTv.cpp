#include <app_precompiled_hdr.h>
#include "EventsQtToGoTv.h"
#include "AppCommon.h"


#include "input/XBMC_keysym.h"
#include "input/XBMC_vkeys.h"
#include "input/mouse/MouseStat.h"

#include "settings/AdvancedSettings.h"
#include "ServiceBroker.h"
#include "settings/SettingsComponent.h"
#include "GoTvCoreUtil.h"

#ifndef TARGET_OS_WINDOWS
// defines that are normally in "WinUser.h"
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#if(_WIN32_WINNT >= 0x0500)
#define VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
#endif /* _WIN32_WINNT >= 0x0500 */


#define VK_BACK           0x08
#define VK_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F

#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87


#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5


#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC

#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7

#endif // TARGET_OS_WINDOWS

//============================================================================
namespace KODI
{
    namespace WINDOWING
    {
        namespace WINDOWS
        {
            static XBMCKey VK_keymap[ XBMCK_LAST ];

            static void DIB_InitOSKeymap()
            {
                /* Map the VK keysyms */
                for( int i = 0; i < ARRAY_SIZE( VK_keymap ); ++i )
                {
                    VK_keymap[ i ] = XBMCK_UNKNOWN;
                }

                VK_keymap[ VK_BACK ] = XBMCK_BACKSPACE;
                VK_keymap[ VK_TAB ] = XBMCK_TAB;
                VK_keymap[ VK_CLEAR ] = XBMCK_CLEAR;
                VK_keymap[ VK_RETURN ] = XBMCK_RETURN;
                VK_keymap[ VK_PAUSE ] = XBMCK_PAUSE;
                VK_keymap[ VK_SPACE ] = XBMCK_SPACE;

                VK_keymap[ Qt::Key_Escape ] = XBMCK_ESCAPE;
                VK_keymap[ VK_APOSTROPHE ] = XBMCK_QUOTE;
                VK_keymap[ VK_COMMA ] = XBMCK_COMMA;
                VK_keymap[ VK_MINUS ] = XBMCK_MINUS;
                VK_keymap[ VK_PERIOD ] = XBMCK_PERIOD;
                VK_keymap[ VK_SLASH ] = XBMCK_SLASH;
                VK_keymap[ VK_0 ] = XBMCK_0;
                VK_keymap[ VK_1 ] = XBMCK_1;
                VK_keymap[ VK_2 ] = XBMCK_2;
                VK_keymap[ VK_3 ] = XBMCK_3;
                VK_keymap[ VK_4 ] = XBMCK_4;
                VK_keymap[ VK_5 ] = XBMCK_5;
                VK_keymap[ VK_6 ] = XBMCK_6;
                VK_keymap[ VK_7 ] = XBMCK_7;
                VK_keymap[ VK_8 ] = XBMCK_8;
                VK_keymap[ VK_9 ] = XBMCK_9;
                VK_keymap[ VK_SEMICOLON ] = XBMCK_SEMICOLON;
                VK_keymap[ VK_EQUALS ] = XBMCK_EQUALS;
                VK_keymap[ VK_LBRACKET ] = XBMCK_LEFTBRACKET;
                VK_keymap[ VK_BACKSLASH ] = XBMCK_BACKSLASH;
                VK_keymap[ VK_OEM_102 ] = XBMCK_BACKSLASH;
                VK_keymap[ VK_RBRACKET ] = XBMCK_RIGHTBRACKET;
                VK_keymap[ VK_GRAVE ] = XBMCK_BACKQUOTE;
                VK_keymap[ VK_BACKTICK ] = XBMCK_BACKQUOTE;
                VK_keymap[ VK_A ] = XBMCK_a;
                VK_keymap[ VK_B ] = XBMCK_b;
                VK_keymap[ VK_C ] = XBMCK_c;
                VK_keymap[ VK_D ] = XBMCK_d;
                VK_keymap[ VK_E ] = XBMCK_e;
                VK_keymap[ VK_F ] = XBMCK_f;
                VK_keymap[ VK_G ] = XBMCK_g;
                VK_keymap[ VK_H ] = XBMCK_h;
                VK_keymap[ VK_I ] = XBMCK_i;
                VK_keymap[ VK_J ] = XBMCK_j;
                VK_keymap[ VK_K ] = XBMCK_k;
                VK_keymap[ VK_L ] = XBMCK_l;
                VK_keymap[ VK_M ] = XBMCK_m;
                VK_keymap[ VK_N ] = XBMCK_n;
                VK_keymap[ VK_O ] = XBMCK_o;
                VK_keymap[ VK_P ] = XBMCK_p;
                VK_keymap[ VK_Q ] = XBMCK_q;
                VK_keymap[ VK_R ] = XBMCK_r;
                VK_keymap[ VK_S ] = XBMCK_s;
                VK_keymap[ VK_T ] = XBMCK_t;
                VK_keymap[ VK_U ] = XBMCK_u;
                VK_keymap[ VK_V ] = XBMCK_v;
                VK_keymap[ VK_W ] = XBMCK_w;
                VK_keymap[ VK_X ] = XBMCK_x;
                VK_keymap[ VK_Y ] = XBMCK_y;
                VK_keymap[ VK_Z ] = XBMCK_z;
                VK_keymap[ VK_DELETE ] = XBMCK_DELETE;

                VK_keymap[ VK_NUMPAD0 ] = XBMCK_KP0;
                VK_keymap[ VK_NUMPAD1 ] = XBMCK_KP1;
                VK_keymap[ VK_NUMPAD2 ] = XBMCK_KP2;
                VK_keymap[ VK_NUMPAD3 ] = XBMCK_KP3;
                VK_keymap[ VK_NUMPAD4 ] = XBMCK_KP4;
                VK_keymap[ VK_NUMPAD5 ] = XBMCK_KP5;
                VK_keymap[ VK_NUMPAD6 ] = XBMCK_KP6;
                VK_keymap[ VK_NUMPAD7 ] = XBMCK_KP7;
                VK_keymap[ VK_NUMPAD8 ] = XBMCK_KP8;
                VK_keymap[ VK_NUMPAD9 ] = XBMCK_KP9;
                VK_keymap[ VK_DECIMAL ] = XBMCK_KP_PERIOD;
                VK_keymap[ VK_DIVIDE ] = XBMCK_KP_DIVIDE;
                VK_keymap[ VK_MULTIPLY ] = XBMCK_KP_MULTIPLY;
                VK_keymap[ VK_SUBTRACT ] = XBMCK_KP_MINUS;
                VK_keymap[ VK_ADD ] = XBMCK_KP_PLUS;

                VK_keymap[ VK_UP ] = XBMCK_UP;
                VK_keymap[ VK_DOWN ] = XBMCK_DOWN;
                VK_keymap[ VK_RIGHT ] = XBMCK_RIGHT;
                VK_keymap[ VK_LEFT ] = XBMCK_LEFT;
                VK_keymap[ VK_INSERT ] = XBMCK_INSERT;
                VK_keymap[ VK_HOME ] = XBMCK_HOME;
                VK_keymap[ VK_END ] = XBMCK_END;
                VK_keymap[ VK_PRIOR ] = XBMCK_PAGEUP;
                VK_keymap[ VK_NEXT ] = XBMCK_PAGEDOWN;

                VK_keymap[ VK_F1 ] = XBMCK_F1;
                VK_keymap[ VK_F2 ] = XBMCK_F2;
                VK_keymap[ VK_F3 ] = XBMCK_F3;
                VK_keymap[ VK_F4 ] = XBMCK_F4;
                VK_keymap[ VK_F5 ] = XBMCK_F5;
                VK_keymap[ VK_F6 ] = XBMCK_F6;
                VK_keymap[ VK_F7 ] = XBMCK_F7;
                VK_keymap[ VK_F8 ] = XBMCK_F8;
                VK_keymap[ VK_F9 ] = XBMCK_F9;
                VK_keymap[ VK_F10 ] = XBMCK_F10;
                VK_keymap[ VK_F11 ] = XBMCK_F11;
                VK_keymap[ VK_F12 ] = XBMCK_F12;
                VK_keymap[ VK_F13 ] = XBMCK_F13;
                VK_keymap[ VK_F14 ] = XBMCK_F14;
                VK_keymap[ VK_F15 ] = XBMCK_F15;

                VK_keymap[ VK_NUMLOCK ] = XBMCK_NUMLOCK;
                VK_keymap[ VK_CAPITAL ] = XBMCK_CAPSLOCK;
                VK_keymap[ VK_SCROLL ] = XBMCK_SCROLLOCK;
                VK_keymap[ VK_RSHIFT ] = XBMCK_RSHIFT;
                VK_keymap[ VK_LSHIFT ] = XBMCK_LSHIFT;
                VK_keymap[ VK_RCONTROL ] = XBMCK_RCTRL;
                VK_keymap[ VK_LCONTROL ] = XBMCK_LCTRL;
                VK_keymap[ VK_RMENU ] = XBMCK_RALT;
                VK_keymap[ VK_LMENU ] = XBMCK_LALT;
                VK_keymap[ VK_RWIN ] = XBMCK_RSUPER;
                VK_keymap[ VK_LWIN ] = XBMCK_LSUPER;

                VK_keymap[ VK_HELP ] = XBMCK_HELP;
#ifdef VK_PRINT
                VK_keymap[ VK_PRINT ] = XBMCK_PRINT;
#endif
                VK_keymap[ VK_SNAPSHOT ] = XBMCK_PRINT;
                VK_keymap[ VK_CANCEL ] = XBMCK_BREAK;
                VK_keymap[ VK_APPS ] = XBMCK_MENU;

                // Only include the multimedia keys if they have been enabled in the
                // advanced settings
                if( CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_enableMultimediaKeys )
                {
                    VK_keymap[ VK_BROWSER_BACK ] = XBMCK_BROWSER_BACK;
                    VK_keymap[ VK_BROWSER_FORWARD ] = XBMCK_BROWSER_FORWARD;
                    VK_keymap[ VK_BROWSER_REFRESH ] = XBMCK_BROWSER_REFRESH;
                    VK_keymap[ VK_BROWSER_STOP ] = XBMCK_BROWSER_STOP;
                    VK_keymap[ VK_BROWSER_SEARCH ] = XBMCK_BROWSER_SEARCH;
                    VK_keymap[ VK_BROWSER_FAVORITES ] = XBMCK_BROWSER_FAVORITES;
                    VK_keymap[ VK_BROWSER_HOME ] = XBMCK_BROWSER_HOME;
                    VK_keymap[ VK_VOLUME_MUTE ] = XBMCK_VOLUME_MUTE;
                    VK_keymap[ VK_VOLUME_DOWN ] = XBMCK_VOLUME_DOWN;
                    VK_keymap[ VK_VOLUME_UP ] = XBMCK_VOLUME_UP;
                    VK_keymap[ VK_MEDIA_NEXT_TRACK ] = XBMCK_MEDIA_NEXT_TRACK;
                    VK_keymap[ VK_MEDIA_PREV_TRACK ] = XBMCK_MEDIA_PREV_TRACK;
                    VK_keymap[ VK_MEDIA_STOP ] = XBMCK_MEDIA_STOP;
                    VK_keymap[ VK_MEDIA_PLAY_PAUSE ] = XBMCK_MEDIA_PLAY_PAUSE;
                    VK_keymap[ VK_LAUNCH_MAIL ] = XBMCK_LAUNCH_MAIL;
                    VK_keymap[ VK_LAUNCH_MEDIA_SELECT ] = XBMCK_LAUNCH_MEDIA_SELECT;
                    VK_keymap[ VK_LAUNCH_APP1 ] = XBMCK_LAUNCH_APP1;
                    VK_keymap[ VK_LAUNCH_APP2 ] = XBMCK_LAUNCH_APP2;
                }
            }
        }
    }
}

//============================================================================
int TranslateKeyQt( int keyQt, int& mod )
{
    keyQt &= ~( Qt::KeypadModifier |  Qt::GroupSwitchModifier );

    mod = 0;
    if( keyQt & Qt::ShiftModifier )
    {
        keyQt &= ~Qt::ShiftModifier;
        mod |= XBMCKMOD_LSHIFT;
    }

    if( keyQt & Qt::ControlModifier )
    {
        keyQt &= ~Qt::ControlModifier;
        mod |= XBMCKMOD_LCTRL;
    }

    if( keyQt & Qt::AltModifier )
    {
        keyQt &= ~Qt::AltModifier;
        mod |= XBMCKMOD_LALT;
    }

    if( keyQt & Qt::MetaModifier )
    {
        keyQt &= ~Qt::MetaModifier;
        mod |= XBMCKMOD_LSUPER;
    }

    switch( keyQt )
    {
    case Qt::Key_Backspace:
        return XBMCK_BACKSPACE;
    case Qt::Key_Tab:
        return XBMCK_TAB;
    case Qt::Key_Clear:
        return XBMCK_CLEAR;
    case Qt::Key_Return:
        return XBMCK_RETURN;
    case Qt::Key_Pause:
        return XBMCK_PAUSE;
    case Qt::Key_Escape:
        return XBMCK_ESCAPE;
    case Qt::Key_Space:
        return XBMCK_SPACE;

    case Qt::Key_QuoteDbl:
        return XBMCK_QUOTE;

    case Qt::Key_Comma:
        return XBMCK_COMMA;
    case Qt::Key_Minus:
        return XBMCK_MINUS;
    case Qt::Key_Plus:
        return XBMCK_PLUS;

    case Qt::Key_Period:
        return XBMCK_PERIOD;
    case Qt::Key_Slash:
        return XBMCK_SLASH;

    case Qt::Key_0:
        return XBMCK_0;
    case Qt::Key_1:
        return XBMCK_1;
    case Qt::Key_2:
        return XBMCK_2;
    case Qt::Key_3:
        return XBMCK_3;
    case Qt::Key_4:
        return XBMCK_4;
    case Qt::Key_5:
        return XBMCK_5;
    case Qt::Key_6:
        return XBMCK_6;
    case Qt::Key_7:
        return XBMCK_7;
    case Qt::Key_8:
        return XBMCK_8;
    case Qt::Key_9:
        return XBMCK_9;    

    case Qt::Key_Semicolon:
        return XBMCK_SEMICOLON;
    case Qt::Key_Equal:
        return XBMCK_EQUALS;
    case Qt::Key_BracketLeft:
        return XBMCK_LEFTBRACKET;
    case Qt::Key_Backslash:
        return XBMCK_BACKSLASH;
    case Qt::Key_BracketRight:
        return XBMCK_RIGHTBRACKET;
    case Qt::Key_QuoteLeft:
        return XBMCK_BACKQUOTE;

    case Qt::Key_A:
        return XBMCK_a;
    case Qt::Key_B:
        return XBMCK_b;
    case Qt::Key_C:
        return XBMCK_c;
    case Qt::Key_D:
        return XBMCK_d;
    case Qt::Key_E:
        return XBMCK_e;
    case Qt::Key_F:
        return XBMCK_f;
    case Qt::Key_G:
        return XBMCK_g;
    case Qt::Key_H:
        return XBMCK_h;
    case Qt::Key_I:
        return XBMCK_i;
    case Qt::Key_J:
        return XBMCK_j;
    case Qt::Key_K:
        return XBMCK_k;
    case Qt::Key_L:
        return XBMCK_l;
    case Qt::Key_M:
        return XBMCK_m;
    case Qt::Key_N:
        return XBMCK_n;
    case Qt::Key_O:
        return XBMCK_o;
    case Qt::Key_P:
        return XBMCK_p;
    case Qt::Key_Q:
        return XBMCK_q;
    case Qt::Key_R:
        return XBMCK_r;
    case Qt::Key_S:
        return XBMCK_s;
    case Qt::Key_T:
        return XBMCK_t;
    case Qt::Key_U:
        return XBMCK_u;
    case Qt::Key_V:
        return XBMCK_v;
    case Qt::Key_W:
        return XBMCK_w;
    case Qt::Key_X:
        return XBMCK_x;
    case Qt::Key_Y:
        return XBMCK_y;
    case Qt::Key_Z:
        return XBMCK_z;

    case Qt::Key_ParenLeft:
        return XBMCK_LEFTPAREN;
    case Qt::Key_ParenRight:
        return XBMCK_RIGHTPAREN;
    case Qt::Key_Asterisk:
        return XBMCK_ASTERISK;
    case Qt::Key_Exclam:
        return XBMCK_EXCLAIM;

    case Qt::Key_Delete:
        return XBMCK_DELETE;

//            VK_keymap[ VK_NUMPAD0 ] = XBMCK_KP0;
//            VK_keymap[ VK_NUMPAD1 ] = XBMCK_KP1;
//            VK_keymap[ VK_NUMPAD2 ] = XBMCK_KP2;
//            VK_keymap[ VK_NUMPAD3 ] = XBMCK_KP3;
//            VK_keymap[ VK_NUMPAD4 ] = XBMCK_KP4;
//            VK_keymap[ VK_NUMPAD5 ] = XBMCK_KP5;
//            VK_keymap[ VK_NUMPAD6 ] = XBMCK_KP6;
//            VK_keymap[ VK_NUMPAD7 ] = XBMCK_KP7;
//            VK_keymap[ VK_NUMPAD8 ] = XBMCK_KP8;
//            VK_keymap[ VK_NUMPAD9 ] = XBMCK_KP9;

    case Qt::Key_Less:
        return XBMCK_LESS;
    case Qt::Key_Greater:
        return XBMCK_GREATER;

    case Qt::Key_division:
        return XBMCK_KP_DIVIDE;

    case Qt::Key_Up:
        return XBMCK_UP;
    case Qt::Key_Down:
        return XBMCK_DOWN;
    case Qt::Key_Right:
        return XBMCK_RIGHT;
    case Qt::Key_Left:
        return XBMCK_LEFT;

    case Qt::Key_Insert:
        return XBMCK_INSERT;
    case Qt::Key_Home:
        return XBMCK_HOME;
    case Qt::Key_End:
        return XBMCK_END;
    case Qt::Key_PageUp:
        return XBMCK_PAGEUP;
    case Qt::Key_PageDown:
        return XBMCK_PAGEDOWN;

    case Qt::Key_F1:
        return XBMCK_F1;
    case Qt::Key_F2:
        return XBMCK_F2;
    case Qt::Key_F3:
        return XBMCK_F3;
    case Qt::Key_F4:
        return XBMCK_F4;
    case Qt::Key_F5:
        return XBMCK_F5;
    case Qt::Key_F6:
        return XBMCK_F6;
    case Qt::Key_F7:
        return XBMCK_F7;
    case Qt::Key_F8:
        return XBMCK_F8;
    case Qt::Key_F9:
        return XBMCK_F9;

    case Qt::Key_F10:
        return XBMCK_F10;
    case Qt::Key_F11:
        return XBMCK_F11;
    case Qt::Key_F12:
        return XBMCK_F12;
    case Qt::Key_F13:
        return XBMCK_F13;
    case Qt::Key_F14:
        return XBMCK_F14;
    case Qt::Key_F15:
        return XBMCK_F15;

    case Qt::Key_NumLock:
        return XBMCK_NUMLOCK;
    case Qt::Key_CapsLock:
        return XBMCK_CAPSLOCK;
    case Qt::Key_ScrollLock:
        return XBMCK_SCROLLOCK;

            //VK_keymap[ VK_RSHIFT ] = XBMCK_RSHIFT;
            //VK_keymap[ VK_LSHIFT ] = XBMCK_LSHIFT;
            //VK_keymap[ VK_RCONTROL ] = XBMCK_RCTRL;
            //VK_keymap[ VK_LCONTROL ] = XBMCK_LCTRL;
            //VK_keymap[ VK_RMENU ] = XBMCK_RALT;
            //VK_keymap[ VK_LMENU ] = XBMCK_LALT;
            //VK_keymap[ VK_RWIN ] = XBMCK_RSUPER;
            //VK_keymap[ VK_LWIN ] = XBMCK_LSUPER;

    case Qt::Key_Help:
        return XBMCK_HELP;

    case Qt::Key_Print:
        return XBMCK_PRINT;

        
        //VK_keymap[ VK_BROWSER_BACK ] = XBMCK_BROWSER_BACK;
        //        VK_keymap[ VK_BROWSER_FORWARD ] = XBMCK_BROWSER_FORWARD;
        //        VK_keymap[ VK_BROWSER_REFRESH ] = XBMCK_BROWSER_REFRESH;
        //        VK_keymap[ VK_BROWSER_STOP ] = XBMCK_BROWSER_STOP;
        //        VK_keymap[ VK_BROWSER_SEARCH ] = XBMCK_BROWSER_SEARCH;
        //        VK_keymap[ VK_BROWSER_FAVORITES ] = XBMCK_BROWSER_FAVORITES;
        //        VK_keymap[ VK_BROWSER_HOME ] = XBMCK_BROWSER_HOME;

    case Qt::Key_VolumeMute:
        return XBMCK_VOLUME_MUTE;
    case Qt::Key_VolumeDown:
        return XBMCK_VOLUME_DOWN;
    case Qt::Key_VolumeUp:
        return XBMCK_VOLUME_UP;

    case Qt::Key_MediaNext:
        return XBMCK_MEDIA_NEXT_TRACK;
    case Qt::Key_MediaPrevious:
        return XBMCK_MEDIA_PREV_TRACK;
    case Qt::Key_MediaStop:
        return XBMCK_MEDIA_STOP;
    case Qt::Key_MediaPause:
        return XBMCK_MEDIA_PLAY_PAUSE;

    case Qt::Key_MediaRecord:
        return XBMCK_RECORD;

                //VK_keymap[ VK_LAUNCH_MAIL ] = XBMCK_LAUNCH_MAIL;
                //VK_keymap[ VK_LAUNCH_MEDIA_SELECT ] = XBMCK_LAUNCH_MEDIA_SELECT;
                //VK_keymap[ VK_LAUNCH_APP1 ] = XBMCK_LAUNCH_APP1;
                //VK_keymap[ VK_LAUNCH_APP2 ] = XBMCK_LAUNCH_APP2;

        default:
            return XBMCK_UNKNOWN;
    }

    return XBMCK_UNKNOWN;
}

//============================================================================
int TranslateMouseButtonQt( int mouseButton )
{
    switch( mouseButton )
    {
    case Qt::RightButton:
        return XBMC_BUTTON_RIGHT;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    case Qt::MiddleButton:
        return XBMC_BUTTON_MIDDLE;
#else
    case Qt::MidButton:
        return XBMC_BUTTON_MIDDLE;
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)

    case Qt::LeftButton:
    default:
        return XBMC_BUTTON_LEFT;
    }
}

//============================================================================
EventsQtToGoTv::EventsQtToGoTv( AppCommon& myApp )
: m_MyApp( myApp )
{
}

//============================================================================
bool EventsQtToGoTv::fromGuiKeyPressEvent( int key )
{
    int mod = 0;
    int kodiKey = TranslateKeyQt( key, mod );

    m_MyApp.fromGuiKeyPressEvent( (int)eAppModuleKodi, kodiKey, mod );
    return false;
}

//============================================================================
bool EventsQtToGoTv::fromGuiKeyReleaseEvent( int key )
{
    int mod = 0;
    int kodiKey = TranslateKeyQt( key, mod );

    m_MyApp.fromGuiKeyPressEvent( (int)eAppModuleKodi, kodiKey, mod );
    return false;
}

//============================================================================
bool EventsQtToGoTv::fromGuiMousePressEvent( int mouseXPos, int mouseyPos, int mouseButton )
{
    m_MyApp.fromGuiMousePressEvent( ( int )eAppModuleKodi, mouseXPos, mouseyPos, TranslateMouseButtonQt( mouseButton ) );
    return false;
}

//============================================================================
bool EventsQtToGoTv::fromGuiMouseReleaseEvent( int mouseXPos, int mouseyPos, int mouseButton )
{
    m_MyApp.fromGuiMouseReleaseEvent( ( int )eAppModuleKodi, mouseXPos, mouseyPos, TranslateMouseButtonQt( mouseButton ) );
    return false;
}

//============================================================================
bool EventsQtToGoTv::fromGuiMouseMoveEvent( int mouseXPos, int mouseyPos )
{
    m_MyApp.fromGuiMouseMoveEvent( ( int )eAppModuleKodi, mouseXPos, mouseyPos );
    return false;
}

//============================================================================
void EventsQtToGoTv::fromGuiResizeBegin( int winWidth, int winHeight )
{
	m_MyApp.fromGuiResizeBegin( (int)eAppModuleKodi, winWidth, winHeight );
}

//============================================================================
void EventsQtToGoTv::fromGuiResizeEvent( int winWidth, int winHeight )
{
    m_MyApp.fromGuiResizeEvent( ( int )eAppModuleKodi, winWidth, winHeight );
}

//============================================================================
void EventsQtToGoTv::fromGuiResizeEnd( int winWidth, int winHeight )
{
	m_MyApp.fromGuiResizeEnd( (int)eAppModuleKodi, winWidth, winHeight );
}

//============================================================================
void EventsQtToGoTv::fromGuiCloseEvent( )
{
    m_MyApp.fromGuiCloseEvent( ( int )eAppModuleKodi );
}

//============================================================================
void EventsQtToGoTv::fromGuiVisibleEvent( bool isVisible )
{
    m_MyApp.fromGuiVisibleEvent( ( int )eAppModuleKodi, isVisible );
}
