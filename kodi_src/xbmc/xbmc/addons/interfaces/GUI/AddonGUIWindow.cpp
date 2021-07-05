/*
 *  Copyright (C) 2015-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AddonGUIWindow.h"

#include "Application.h"
#include "FileItem.h"
#include "ServiceBroker.h"
#include "addons/Addon.h"
#include "guilib/GUIComponent.h"
#include "guilib/GUIWindowManager.h"
#include "guilib/TextureManager.h"
#include "input/Key.h"
#include "messaging/ApplicationMessenger.h"
#include "utils/log.h"
#include "utils/URIUtils.h"

#define CONTROL_BTNVIEWASICONS  2
#define CONTROL_BTNSORTBY       3
#define CONTROL_BTNSORTASC      4
#define CONTROL_LABELFILES      12

using namespace KODI::MESSAGING;
using namespace ADDON;

namespace KodiAPI
{
namespace GUI
{

CGUIAddonWindow::CGUIAddonWindow(int id, const std::string& strXML, CAddon* addon)
 : CGUIMediaWindow(id, strXML.c_str())
 , CBOnInit{nullptr}
 , CBOnFocus{nullptr}
 , CBOnClick{nullptr}
 , CBOnAction{nullptr}
 , m_clientHandle{nullptr}
 , m_iWindowId(id)
 , m_iOldWindowId(0)
 , m_bModal(false)
 , m_bIsDialog(false)
 , m_actionEvent(true)
 , m_addon(addon)

{
  m_loadType = LOAD_ON_GUI_INIT;
}

CGUIAddonWindow::~CGUIAddonWindow(void) = default;

bool CGUIAddonWindow::OnAction(const CAction &action)
{
  // Let addon decide whether it wants to handle action first
  if (CBOnAction && CBOnAction(m_clientHandle, action.GetID()))
    return true;

  return CGUIWindow::OnAction(action);
}

bool CGUIAddonWindow::OnMessage(CGUIMessage& message)
{
  //! @todo We shouldn't be dropping down to CGUIWindow in any of this ideally.
  //! We have to make up our minds about what python should be doing and
  //! what this side of things should be doing
  switch (message.GetMessage())
  {
    case GUI_MSG_WINDOW_DEINIT:
    {
      return CGUIMediaWindow::OnMessage(message);
    }
    break;

    case GUI_MSG_WINDOW_INIT:
    {
      CGUIMediaWindow::OnMessage(message);
      if (CBOnInit)
        CBOnInit(m_clientHandle);

      return true;
    }
    break;

    case GUI_MSG_SETFOCUS:
    {
      if (m_viewControl.HasControl(message.GetControlId()) && m_viewControl.GetCurrentControl() != message.GetControlId())
      {
        m_viewControl.SetFocused();
        return true;
      }
      // check if our focused control is one of our category buttons
      int iControl = message.GetControlId();
      if (CBOnFocus)
      {
        CBOnFocus(m_clientHandle, iControl);
      }
    }
    break;

    case GUI_MSG_FOCUSED:
    {
      if (HasID(message.GetSenderId()) && CBOnFocus)
      {
        CBOnFocus(m_clientHandle, message.GetControlId());
      }
    }
    break;

    case GUI_MSG_CLICKED:
    {
      int iControl=message.GetSenderId();
      // Handle Sort/View internally. Scripters shouldn't use ID 2, 3 or 4.
      if (iControl == CONTROL_BTNSORTASC) // sort asc
      {
        CLog::Log(LOGINFO, "WindowXML: Internal asc/dsc button not implemented");
        /*if (m_guiState.get())
          m_guiState->SetNextSortOrder();
        UpdateFileList();*/
        return true;
      }
      else if (iControl == CONTROL_BTNSORTBY) // sort by
      {
        CLog::Log(LOGINFO, "WindowXML: Internal sort button not implemented");
        /*if (m_guiState.get())
          m_guiState->SetNextSortMethod();
        UpdateFileList();*/
        return true;
      }

      if (CBOnClick && iControl && iControl != this->GetID())
      {
        CGUIControl* controlClicked = this->GetControl(iControl);

        // The old python way used to check list AND SELECITEM method or if its a button, radiobutton.
        // Its done this way for now to allow other controls without a python version like togglebutton to still raise a onAction event
        if (controlClicked) // Will get problems if we the id is not on the window and we try to do GetControlType on it. So check to make sure it exists
        {
          if ((controlClicked->IsContainer() && (message.GetParam1() == ACTION_SELECT_ITEM ||
                                                 message.GetParam1() == ACTION_MOUSE_LEFT_CLICK)) ||
                                                 !controlClicked->IsContainer())
          {
            if (CBOnClick(m_clientHandle, iControl))
              return true;
          }
          else if (controlClicked->IsContainer() && message.GetParam1() == ACTION_MOUSE_RIGHT_CLICK)
          {
//            PyXBMCAction* inf = new PyXBMCAction;
//            inf->pObject = Action_FromAction(CAction(ACTION_CONTEXT_MENU));
//            inf->pCallbackWindow = pCallbackWindow;
//
//            // acquire lock?
//            PyXBMC_AddPendingCall(Py_XBMC_Event_OnAction, inf);
//            PulseActionEvent();
          }
        }
      }
    }
    break;
  }

  return CGUIMediaWindow::OnMessage(message);
}

void CGUIAddonWindow::AllocResources(bool forceLoad /*= false */)
{
  std::string tmpDir = URIUtils::GetDirectory(GetProperty("xmlfile").asString());
  std::string fallbackMediaPath;
  URIUtils::GetParentPath(tmpDir, fallbackMediaPath);
  URIUtils::RemoveSlashAtEnd(fallbackMediaPath);
  m_mediaDir = fallbackMediaPath;

  //CLog::Log(LOGDEBUG, "CGUIPythonWindowXML::AllocResources called: %s", fallbackMediaPath.c_str());
  CServiceBroker::GetGUI()->GetTextureManager().AddTexturePath(m_mediaDir);
  CGUIMediaWindow::AllocResources(forceLoad);
  CServiceBroker::GetGUI()->GetTextureManager().RemoveTexturePath(m_mediaDir);
}

void CGUIAddonWindow::FreeResources(bool forceUnLoad /*= false */)
{
  CGUIMediaWindow::FreeResources(forceUnLoad);
}

void CGUIAddonWindow::Render()
{
  CServiceBroker::GetGUI()->GetTextureManager().AddTexturePath(m_mediaDir);
  CGUIMediaWindow::Render();
  CServiceBroker::GetGUI()->GetTextureManager().RemoveTexturePath(m_mediaDir);
}

void CGUIAddonWindow::Update()
{
}

void CGUIAddonWindow::AddItem(CFileItemPtr fileItem, int itemPosition)
{
  if (itemPosition == -1 || itemPosition > m_vecItems->Size())
  {
    m_vecItems->Add(fileItem);
  }
  else if (itemPosition <  -1 &&  !(itemPosition-1 < m_vecItems->Size()))
  {
    m_vecItems->AddFront(fileItem,0);
  }
  else
  {
    m_vecItems->AddFront(fileItem,itemPosition);
  }
  m_viewControl.SetItems(*m_vecItems);
  UpdateButtons();
}

void CGUIAddonWindow::RemoveItem(int itemPosition)
{
  m_vecItems->Remove(itemPosition);
  m_viewControl.SetItems(*m_vecItems);
  UpdateButtons();
}

int CGUIAddonWindow::GetCurrentListPosition()
{
  return m_viewControl.GetSelectedItem();
}

void CGUIAddonWindow::SetCurrentListPosition(int item)
{
  m_viewControl.SetSelectedItem(item);
}

int CGUIAddonWindow::GetListSize()
{
  return m_vecItems->Size();
}

CFileItemPtr CGUIAddonWindow::GetListItem(int position)
{
  if (position < 0 || position >= m_vecItems->Size()) return CFileItemPtr();
  return m_vecItems->Get(position);
}

void CGUIAddonWindow::ClearList()
{
  ClearFileItems();

  m_viewControl.SetItems(*m_vecItems);
  UpdateButtons();
}

void CGUIAddonWindow::GetContextButtons(int itemNumber, CContextButtons &buttons)
{
  // maybe on day we can make an easy way to do this context menu
  // with out this method overriding the MediaWindow version, it will display 'Add to Favourites'
}

void CGUIAddonWindow::WaitForActionEvent(unsigned int timeout)
{
  m_actionEvent.WaitMSec(timeout);
  m_actionEvent.Reset();
}

void CGUIAddonWindow::PulseActionEvent()
{
  m_actionEvent.Set();
}

bool CGUIAddonWindow::OnClick(int iItem, const std::string &player)
{
  // Hook Over calling  CGUIMediaWindow::OnClick(iItem) results in it trying to PLAY the file item
  // which if its not media is BAD and 99 out of 100 times undesirable.
  return false;
}

// SetupShares();
/*
 CGUIMediaWindow::OnWindowLoaded() calls SetupShares() so override it
and just call UpdateButtons();
*/
void CGUIAddonWindow::SetupShares()
{
  UpdateButtons();
}

CGUIAddonWindowDialog::CGUIAddonWindowDialog(int id, const std::string& strXML, CAddon* addon)
: CGUIAddonWindow(id,strXML,addon)
{
  m_bRunning = false;
  m_bIsDialog = true;
}

CGUIAddonWindowDialog::~CGUIAddonWindowDialog(void) = default;

bool CGUIAddonWindowDialog::OnMessage(CGUIMessage &message)
{
  if (message.GetMessage() == GUI_MSG_WINDOW_DEINIT)
    return CGUIWindow::OnMessage(message);

  return CGUIAddonWindow::OnMessage(message);
}

void CGUIAddonWindowDialog::Show(bool show /* = true */)
{
  unsigned int iCount = CServiceBroker::GetWinSystem()->GetGfxContext().exit();
  CApplicationMessenger::GetInstance().SendMsg(TMSG_GUI_ADDON_DIALOG, 1, show ? 1 : 0, static_cast<void*>(this));
  CServiceBroker::GetWinSystem()->GetGfxContext().restore(iCount);
}

void CGUIAddonWindowDialog::Show_Internal(bool show /* = true */)
{
  if (show)
  {
    m_bModal = true;
    m_bRunning = true;
    CServiceBroker::GetGUI()->GetWindowManager().RegisterDialog(this);

    // active this window...
    CGUIMessage msg(GUI_MSG_WINDOW_INIT, 0, 0, WINDOW_INVALID, m_iWindowId);
    OnMessage(msg);

    // this dialog is derived from GUiMediaWindow
    // make sure it is rendered last
    m_renderOrder = RENDER_ORDER_DIALOG;
    while (m_bRunning)
    {
      if (!ProcessRenderLoop(false))
        break;
    }
  }
  else // hide
  {
    m_bRunning = false;

    CGUIMessage msg(GUI_MSG_WINDOW_DEINIT,0,0);
    OnMessage(msg);

    CServiceBroker::GetGUI()->GetWindowManager().RemoveDialog(GetID());
  }
}

} /* namespace GUI */
} /* namespace KodiAPI */
