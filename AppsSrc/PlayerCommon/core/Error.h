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

#ifndef GOTVQT_ERROR_H_
#define GOTVQT_ERROR_H_

#include "SharedExportCore.h"

/*!
    \class NlcPtoPError Error.h core/Error.h
    \ingroup GOTVQtCore
	\brief Error handler

	A basic error handler for GOTV-Qt library.
*/
class GOTVQT_CORE_EXPORT NlcPtoPError
{
public:
    /*!
        \brief A human-readable error message for the last libgotvptop error in the calling thread.

        The resulting string is valid until another error occurs.
        \return error message (QString)
        \warning There may be no output, if there is no error.
    */
    static QString errmsg();

    /*!
        \brief A human-readable error message displayed into standard output for the last libgotvptop error in the calling thread.

		The resulting string is valid until another error occurs.
		\warning There may be no output, if there is no error.
	*/
    static void showErrmsg();

    /*!
		\brief Clears the libgotvptop error status for the current thread.

        This does nothing.

        \deprecated Deprecated since GOTV-Qt 1.1, will be removed in 2.0
	*/
    Q_DECL_DEPRECATED static void clearerr();
};

#endif // GOTVQT_ERROR_H_
