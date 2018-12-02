/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>
#include <vector>

#include "FileItem.h"
#include "XBDateTime.h"
#include "guilib/GUIControl.h"
#include "guilib/GUIListItemLayout.h"
#include "guilib/IGUIContainer.h"

namespace PVR
{
  struct GridItem;
  class CGUIEPGGridContainerModel;

  class CGUIEPGGridContainer : public IGUIContainer
  {
  public:
    CGUIEPGGridContainer(int parentID, int controlID, float posX, float posY, float width, float height,
                         ORIENTATION orientation, int scrollTime, int preloadItems, int minutesPerPage,
                         int rulerUnit, const CTextureInfo& progressIndicatorTexture);
    CGUIEPGGridContainer(const CGUIEPGGridContainer &other);

    CGUIEPGGridContainer *Clone() const override { return new CGUIEPGGridContainer(*this); }

    /*!
     * @brief Check whether the control currently holds data.
     * @return true if the control has data, false otherwise.
     */
    bool HasData() const;

    void AllocResources() override;
    void FreeResources(bool immediately) override;

    bool OnAction(const CAction &action) override;
    void OnDown() override;
    void OnUp() override;
    void OnLeft() override;
    void OnRight() override;
    bool OnMouseOver(const CPoint &point) override;
    bool OnMessage(CGUIMessage& message) override;
    void SetFocus(bool focus) override;
    std::string GetDescription() const override;
    EVENT_RESULT OnMouseEvent(const CPoint &point, const CMouseEvent &event) override;

    void Process(unsigned int currentTime, CDirtyRegionList &dirtyregions) override;
    void Render() override;

    CGUIListItemPtr GetListItem(int offset, unsigned int flag = 0) const override;
    std::string GetLabel(int info) const override;

    CFileItemPtr GetSelectedChannelItem() const;
    PVR::CPVRChannelPtr GetSelectedChannel() const;
    CDateTime GetSelectedDate() const;

    void LoadLayout(TiXmlElement *layout);
    void SetPageControl(int id);

    /*! \brief Set the offset of the first item in the container from the container's position
     Useful for lists/panels where the focused item may be larger than the non-focused items and thus
     normally cut off from the clipping window defined by the container's position + size.
     \param offset CPoint holding the offset in skin coordinates.
     */
    void SetRenderOffset(const CPoint &offset);

    void GoToBegin();
    void GoToEnd();
    void GoToNow();
    void GoToDate(const CDateTime &date);
    void GoToTop();
    void GoToBottom();
    void GoToMostLeft();
    void GoToMostRight();

    void SetTimelineItems(const std::unique_ptr<CFileItemList> &items, const CDateTime &gridStart, const CDateTime &gridEnd);
    /*!
     * @brief Set the control's selection to the given channel and set the control's view port to show the channel.
     * @param channel the channel.
     * @return true if the selection was set to the given channel, false otherwise.
     */
    bool SetChannel(const PVR::CPVRChannelPtr &channel);
    /*!
     * @brief Set the control's selection to the given channel and set the control's view port to show the channel.
     * @param channel the channel's path.
     * @return true if the selection was set to the given channel, false otherwise.
     */
    bool SetChannel(const std::string &channel);

  protected:
    bool OnClick(int actionID);
    bool SelectItemFromPoint(const CPoint &point, bool justGrid = true);

    void SetChannel(int channel);
    void SetBlock(int block, bool bUpdateBlockTravelAxis = true);
    void ChannelScroll(int amount);
    void ProgrammesScroll(int amount);
    void ValidateOffset();
    void UpdateLayout();

    GridItem *GetItem(int channel);
    GridItem *GetNextItem(int channel);
    GridItem *GetPrevItem(int channel);

    int GetBlock(const CGUIListItemPtr &item, int channel);
    int GetRealBlock(const CGUIListItemPtr &item, int channel);
    void MoveToRow(int row);

    CGUIListItemLayout *GetFocusedLayout() const;

    void ScrollToBlockOffset(int offset);
    void ScrollToChannelOffset(int offset);
    void GoToBlock(int blockIndex);
    void GoToChannel(int channelIndex);
    void UpdateScrollOffset(unsigned int currentTime);
    void ProcessItem(float posX, float posY, const CFileItemPtr &item, CFileItemPtr &lastitem, bool focused, CGUIListItemLayout* normallayout, CGUIListItemLayout* focusedlayout, unsigned int currentTime, CDirtyRegionList &dirtyregions, float resize = -1.0f);
    void RenderItem(float posX, float posY, CGUIListItem *item, bool focused);
    void GetCurrentLayouts();

    void ProcessChannels(unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void ProcessRuler(unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void ProcessRulerDate(unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void ProcessProgrammeGrid(unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void ProcessProgressIndicator(unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void RenderChannels();
    void RenderRulerDate();
    void RenderRuler();
    void RenderProgrammeGrid();
    void RenderProgressIndicator();

    CPoint m_renderOffset; ///< \brief render offset of the first item in the list \sa SetRenderOffset

    ORIENTATION m_orientation;

    std::vector<CGUIListItemLayout> m_channelLayouts;
    std::vector<CGUIListItemLayout> m_focusedChannelLayouts;
    std::vector<CGUIListItemLayout> m_focusedProgrammeLayouts;
    std::vector<CGUIListItemLayout> m_programmeLayouts;
    std::vector<CGUIListItemLayout> m_rulerLayouts;
    std::vector<CGUIListItemLayout> m_rulerDateLayouts;

    CGUIListItemLayout *m_channelLayout;
    CGUIListItemLayout *m_focusedChannelLayout;
    CGUIListItemLayout *m_programmeLayout;
    CGUIListItemLayout *m_focusedProgrammeLayout;
    CGUIListItemLayout *m_rulerLayout;
    CGUIListItemLayout *m_rulerDateLayout;

    int m_pageControl;

    void GetChannelCacheOffsets(int &cacheBefore, int &cacheAfter);
    void GetProgrammeCacheOffsets(int &cacheBefore, int &cacheAfter);

  private:
    bool OnMouseClick(int dwButton, const CPoint &point);
    bool OnMouseDoubleClick(int dwButton, const CPoint &point);
    bool OnMouseWheel(char wheel, const CPoint &point);

    void HandleChannels(bool bRender, unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void HandleRuler(bool bRender, unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void HandleRulerDate(bool bRender, unsigned int currentTime, CDirtyRegionList &dirtyregions);
    void HandleProgrammeGrid(bool bRender, unsigned int currentTime, CDirtyRegionList &dirtyregions);

    float GetCurrentTimePositionOnPage() const;
    float GetProgressIndicatorWidth() const;
    float GetProgressIndicatorHeight() const;

    void UpdateItems();

    int GetSelectedItem() const;

    int m_rulerUnit; //! number of blocks that makes up one element of the ruler
    int m_channelsPerPage;
    int m_programmesPerPage;
    int m_channelCursor;
    int m_channelOffset;
    int m_blocksPerPage;
    int m_blockCursor;
    int m_blockOffset;
    int m_blockTravelAxis;
    int m_cacheChannelItems;
    int m_cacheProgrammeItems;
    int m_cacheRulerItems;

    float m_rulerDateHeight; //! height of ruler date item
    float m_rulerDateWidth; //! width of ruler date item
    float m_rulerPosX;      //! X position of first ruler item
    float m_rulerPosY;      //! Y position of first ruler item
    float m_rulerHeight;    //! height of the scrolling timeline above the ruler items
    float m_rulerWidth;     //! width of each element of the ruler
    float m_channelPosX;    //! X position of first channel row
    float m_channelPosY;    //! Y position of first channel row
    float m_channelHeight;  //! height of the channel item
    float m_channelWidth;   //! width of the channel item
    float m_gridPosX;       //! X position of first grid item
    float m_gridPosY;       //! Y position of first grid item
    float m_gridWidth;      //! width of the epg grid control
    float m_gridHeight;     //! height of the epg grid control
    float m_blockSize;      //! a block's width in pixels
    float m_analogScrollCount;

    CGUITexture m_guiProgressIndicatorTexture;

    CFileItemPtr m_lastItem;
    CFileItemPtr m_lastChannel;

    int m_scrollTime;

    int m_programmeScrollLastTime;
    float m_programmeScrollSpeed;
    float m_programmeScrollOffset;

    int m_channelScrollLastTime;
    float m_channelScrollSpeed;
    float m_channelScrollOffset;

    mutable CCriticalSection m_critSection;
    std::unique_ptr<CGUIEPGGridContainerModel> m_gridModel;
    std::unique_ptr<CGUIEPGGridContainerModel> m_updatedGridModel;

    GridItem *m_item;
  };
}
