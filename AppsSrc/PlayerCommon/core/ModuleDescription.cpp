/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

#include "ModuleDescription.h"

GoTvPtoPModuleDescription::GoTvPtoPModuleDescription(Type type,
                                           const QString &name)
    : _type(type),
      _name(name) {}

GoTvPtoPModuleDescription::~GoTvPtoPModuleDescription() {} // LCOV_EXCL_LINE

void GoTvPtoPModuleDescription::setShortName(const QString &name)
{
    if (_shortName != name) {
        _shortName = name;
    }
}

void GoTvPtoPModuleDescription::setLongName(const QString &name)
{
    if (_longName != name) {
        _longName = name;
    }
}

void GoTvPtoPModuleDescription::setHelp(const QString &help)
{
    if (_help != help) {
        _help = help;
    }
}
