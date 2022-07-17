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

#ifndef GOTVQT_WIDGETSEEKPROGRESS_H_
#define GOTVQT_WIDGETSEEKPROGRESS_H_

#include "WidgetSeek.h"

class QProgressBar;

/*!
    \class NlcPtoPWidgetSeekProgress WidgetSeekProgress.h GOTVQtWidgets/WidgetSeekProgress.h
    \ingroup GOTVQtWidgets
    \brief Seek widget

    This is one of GOTV-Qt GUI classes.
    It provides a display of elapsed time and full time of the media using a progress bar.
*/
class NlcPtoPWidgetSeekProgress : public NlcPtoPWidgetSeek
{
    Q_OBJECT
public:
    /*!
        \brief NlcPtoPWidgetSeekProgress constructor
        \param player media player
        \param parent seek widget's parent GUI widget
    */
    explicit NlcPtoPWidgetSeekProgress(NlcPtoPMediaPlayer *player,
                                   QWidget *parent = 0);

    /*!
        \brief NlcPtoPWidgetSeekProgress constructor
        \param parent seek widget's parent GUI widget
    */
    explicit NlcPtoPWidgetSeekProgress(QWidget *parent = 0);

    /*!
        \brief NlcPtoPWidgetSeekProgress destructor
    */
    ~NlcPtoPWidgetSeekProgress();

protected:
    /*!
        \brief Mouse move event override
        \param event mouse event
    */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
        \brief Mouse press event override
        \param event mouse event
    */
    void mousePressEvent(QMouseEvent *event);

    /*!
        \brief Mouse release event override
        \param event mouse event
    */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
        \brief Mouse wheel event override
        \param event mouse event
    */
    void wheelEvent(QWheelEvent *event);

private slots:
    virtual void updateCurrentTime(int time);
    virtual void updateFullTime(int time);

private:
    virtual void setSliderWidget(QWidget *slider,
                                 bool updateSlider = true);

    void updateEvent(const QPoint &pos);

    void lock();
    void unlock();

    bool _lock;
};

#endif // GOTVQT_WIDGETSEEKPROGRESS_H_
