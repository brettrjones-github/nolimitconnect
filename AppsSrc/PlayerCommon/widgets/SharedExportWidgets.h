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

#ifndef GOTVQT_SHARED_EXPORT_WIDGETS_H
#define GOTVQT_SHARED_EXPORT_WIDGETS_H

#include <QtCore/qglobal.h>

#include <gotvptop_config.h>

#ifdef USE_STATIC_LIBS
# define GOTVQT_WIDGETS_EXPORT
#elif defined(GOTVQT_WIDGETS_LIBRARY)
# define GOTVQT_WIDGETS_EXPORT Q_DECL_EXPORT
#else
# define GOTVQT_WIDGETS_EXPORT Q_DECL_IMPORT
#endif

#endif // SHARED_EXPORT_WIDGETS_H
