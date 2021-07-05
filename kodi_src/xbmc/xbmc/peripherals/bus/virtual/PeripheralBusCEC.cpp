/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "config_kodi.h"

#if defined(HAVE_LIBCEC)
#include "PeripheralBusCEC.h"
#include "peripherals/Peripherals.h"
#include "DynamicDll.h"

#include <libcec/include/cec.h>

using namespace PERIPHERALS;
using namespace CEC;

class DllLibCECInterface
{
public:
  virtual ~DllLibCECInterface() = default;
  virtual ICECAdapter* CECInitialise(libcec_configuration *configuration)=0;
  virtual void*        CECDestroy(ICECAdapter *adapter)=0;
};

#if !USE_STATIC_LIBS

class PERIPHERALS::DllLibCEC : public DllDynamic, DllLibCECInterface
{
  DECLARE_DLL_WRAPPER(DllLibCEC, DLL_PATH_LIBCEC)

  DEFINE_METHOD1(ICECAdapter*, CECInitialise, (libcec_configuration *p1))
  DEFINE_METHOD1(void*       , CECDestroy,    (ICECAdapter *p1))

  BEGIN_METHOD_RESOLVE()
    RESOLVE_METHOD_RENAME(CECInitialise,  CECInitialise)
    RESOLVE_METHOD_RENAME(CECDestroy, CECDestroy)
  END_METHOD_RESOLVE()
};

#else
#include "StaticDll.h"

class PERIPHERALS::DllLibCEC : public StaticDll, public DllLibCECInterface
{
public:
    DllLibCEC();
    virtual ~DllLibCEC() = default;
    virtual ICECAdapter* CECInitialise( libcec_configuration *configuration );
    virtual void*        CECDestroy( ICECAdapter *adapter );
};

PERIPHERALS::DllLibCEC::DllLibCEC()
    : StaticDll( DLL_PATH_LIBCEC )
{
}

ICECAdapter* PERIPHERALS::DllLibCEC::CECInitialise( libcec_configuration *configuration )
{
    return ::CECInitialise( configuration );
}

void * PERIPHERALS::DllLibCEC::CECDestroy( ICECAdapter *adapter )
{
    ::CECDestroy( adapter );
    return this;
}

#endif // USE_STATIC_LIBS

CPeripheralBusCEC::CPeripheralBusCEC(CPeripherals& manager) :
    CPeripheralBus("PeripBusCEC", manager, PERIPHERAL_BUS_CEC),
    m_dll(new DllLibCEC),
    m_cecAdapter(NULL)
{
  m_iRescanTime = 5000;
  if (!m_dll->Load() || !m_dll->IsLoaded())
  {
    delete m_dll;
    m_dll = NULL;
  }
  else
  {
    m_cecAdapter = m_dll->CECInitialise(&m_configuration);
  }
}

CPeripheralBusCEC::~CPeripheralBusCEC(void)
{
  if (m_dll && m_cecAdapter)
    m_dll->CECDestroy(m_cecAdapter);
  delete m_dll;
}

bool CPeripheralBusCEC::PerformDeviceScan(PeripheralScanResults &results)
{
  if (!m_dll || !m_cecAdapter)
    return false;

  cec_adapter_descriptor deviceList[10];
  int8_t iFound = m_cecAdapter->DetectAdapters(deviceList, 10, NULL, true);

  for (uint8_t iDevicePtr = 0; iDevicePtr < iFound; iDevicePtr++)
  {
    PeripheralScanResult result(m_type);
    result.m_iVendorId   = deviceList[iDevicePtr].iVendorId;
    result.m_iProductId  = deviceList[iDevicePtr].iProductId;
    result.m_strLocation = deviceList[iDevicePtr].strComName;
    result.m_type        = PERIPHERAL_CEC;

    // override the bus type, so users don't have to reconfigure their adapters
    switch(deviceList[iDevicePtr].adapterType)
    {
    case ADAPTERTYPE_P8_EXTERNAL:
    case ADAPTERTYPE_P8_DAUGHTERBOARD:
      result.m_mappedBusType = PERIPHERAL_BUS_USB;
      break;
    case ADAPTERTYPE_RPI:
      result.m_mappedBusType = PERIPHERAL_BUS_RPI;
      /** the Pi's adapter cannot be removed, no need to rescan */
      m_bNeedsPolling = false;
      break;
    default:
      break;
    }

    result.m_iSequence = GetNumberOfPeripheralsWithId(result.m_iVendorId, result.m_iProductId);
    if (!results.ContainsResult(result))
      results.m_results.push_back(result);
  }
  return true;
}

#endif
