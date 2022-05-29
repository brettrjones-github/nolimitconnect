//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

// #include <PktLib/VxCommon.h>
// #include <CoreLib/VxGUID.h>

#include "AppTranslate.h"

#include <QLocale>

// for reference
// QT_TRANSLATE_NOOP( "QObject","Greek" ),      "el", QLocale::Greek );

QString AppTranslate::describeLanguage( ELanguageType langType )
{
    QString lang = "English";
    switch( langType )
    {
    case eLangUnspecified:
        lang = "Any";
        break;
    case eLangEnglish:
        lang = "English";
        break;
    case eLangBulgarian:
        lang = "Bulgarian";
        break;
    case eLangChinese:
        lang = "Chinese";
        break;
    case eLangCroatian:
        lang = "Croatian";
        break;
    case eLangCzech:
        lang = "Czech";
        break;
    case eLangDanish:
        lang = "Danish";
        break;
    case eLangDutch:
        lang = "Dutch";
        break;
    case eLangEstonian:
        lang = "Estonian";
        break;
    case eLangFinnish:
        lang = "Finnish";
        break;
    case eLangFrench:
        lang = "French";
        break;
    case eLangGerman:
        lang = "German";
        break;
    case eLangGreek:
        lang = "Greek";
        break;
    case eLangHindi:
        lang = "Hindi";
        break;
    case eLangHungarian:
        lang = "Hungarian";
        break;
    case eLangItalian:
        lang = "Italian";
        break;
    case eLangJapanese:
        lang = "Japanese";
        break;
    case eLangLatvian:
        lang = "Latvian";
        break;
    case eLangLithuanian:
        lang = "Lithuanian";
        break;
    case eLangPolish:
        lang = "Polish";
        break;
    case eLangPortuguese:
        lang = "Portuguese";
        break;
    case eLangRomanian:
        lang = "Romanian";
        break;
    case eLangRussian:
        lang = "Russian";
        break;
    case eLangSerbian:
        lang = "Serbian";
        break;
    case eLangSlovak:
        lang = "Slovak";
        break;
    case eLangSpanish:
        lang = "Spanish";
        break;
    case eLangSwedish:
        lang = "Swedish";
        break;
    case eLangThai:
        lang = "Thai";
        break;
    case eLangTurkish:
        lang = "Turkish";
        break;
    case eLangUkrainian:
        lang = "Ukrainian";
        break;

    default:
        break;
    }

    return lang;
}

//============================================================================
QLocale AppTranslate::getLocale( ELanguageType langType )
{
    QLocale locale = QLocale::English;
    switch( langType )
    {
    case eLangEnglish:
        locale = QLocale::English;
        break;
    case eLangBulgarian:
        locale = QLocale::Bulgarian;
        break;
    case eLangChinese:
        locale = QLocale::Chinese;
        break;
    case eLangCroatian:
        locale = QLocale::Croatian;
        break;
    case eLangCzech:
        locale = QLocale::Czech;
        break;
    case eLangDanish:
        locale = QLocale::Danish;
        break;
    case eLangDutch:
        locale = QLocale::Dutch;
        break;
    case eLangEstonian:
        locale = QLocale::Estonian;
        break;
    case eLangFinnish:
        locale = QLocale::Finnish;
        break;
    case eLangFrench:
        locale = QLocale::French;
        break;
    case eLangGerman:
        locale = QLocale::German;
        break;
    case eLangGreek:
        locale = QLocale::Greek;
        break;
    case eLangHindi:
        locale = QLocale::Hindi;
        break;
    case eLangHungarian:
        locale = QLocale::Hungarian;
        break;
    case eLangItalian:
        locale = QLocale::Italian;
        break;
    case eLangJapanese:
        locale = QLocale::Japanese;
        break;
    case eLangLatvian:
        locale = QLocale::Latvian;
        break;
    case eLangLithuanian:
        locale = QLocale::Lithuanian;
        break;
    case eLangPolish:
        locale = QLocale::Polish;
        break;
    case eLangPortuguese:
        locale = QLocale::Portuguese;
        break;
    case eLangRomanian:
        locale = QLocale::Romanian;
        break;
    case eLangRussian:
        locale = QLocale::Russian;
        break;
    case eLangSerbian:
        locale = QLocale::Serbian;
        break;
    case eLangSlovak:
        locale = QLocale::Slovak;
        break;
    case eLangSpanish:
        locale = QLocale::Spanish;
        break;
    case eLangSwedish:
        locale = QLocale::Swedish;
        break;
    case eLangThai:
        locale = QLocale::Thai;
        break;
    case eLangTurkish:
        locale = QLocale::Turkish;
        break;
    case eLangUkrainian:
        locale = QLocale::Ukrainian;
        break;

    default:
        break;
    }

    return locale;
}
