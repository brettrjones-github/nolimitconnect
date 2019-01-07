/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "BinaryAddonManager.h"
#include "DllAddon.h"
#include "addons/Addon.h"
#include "addons/interfaces/AddonInterfaces.h"
#include "utils/XMLUtils.h"

namespace ADDON
{

  typedef void* (*ADDON_GET_INTERFACE_FN)(const std::string &name, const std::string &version);

  class CAddonDll : public CAddon
  {
  public:
    CAddonDll(CAddonInfo addonInfo, BinaryAddonBasePtr addonBase);
    explicit CAddonDll(CAddonInfo addonInfo);
    ~CAddonDll() override;

    virtual ADDON_STATUS GetStatus();

    static void RegisterInterface(ADDON_GET_INTERFACE_FN fn);

    // Implementation of IAddon via CAddon
    std::string LibPath() const override;

    // addon settings
    void SaveSettings() override;
    std::string GetSetting(const std::string& key) override;

    ADDON_STATUS Create(ADDON_TYPE type, void* funcTable, void* info);
    void Destroy();

    bool DllLoaded(void) const;

    /*!
     * @brief Function to create a addon instance class
     *
     * @param[in] instanceType The wanted instance type class to open on addon
     * @param[in] instanceID The from Kodi used ID string of active instance
     * @param[in] instance Pointer where the interface functions from addon
     *                     becomes stored during his instance creation.
     * @param[in] parentInstance In case the instance class is related to another
     *                           addon instance class becomes with the pointer
     *                           given to addon. Is optional and most addon types
     *                           not use it.
     * @return The status of addon after the creation.
     */
    ADDON_STATUS CreateInstance(ADDON_TYPE instanceType, const std::string& instanceID, KODI_HANDLE instance, KODI_HANDLE parentInstance = nullptr);

    /*!
     * @brief Function to destroy a on addon created instance class
     *
     * @param[in] instanceID The from Kodi used ID string of active instance
     */
    void DestroyInstance(const std::string& instanceID);

    AddonPtr GetRunningInstance() const override;

    bool Initialized() const { return m_initialized; }

  protected:
    static std::string GetDllPath(const std::string &strFileName);

    CAddonInterfaces* m_pHelpers;
    std::string m_parentLib;

  private:
    /*!
     * @brief Main addon creation call function
     *
     * This becomes called only one time before a addon instance becomes created.
     * If another instance becomes requested is this Create no more used. To see
     * like a "int main()" on exe.
     *
     * @param[in] firstKodiInstance The first instance who becomes used.
     *                              In case addon supports only one instance
     *                              and not multiple together can addon use
     *                              only one complete class for everything.
     *                              This is used then to interact on interface.
     * @return The status of addon after the creation.
     */
    ADDON_STATUS Create(KODI_HANDLE firstKodiInstance);

    bool CheckAPIVersion(int type);

    BinaryAddonBasePtr m_binaryAddonBase;
    DllAddon* m_pDll;
    bool m_initialized;
    bool LoadDll();
    std::map<std::string, std::pair<ADDON_TYPE, KODI_HANDLE>> m_usedInstances;

    virtual ADDON_STATUS TransferSettings();

    bool UpdateSettingInActiveDialog(const char* id, const std::string& value);

    static std::vector<ADDON_GET_INTERFACE_FN> s_registeredInterfaces;

    /// addon to kodi basic callbacks below
    //@{

    /*!
     * This structure, which is fixed to the addon headers, makes use of the at
     * least supposed parts for the interface.
     * This structure is defined in:
     * /xbmc/addons/kodi-addon-dev-kit/include/kodi/AddonBase.h
     */
    AddonGlobalInterface m_interface;

    inline bool InitInterface(KODI_HANDLE firstKodiInstance);
    inline void DeInitInterface();

    static char* get_addon_path(void* kodiBase);
    static char* get_base_user_path(void* kodiBase);
    static void addon_log_msg(void* kodiBase, const int addonLogLevel, const char* strMessage);
    static bool get_setting_bool(void* kodiBase, const char* id, bool* value);
    static bool get_setting_int(void* kodiBase, const char* id, int* value);
    static bool get_setting_float(void* kodiBase, const char* id, float* value);
    static bool get_setting_string(void* kodiBase, const char* id, char** value);
    static bool set_setting_bool(void* kodiBase, const char* id, bool value);
    static bool set_setting_int(void* kodiBase, const char* id, int value);
    static bool set_setting_float(void* kodiBase, const char* id, float value);
    static bool set_setting_string(void* kodiBase, const char* id, const char* value);
    static void free_string(void* kodiBase, char* str);
    static void free_string_array(void* kodiBase, char** arr, int numElements);
    static void* get_interface(void* kodiBase, const char* name, const char *version);
    //@}
  };

}; /* namespace ADDON */

