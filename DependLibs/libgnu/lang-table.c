/* Table of languages.
   Copyright (C) 2001-2009, 2015-2016 Free Software Foundation, Inc.
   Written by Bruno Haible <haible@clisp.cons.org>, 2005.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

# include <libgnu/config_libgnu.h>

/* Specification.  */
#include "lang-table.h"

/* Derived from ISO 639.  */
struct language_table_entry language_table[] =
  {
    { "aa", "Afar" },
    { "ab", "Abkhazian" },
    { "ace", "Achinese" },
    { "ae", "Avestan" },
    { "af", "Afrikaans" },
    { "ak", "Akan" },
    { "am", "Amharic" },
    { "an", "Aragonese" },
    { "ang", "Old English" },
    { "ar", "Arabic" },
    { "arn", "Mapudungun" },
    { "as", "Assamese" },
    { "ast", "Asturian" },
    { "av", "Avaric" },
    { "awa", "Awadhi" },
    { "ay", "Aymara" },
    { "az", "Azerbaijani" },
    { "ba", "Bashkir" },
    { "bal", "Baluchi" },
    { "ban", "Balinese" },
    { "be", "Belarusian" },
    { "bej", "Beja" },
    { "bem", "Bemba" },
    { "bg", "Bulgarian" },
    { "bh", "Bihari" },
    { "bho", "Bhojpuri" },
    { "bi", "Bislama" },
    { "bik", "Bikol" },
    { "bin", "Bini" },
    { "bm", "Bambara" },
    { "bn", "Bengali" },
    { "bo", "Tibetan" },
    { "br", "Breton" },
    { "bs", "Bosnian" },
    { "bug", "Buginese" },
    { "ca", "Catalan" },
    { "ce", "Chechen" },
    { "ceb", "Cebuano" },
    { "ch", "Chamorro" },
    { "co", "Corsican" },
    { "cr", "Cree" },
    { "crh", "Crimean Tatar" },
    { "cs", "Czech" },
    { "csb", "Kashubian" },
    { "cu", "Church Slavic" },
    { "cv", "Chuvash" },
    { "cy", "Welsh" },
    { "da", "Danish" },
    { "de", "German" },
    { "din", "Dinka" },
    { "doi", "Dogri" },
    { "dsb", "Lower Sorbian" },
    { "dv", "Divehi" },
    { "dz", "Dzongkha" },
    { "ee", "Ewe" },
    { "el", "Greek" },
    { "en", "English" },
    { "eo", "Esperanto" },
    { "es", "Spanish" },
    { "et", "Estonian" },
    { "eu", "Basque" },
    { "fa", "Persian" },
    { "ff", "Fulah" },
    { "fi", "Finnish" },
    { "fil", "Filipino" },
    { "fj", "Fijian" },
    { "fo", "Faroese" },
    { "fon", "Fon" },
    { "fr", "French" },
    { "fur", "Friulian" },
    { "fy", "Western Frisian" },
    { "ga", "Irish" },
    { "gd", "Scottish Gaelic" },
    { "gl", "Galician" },
    { "gn", "Guarani" },
    { "gon", "Gondi" },
    { "gsw", "Swiss German" }, /* can also be "Alsatian" */
    { "gu", "Gujarati" },
    { "gv", "Manx" },
    { "ha", "Hausa" },
    { "he", "Hebrew" },
    { "hi", "Hindi" },
    { "hil", "Hiligaynon" },
    { "hmn", "Hmong" },
    { "ho", "Hiri Motu" },
    { "hr", "Croatian" },
    { "hsb", "Upper Sorbian" },
    { "ht", "Haitian" },
    { "hu", "Hungarian" },
    { "hy", "Armenian" },
    { "hz", "Herero" },
    { "ia", "Interlingua" },
    { "id", "Indonesian" },
    { "ie", "Interlingue" },
    { "ig", "Igbo" },
    { "ii", "Sichuan Yi" },
    { "ik", "Inupiak" },
    { "ilo", "Iloko" },
    { "is", "Icelandic" },
    { "it", "Italian" },
    { "iu", "Inuktitut" },
    { "ja", "Japanese" },
    { "jab", "Hyam" },
    { "jv", "Javanese" },
    { "ka", "Georgian" },
    { "kab", "Kabyle" },
    { "kaj", "Jju" },
    { "kam", "Kamba" },
    { "kbd", "Kabardian" },
    { "kcg", "Tyap" },
    { "kdm", "Kagoma" },
    { "kg", "Kongo" },
    { "ki", "Kikuyu" },
    { "kj", "Kuanyama" },
    { "kk", "Kazakh" },
    { "kl", "Kalaallisut" },
    { "km", "Central Khmer" },
    { "kmb", "Kimbundu" },
    { "kn", "Kannada" },
    { "ko", "Korean" },
    { "kr", "Kanuri" },
    { "kru", "Kurukh" },
    { "ks", "Kashmiri" },
    { "ku", "Kurdish" },
    { "kv", "Komi" },
    { "kw", "Cornish" },
    { "ky", "Kirghiz" },
    { "kok", "Konkani" },
    { "la", "Latin" },
    { "lb", "Letzeburgesch" },
    { "lg", "Ganda" },
    { "li", "Limburgish" },
    { "ln", "Lingala" },
    { "lo", "Laotian" },
    { "lt", "Lithuanian" },
    { "lu", "Luba-Katanga" },
    { "lua", "Luba-Lulua" },
    { "luo", "Luo" },
    { "lv", "Latvian" },
    { "mad", "Madurese" },
    { "mag", "Magahi" },
    { "mai", "Maithili" },
    { "mak", "Makasar" },
    { "man", "Mandingo" },
    { "men", "Mende" },
    { "mg", "Malagasy" },
    { "mh", "Marshallese" },
    { "mi", "Maori" },
    { "min", "Minangkabau" },
    { "mk", "Macedonian" },
    { "ml", "Malayalam" },
    { "mn", "Mongolian" },
    { "mni", "Manipuri" },
    { "mo", "Moldavian" },
    { "moh", "Mohawk" },
    { "mos", "Mossi" },
    { "mr", "Marathi" },
    { "ms", "Malay" },
    { "mt", "Maltese" },
    { "mwr", "Marwari" },
    { "my", "Burmese" },
    { "myn", "Mayan" },
    { "na", "Nauru" },
    { "nap", "Neapolitan" },
    { "nah", "Nahuatl" },
    { "nb", "Norwegian Bokmal" },
    { "nd", "North Ndebele" },
    { "nds", "Low Saxon" },
    { "ne", "Nepali" },
    { "ng", "Ndonga" },
    { "nl", "Dutch" },
    { "nn", "Norwegian Nynorsk" },
    { "no", "Norwegian" },
    { "nr", "South Ndebele" },
    { "nso", "Northern Sotho" },
    { "nv", "Navajo" },
    { "ny", "Nyanja" },
    { "nym", "Nyamwezi" },
    { "nyn", "Nyankole" },
    { "oc", "Occitan" },
    { "oj", "Ojibwa" },
    { "om", "(Afan) Oromo" },
    { "or", "Oriya" },
    { "os", "Ossetian" },
    { "pa", "Punjabi" },
    { "pag", "Pangasinan" },
    { "pam", "Pampanga" },
    { "pap", "Papiamento" },
    { "pbb", "Páez" },
    { "pi", "Pali" },
    { "pl", "Polish" },
    { "ps", "Pashto" },
    { "pt", "Portuguese" },
    { "qu", "Quechua" },
    { "raj", "Rajasthani" },
    { "rm", "Romansh" },
    { "rn", "Kirundi" },
    { "ro", "Romanian" },
    { "ru", "Russian" },
    { "rw", "Kinyarwanda" },
    { "sa", "Sanskrit" },
    { "sah", "Yakut" },
    { "sas", "Sasak" },
    { "sat", "Santali" },
    { "sc", "Sardinian" },
    { "scn", "Sicilian" },
    { "sd", "Sindhi" },
    { "se", "Northern Sami" },
    { "sg", "Sango" },
    { "shn", "Shan" },
    { "si", "Sinhala" },
    { "sid", "Sidamo" },
    { "sk", "Slovak" },
    { "sl", "Slovenian" },
    { "sm", "Samoan" },
    { "sma", "Southern Sami" },
    { "smj", "Lule Sami" },
    { "smn", "Inari Sami" },
    { "sms", "Skolt Sami" },
    { "sn", "Shona" },
    { "so", "Somali" },
    { "sq", "Albanian" },
    { "sr", "Serbian" },
    { "srr", "Serer" },
    { "ss", "Siswati" },
    { "st", "Sesotho" },
    { "su", "Sundanese" },
    { "suk", "Sukuma" },
    { "sus", "Susu" },
    { "sv", "Swedish" },
    { "sw", "Swahili" },
    { "ta", "Tamil" },
    { "te", "Telugu" },
    { "tem", "Timne" },
    { "tet", "Tetum" },
    { "tg", "Tajik" },
    { "th", "Thai" },
    { "ti", "Tigrinya" },
    { "tiv", "Tiv" },
    { "tk", "Turkmen" },
    { "tl", "Tagalog" },
    { "tn", "Setswana" },
    { "to", "Tonga" },
    { "tr", "Turkish" },
    { "ts", "Tsonga" },
    { "tt", "Tatar" },
    { "tum", "Tumbuka" },
    { "tw", "Twi" },
    { "ty", "Tahitian" },
    { "ug", "Uighur" },
    { "uk", "Ukrainian" },
    { "umb", "Umbundu" },
    { "ur", "Urdu" },
    { "uz", "Uzbek" },
    { "ve", "Venda" },
    { "vi", "Vietnamese" },
    { "vo", "Volapuk" },
    { "wal", "Walamo" },
    { "war", "Waray" },
    { "wen", "Sorbian" },
    { "wo", "Wolof" },
    { "xh", "Xhosa" },
    { "yao", "Yao" },
    { "yi", "Yiddish" },
    { "yo", "Yoruba" },
    { "za", "Zhuang" },
    { "zh", "Chinese" },
    { "zu", "Zulu" },
    { "zap", "Zapotec" }
  };
const size_t language_table_size = sizeof (language_table) / sizeof (language_table[0]);

/* The language names for variants of languages, according to the catalog name
   (usually built from the language code and territory code).
   Should be consistent with the list of languages found on the TP site, see
   the URL contained in gettext-tools/projects/TP/teams.url.  */
struct language_table_entry language_variant_table[] =
  {
    { "de_AT", "Austrian" },
    { "en_GB", "English (British)" },
    { "es_AR", "Argentinian" },
    { "es_IC", "Spanish (Canary Islands)" },
    { "pt_BR", "Brazilian Portuguese" },
    { "zh_CN", "Chinese (simplified)" },
    { "zh_HK", "Chinese (Hong Kong)" },
    { "zh_TW", "Chinese (traditional)" }
  };
const size_t language_variant_table_size = sizeof (language_variant_table) / sizeof (language_variant_table[0]);
