/****************************************************************************
* GOTV-Qt - Qt and libgotvptop connector library
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#ifndef GOTVQT_WIDGETVIDEO_H_
#define GOTVQT_WIDGETVIDEO_H_

#include <QtCore/QTimer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QFrame>
#else
#include <QtGui/QFrame>
#endif

#include <core/Enums.h>
#include <core/VideoDelegate.h>

#include "SharedExportWidgets.h"

class GoTvPtoPMediaPlayer;

/*!
    \defgroup GOTVQtWidgets GOTV-Qt Widgets (GOTVQtWidgets)
    \brief Widget classes for faster media player developement
 */

/*!
    \class GoTvPtoPWidgetVideo WidgetVideo.h GOTVQtWidgets/WidgetVideo.h
    \ingroup GOTVQtWidgets
    \brief Video widget

    This is one of GOTV-Qt GUI classes.
    It provides video display and mouse control.
*/
class GOTVQT_WIDGETS_EXPORT GoTvPtoPWidgetVideo : public QFrame, public GoTvPtoPVideoDelegate
{
    Q_OBJECT
public:
    /*!
        \brief GoTvPtoPWidgetVideo constructor
        \param player media player
        \param parent video widget's parent GUI widget
    */
    explicit GoTvPtoPWidgetVideo(GoTvPtoPMediaPlayer *player,
                            QWidget *parent = 0);

    /*!
        \brief GoTvPtoPWidgetVideo constructor
        \param parent video widget's parent GUI widget
    */
    explicit GoTvPtoPWidgetVideo(QWidget *parent = 0);

    /*!
        \brief GoTvPtoPWidgetVideo destructor
    */
    ~GoTvPtoPWidgetVideo();

    /*!
        \brief Get current aspect ratio setting
        \return current aspect ratio
    */
    inline GoTvPtoP::Ratio currentAspectRatio() const { return _currentAspectRatio; }

    /*!
        \brief Get default aspect ratio setting
        \return default aspect ratio
    */
    inline GoTvPtoP::Ratio defaultAspectRatio() const { return _defaultAspectRatio; }

    /*!
        \brief Set current aspect ratio setting
        \param ratio current aspect ratio
    */
    void setCurrentAspectRatio(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Set default aspect ratio setting
        \param ratio default aspect ratio
    */
    void setDefaultAspectRatio(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Get current crop ratio setting
        \return current crop ratio
    */
    inline GoTvPtoP::Ratio currentCropRatio() const { return _currentCropRatio; }

    /*!
        \brief Get default crop ratio setting
        \return default crop ratio
    */
    inline GoTvPtoP::Ratio defaultCropRatio() const { return _defaultCropRatio; }

    /*!
        \brief Set current crop ratio setting
        \param ratio current crop ratio
    */
    void setCurrentCropRatio(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Set default crop ratio setting
        \param ratio default crop ratio
    */
    void setDefaultCropRatio(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Get current deinterlacing filter setting
        \return current deinterlacing filter
    */
    inline GoTvPtoP::Deinterlacing currentDeinterlacing() const { return _currentDeinterlacing; }

    /*!
        \brief Get default deinterlacing filter setting
        \return current deinterlacing filter
    */
    inline GoTvPtoP::Deinterlacing defaultDeinterlacing() const { return _defaultDeinterlacing; }

    /*!
        \brief Set current deinterlacing filter setting
        \param deinterlacing current deinterlacing filter
    */
    void setCurrentDeinterlacing(const GoTvPtoP::Deinterlacing &deinterlacing);

    /*!
        \brief Set default deinterlacing filter setting
        \param deinterlacing default deinterlacing filter
    */
    void setDefaultDeinterlacing(const GoTvPtoP::Deinterlacing &deinterlacing);

    /*!
        \brief Get current scale ratio setting
        \return current scale ratio
    */
    inline GoTvPtoP::Scale currentScale() const { return _currentScale; }

    /*!
        \brief Get default scale ratio setting
        \return default scale ratio
    */
    inline GoTvPtoP::Scale defaultScale() const { return _defaultScale; }

    /*!
        \brief Set current scale ratio setting
        \param scale current scale ratio
    */
    void setCurrentScale(const GoTvPtoP::Scale &scale);

    /*!
        \brief Set default scale ratio setting
        \param scale default scale ratio
    */
    void setDefaultScale(const GoTvPtoP::Scale &scale);

    /*!
        \brief Set media player if initialised without it
        \param player media player
    */
    void setMediaPlayer(GoTvPtoPMediaPlayer *player);

    /*!
        \brief Request new video frame.

        Request new video frame and its widget ID to set it in the instance.
        \return widget ID
        \sa GoTvPtoPMediaPlayer::GoTvPtoPMediaPlayer()
    */
    WId request();

    /*!
        \brief Release current video frame.
    */
    void release();

public slots:
    /*!
        \brief Enable default video settings

        crop, ratio, deinterlacing filter, scale
    */
    void enableDefaultSettings();

    /*!
        \brief Enable previous video settings

        crop, ratio, deinterlacing filter, scale
    */
    void enablePreviousSettings();

    /*!
        \brief Initialize default video settings

        Set but not apply crop, ratio, deinterlacing filter, scale
    */
    void initDefaultSettings();

    /*!
        \brief Set aspect ratio
        \param ratio aspect ratio
    */
    void setAspectRatio(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Set crop ratio
        \param ratio crop ratio
    */
    void setCropRatio(const GoTvPtoP::Ratio &ratio);

    /*!
        \brief Set deinterlace filter
        \param deinterlacing deinterlacing filter
    */
    void setDeinterlacing(const GoTvPtoP::Deinterlacing &deinterlacing);

    /*!
        \brief Set scale ratio
        \param scale scale ratio
    */
    void setScale(const GoTvPtoP::Scale &scale);

private slots:
    void applyPreviousSettings();

private:
    void initWidgetVideo();
    void sync();

    GoTvPtoPMediaPlayer *_gotvptopMediaPlayer;

    QWidget *_video;
    QLayout *_layout;

    bool _enableSettings;

    GoTvPtoP::Ratio _defaultAspectRatio;
    GoTvPtoP::Ratio _defaultCropRatio;
    GoTvPtoP::Deinterlacing _defaultDeinterlacing;
    GoTvPtoP::Scale _defaultScale;

    GoTvPtoP::Ratio _currentAspectRatio;
    GoTvPtoP::Ratio _currentCropRatio;
    GoTvPtoP::Deinterlacing _currentDeinterlacing;
    GoTvPtoP::Scale _currentScale;
};

#endif // GOTVQ_WIDGETVIDEO_H_
