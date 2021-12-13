/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#pragma once

#include <QtCore/qglobal.h>
#include "config_gotvapps.h"
#ifdef USE_STATIC_LIBS
# define GOTVQT_CORE_EXPORT
#else
# if defined(GOTVQT_CORE_LIBRARY)
#  define GOTVQT_CORE_EXPORT Q_DECL_EXPORT
# else
#  define GOTVQT_CORE_EXPORT Q_DECL_IMPORT
# endif
#endif

#define GOTVQT_CORE_LIBRARY 1