//============================================================================
// Copyright (C) 2014 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#ifdef TARGET_OS_WINDOWS

#include "Firewall.h"

#include <CoreLib/AppErr.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileUtil.h>

namespace
{
	uint16_t				RC_DEFAULT_MULTICAST_PORT	= 45124;
}

//============================================================================
Firewall::Firewall()
{
}

//============================================================================
Firewall::~Firewall()
{
}

/*
    Copyright (c) Microsoft Corporation

    SYNOPSIS

        Sample code for the Windows Firewall COM interface.
*/

#include <WinSock2.h>
#include <windows.h>
#include <crtdbg.h>
#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>
#include <comutil.h>
#include <atlcomcli.h>

#include <stdio.h>

#pragma comment( lib, "ole32.lib" )
#pragma comment( lib, "oleaut32.lib" )

namespace
{
	enum EProtocol
	{
		eProtocolTcp,
		eProtocolUdp
	};
}

//============================================================================
HRESULT WindowsFirewallInitialize(OUT INetFwProfile** fwProfile, int i )
{
    HRESULT hr = S_OK;
    INetFwMgr* fwMgr = NULL;
    INetFwPolicy* fwPolicy = NULL;

    _ASSERT(fwProfile != NULL);

    *fwProfile = NULL;

    // Create an instance of the firewall settings manager.
    hr = CoCreateInstance(
            __uuidof(NetFwMgr),
            NULL,
            CLSCTX_INPROC_SERVER,
            __uuidof(INetFwMgr),
            (void**)&fwMgr
            );
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "CoCreateInstance failed: 0x%08lx\n", hr);
        goto error;
    }

    // Retrieve the local firewall policy.
    hr = fwMgr->get_LocalPolicy(&fwPolicy);
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "get_LocalPolicy failed: 0x%08lx\n", hr);
        goto error;
    }

    // Retrieve the firewall profile currently in effect.
	//hr = fwPolicy->get_CurrentProfile(fwProfile);
	if( i )
	{
		hr = fwPolicy->GetProfileByType( NET_FW_PROFILE_DOMAIN, fwProfile);
	}
	else
	{
		hr = fwPolicy->GetProfileByType( NET_FW_PROFILE_STANDARD, fwProfile);
	}
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "get_CurrentProfile failed: 0x%08lx\n", hr);
        goto error;
    }

error:

    // Release the local firewall policy.
    if (fwPolicy != NULL)
    {
        fwPolicy->Release();
    }

    // Release the firewall settings manager.
    if (fwMgr != NULL)
    {
        fwMgr->Release();
    }

    return hr;
}

//============================================================================
void WindowsFirewallCleanup(IN INetFwProfile* fwProfile)
{
    // Release the firewall profile.
    if (fwProfile != NULL)
    {
        fwProfile->Release();
    }
}

//============================================================================
HRESULT WindowsFirewallIsOn(IN INetFwProfile* fwProfile, OUT BOOL* fwOn)
{
    HRESULT hr = S_OK;
    VARIANT_BOOL fwEnabled;

    _ASSERT(fwProfile != NULL);
    _ASSERT(fwOn != NULL);

    *fwOn = FALSE;

    // Get the current state of the firewall.
    hr = fwProfile->get_FirewallEnabled(&fwEnabled);
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "get_FirewallEnabled failed: 0x%08lx\n", hr);
        goto error;
    }

    // Check to see if the firewall is on.
    if (fwEnabled != VARIANT_FALSE)
    {
        *fwOn = TRUE;
        LogMsg( LOG_DEBUG, "The firewall is on.\n");
    }
    else
    {
        LogMsg( LOG_DEBUG, "The firewall is off.\n");
    }

error:

    return hr;
}

//============================================================================
HRESULT WindowsFirewallTurnOn(IN INetFwProfile* fwProfile)
{
    HRESULT hr = S_OK;
    BOOL fwOn;

    _ASSERT(fwProfile != NULL);

    // Check to see if the firewall is off.
    hr = WindowsFirewallIsOn(fwProfile, &fwOn);
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "WindowsFirewallIsOn failed: 0x%08lx\n", hr);
        goto error;
    }

    // If it is, turn it on.
    if (!fwOn)
    {
        // Turn the firewall on.
        hr = fwProfile->put_FirewallEnabled(VARIANT_TRUE);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "put_FirewallEnabled failed: 0x%08lx\n", hr);
            goto error;
        }

        LogMsg( LOG_DEBUG, "The firewall is now on.\n");
    }

error:

    return hr;
}

//============================================================================
HRESULT WindowsFirewallTurnOff(IN INetFwProfile* fwProfile)
{
    HRESULT hr = S_OK;
    BOOL fwOn;

    _ASSERT(fwProfile != NULL);

    // Check to see if the firewall is on.
    hr = WindowsFirewallIsOn(fwProfile, &fwOn);
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "WindowsFirewallIsOn failed: 0x%08lx\n", hr);
        goto error;
    }

    // If it is, turn it off.
    if (fwOn)
    {
        // Turn the firewall off.
        hr = fwProfile->put_FirewallEnabled(VARIANT_FALSE);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "put_FirewallEnabled failed: 0x%08lx\n", hr);
            goto error;
        }

        LogMsg( LOG_DEBUG, "The firewall is now off.\n");
    }

error:

    return hr;
}

//============================================================================
HRESULT WindowsFirewallAppIsEnabled(
            IN INetFwProfile* fwProfile,
            IN const wchar_t* fwProcessImageFileName,
            OUT BOOL* fwAppEnabled
            )
{
    HRESULT hr = S_OK;
    BSTR fwBstrProcessImageFileName = NULL;
    VARIANT_BOOL fwEnabled;
    INetFwAuthorizedApplication* fwApp = NULL;
    INetFwAuthorizedApplications* fwApps = NULL;

    _ASSERT(fwProfile != NULL);
    _ASSERT(fwProcessImageFileName != NULL);
    _ASSERT(fwAppEnabled != NULL);

    *fwAppEnabled = FALSE;

    // Retrieve the authorized application collection.
    hr = fwProfile->get_AuthorizedApplications(&fwApps);
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "get_AuthorizedApplications failed: 0x%08lx\n", hr);
        goto error;
    }

    // Allocate a BSTR for the process image file name.
    fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
    if (fwBstrProcessImageFileName == NULL)
    {
        hr = E_OUTOFMEMORY;
        LogMsg( LOG_DEBUG, "SysAllocString failed: 0x%08lx\n", hr);
        goto error;
    }

    // Attempt to retrieve the authorized application.
    hr = fwApps->Item(fwBstrProcessImageFileName, &fwApp);
    if (SUCCEEDED(hr))
    {
        // Find out if the authorized application is enabled.
        hr = fwApp->get_Enabled(&fwEnabled);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "get_Enabled failed: 0x%08lx\n", hr);
            goto error;
        }

        if (fwEnabled != VARIANT_FALSE)
        {
            // The authorized application is enabled.
            *fwAppEnabled = TRUE;

            LogMsg( LOG_DEBUG, 
                "Authorized application %lS is enabled in the firewall.\n",
                fwProcessImageFileName
                );
        }
        else
        {
            LogMsg( LOG_DEBUG, 
                "Authorized application %lS is disabled in the firewall.\n",
                fwProcessImageFileName
                );
        }
    }
    else
    {
        // The authorized application was not in the collection.
        hr = S_OK;

        LogMsg( LOG_DEBUG, 
            "Authorized application %lS is disabled in the firewall.\n",
            fwProcessImageFileName
            );
    }

error:

    // Free the BSTR.
    SysFreeString(fwBstrProcessImageFileName);

    // Release the authorized application instance.
    if (fwApp != NULL)
    {
        fwApp->Release();
    }

    // Release the authorized application collection.
    if (fwApps != NULL)
    {
        fwApps->Release();
    }

    return hr;
}

//============================================================================
HRESULT WindowsFirewallAddApp(
            IN INetFwProfile* fwProfile,
            IN const wchar_t* fwProcessImageFileName,
            IN const wchar_t* fwName
            )
{
    HRESULT hr = S_OK;
    BOOL fwAppEnabled;
    BSTR fwBstrName = NULL;
    BSTR fwBstrProcessImageFileName = NULL;
    INetFwAuthorizedApplication* fwApp = NULL;
    INetFwAuthorizedApplications* fwApps = NULL;

    _ASSERT(fwProfile != NULL);
    _ASSERT(fwProcessImageFileName != NULL);
    _ASSERT(fwName != NULL);

    // First check to see if the application is already authorized.
    hr = WindowsFirewallAppIsEnabled(
            fwProfile,
            fwProcessImageFileName,
            &fwAppEnabled
            );
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "WindowsFirewallAppIsEnabled failed: 0x%08lx\n", hr);
        goto error;
    }

    // Only add the application if it isn't already authorized.
    if (!fwAppEnabled)
    {
        // Retrieve the authorized application collection.
        hr = fwProfile->get_AuthorizedApplications(&fwApps);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "get_AuthorizedApplications failed: 0x%08lx\n", hr);
            goto error;
        }

        // Create an instance of an authorized application.
        hr = CoCreateInstance(
                __uuidof(NetFwAuthorizedApplication),
                NULL,
                CLSCTX_INPROC_SERVER,
                __uuidof(INetFwAuthorizedApplication),
                (void**)&fwApp
                );
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "CoCreateInstance failed: 0x%08lx\n", hr);
            goto error;
        }

        // Allocate a BSTR for the process image file name.
        fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
        if (fwBstrProcessImageFileName == NULL)
        {
            hr = E_OUTOFMEMORY;
            LogMsg( LOG_DEBUG, "SysAllocString failed: 0x%08lx\n", hr);
            goto error;
        }

        // Set the process image file name.
        hr = fwApp->put_ProcessImageFileName(fwBstrProcessImageFileName);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "put_ProcessImageFileName failed: 0x%08lx\n", hr);
            goto error;
        }

        // Allocate a BSTR for the application friendly name.
        fwBstrName = SysAllocString(fwName);
        if (SysStringLen(fwBstrName) == 0)
        {
            hr = E_OUTOFMEMORY;
            LogMsg( LOG_DEBUG, "SysAllocString failed: 0x%08lx\n", hr);
            goto error;
        }

        // Set the application friendly name.
        hr = fwApp->put_Name(fwBstrName);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "put_Name failed: 0x%08lx\n", hr);
            goto error;
        }

        // Add the application to the collection.
        hr = fwApps->Add(fwApp);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "Add failed: 0x%08lx\n", hr);
            goto error;
        }

        LogMsg( LOG_DEBUG, 
            "Authorized application %lS is now enabled in the firewall.\n",
            fwProcessImageFileName
            );
    }

error:

    // Free the BSTRs.
    SysFreeString(fwBstrName);
    SysFreeString(fwBstrProcessImageFileName);

    // Release the authorized application instance.
    if (fwApp != NULL)
    {
        fwApp->Release();
    }

    // Release the authorized application collection.
    if (fwApps != NULL)
    {
        fwApps->Release();
    }

    return hr;
}

//============================================================================
HRESULT WindowsFirewallPortIsEnabled(
            IN INetFwProfile* fwProfile,
            IN LONG portNumber,
            IN NET_FW_IP_PROTOCOL ipProtocol,
            OUT BOOL* fwPortEnabled
            )
{
    HRESULT hr = S_OK;
    VARIANT_BOOL fwEnabled;
    INetFwOpenPort* fwOpenPort = NULL;
    INetFwOpenPorts* fwOpenPorts = NULL;

    _ASSERT(fwProfile != NULL);
    _ASSERT(fwPortEnabled != NULL);

    *fwPortEnabled = FALSE;

    // Retrieve the globally open ports collection.
    hr = fwProfile->get_GloballyOpenPorts(&fwOpenPorts);
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "get_GloballyOpenPorts failed: 0x%08lx\n", hr);
        goto error;
    }

    // Attempt to retrieve the globally open port.
    hr = fwOpenPorts->Item(portNumber, ipProtocol, &fwOpenPort);
    if (SUCCEEDED(hr))
    {
        // Find out if the globally open port is enabled.
        hr = fwOpenPort->get_Enabled(&fwEnabled);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "get_Enabled failed: 0x%08lx\n", hr);
            goto error;
        }

        if (fwEnabled != VARIANT_FALSE)
        {
            // The globally open port is enabled.
            *fwPortEnabled = TRUE;

            LogMsg( LOG_DEBUG, "Port %ld is open in the firewall.\n", portNumber);
        }
        else
        {
            LogMsg( LOG_DEBUG, "Port %ld is not open in the firewall.\n", portNumber);
        }
    }
    else
    {
        // The globally open port was not in the collection.
        hr = S_OK;

        LogMsg( LOG_DEBUG, "Port %ld is not open in the firewall.\n", portNumber);
    }

error:

    // Release the globally open port.
    if (fwOpenPort != NULL)
    {
        fwOpenPort->Release();
    }

    // Release the globally open ports collection.
    if (fwOpenPorts != NULL)
    {
        fwOpenPorts->Release();
    }

    return hr;
}

//============================================================================
HRESULT WindowsFirewallPortAdd(
            IN INetFwProfile* fwProfile,
            IN LONG portNumber,
            IN NET_FW_IP_PROTOCOL ipProtocol,
            IN const wchar_t* name
            )
{
    HRESULT hr = S_OK;
    BOOL fwPortEnabled;
    BSTR fwBstrName = NULL;
    INetFwOpenPort* fwOpenPort = NULL;
    INetFwOpenPorts* fwOpenPorts = NULL;

    _ASSERT(fwProfile != NULL);
    _ASSERT(name != NULL);

    // First check to see if the port is already added.
    hr = WindowsFirewallPortIsEnabled(
            fwProfile,
            portNumber,
            ipProtocol,
            &fwPortEnabled
            );
    if (FAILED(hr))
    {
        LogMsg( LOG_DEBUG, "WindowsFirewallPortIsEnabled failed: 0x%08lx\n", hr);
        goto error;
    }

    // Only add the port if it isn't already added.
    if (!fwPortEnabled)
    {
        // Retrieve the collection of globally open ports.
        hr = fwProfile->get_GloballyOpenPorts(&fwOpenPorts);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "get_GloballyOpenPorts failed: 0x%08lx\n", hr);
            goto error;
        }

        // Create an instance of an open port.
        hr = CoCreateInstance(
                __uuidof(NetFwOpenPort),
                NULL,
                CLSCTX_INPROC_SERVER,
                __uuidof(INetFwOpenPort),
                (void**)&fwOpenPort
                );
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "CoCreateInstance failed: 0x%08lx\n", hr);
            goto error;
        }

        // Set the port number.
        hr = fwOpenPort->put_Port(portNumber);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "put_Port failed: 0x%08lx\n", hr);
            goto error;
        }

        // Set the IP protocol.
        hr = fwOpenPort->put_Protocol(ipProtocol);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "put_Protocol failed: 0x%08lx\n", hr);
            goto error;
        }

        // Allocate a BSTR for the friendly name of the port.
        fwBstrName = SysAllocString(name);
        if (SysStringLen(fwBstrName) == 0)
        {
            hr = E_OUTOFMEMORY;
            LogMsg( LOG_DEBUG, "SysAllocString failed: 0x%08lx\n", hr);
            goto error;
        }

        // Set the friendly name of the port.
        hr = fwOpenPort->put_Name(fwBstrName);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "put_Name failed: 0x%08lx\n", hr);
            goto error;
        }

        // Opens the port and adds it to the collection.
        hr = fwOpenPorts->Add(fwOpenPort);
        if (FAILED(hr))
        {
            LogMsg( LOG_DEBUG, "Add failed: 0x%08lx\n", hr);
            goto error;
        }

        LogMsg( LOG_DEBUG, "Port %ld is now open in the firewall.\n", portNumber);
    }

error:

    // Free the BSTR.
    SysFreeString(fwBstrName);

    // Release the open port instance.
    if (fwOpenPort != NULL)
    {
        fwOpenPort->Release();
    }

    // Release the globally open ports collection.
    if (fwOpenPorts != NULL)
    {
        fwOpenPorts->Release();
    }

    return hr;
}

// Forward declarations
HRESULT     WFCOMInitialize_FirewallLib(INetFwPolicy2** ppNetFwPolicy2);
//============================================================================
//============================================================================
//============================================================================
#define NET_FW_IP_PROTOCOL_TCP_NAME L"TCP"
#define NET_FW_IP_PROTOCOL_UDP_NAME L"UDP"

#define NET_FW_RULE_DIR_IN_NAME L"In"
#define NET_FW_RULE_DIR_OUT_NAME L"Out"

#define NET_FW_RULE_ACTION_BLOCK_NAME L"Block"
#define NET_FW_RULE_ACTION_ALLOW_NAME L"Allow"

#define NET_FW_RULE_ENABLE_IN_NAME L"TRUE"
#define NET_FW_RULE_DISABLE_IN_NAME L"FALSE"


// Forward declarations
void DumpFWRulesInCollection(INetFwRule* FwRule,  wchar_t * ruleName, wchar_t * exePath, uint16_t u16Port, bool& ruleExists );


bool RuleExists( wchar_t * ruleName, wchar_t * exePath, uint16_t u16Port )
{
	bool ruleExists = false;

	HRESULT hrComInit = S_OK;
	HRESULT hr = S_OK;

	ULONG cFetched = 0; 
	CComVariant var;

	IUnknown *pEnumerator;
	IEnumVARIANT* pVariant = NULL;

	INetFwPolicy2 *pNetFwPolicy2 = NULL;
	INetFwRules *pFwRules = NULL;
	INetFwRule *pFwRule = NULL;

	long fwRuleCount;

	// Initialize COM.
	hrComInit = CoInitializeEx(
		0,
		COINIT_APARTMENTTHREADED
		);

	// Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
	// initialized with a different mode. Since we don't care what the mode is,
	// we'll just use the existing mode.
	if (hrComInit != RPC_E_CHANGED_MODE)
	{
		if (FAILED(hrComInit))
		{
			wprintf(L"CoInitializeEx failed: 0x%08lx\n", hrComInit);
			goto Cleanup;
		}
	}

	// Retrieve INetFwPolicy2
	hr = WFCOMInitialize_FirewallLib(&pNetFwPolicy2);
	if (FAILED(hr))
	{
		goto Cleanup;
	}

	// Retrieve INetFwRules
	hr = pNetFwPolicy2->get_Rules(&pFwRules);
	if (FAILED(hr))
	{
		wprintf(L"get_Rules failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	// Obtain the number of Firewall rules
	hr = pFwRules->get_Count(&fwRuleCount);
	if (FAILED(hr))
	{
		wprintf(L"get_Count failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	wprintf(L"The number of rules in the Windows Firewall are %d\n", fwRuleCount);

	// Iterate through all of the rules in pFwRules
	pFwRules->get__NewEnum(&pEnumerator);

	if(pEnumerator)
	{
		hr = pEnumerator->QueryInterface(__uuidof(IEnumVARIANT), (void **) &pVariant);
	}

	while(SUCCEEDED(hr) && hr != S_FALSE)
	{
		var.Clear();
		hr = pVariant->Next(1, &var, &cFetched);

		if (S_FALSE != hr)
		{
			if (SUCCEEDED(hr))
			{
				hr = var.ChangeType(VT_DISPATCH);
			}
			if (SUCCEEDED(hr))
			{
				hr = (V_DISPATCH(&var))->QueryInterface(__uuidof(INetFwRule), reinterpret_cast<void**>(&pFwRule));
			}

			if (SUCCEEDED(hr))
			{
				// Output the properties of this rule
				DumpFWRulesInCollection( pFwRule, ruleName, exePath,  u16Port, ruleExists );
				if( ruleExists )
				{
					break;
				}
			}
		}
	}

Cleanup:

	// Release pFwRule
	if (pFwRule != NULL)
	{
		pFwRule->Release();
	}

	// Release INetFwPolicy2
	if (pNetFwPolicy2 != NULL)
	{
		pNetFwPolicy2->Release();
	}

	// Uninitialize COM.
	if (SUCCEEDED(hrComInit))
	{
		CoUninitialize();
	}

	return ruleExists;
}


// Output properties of a Firewall rule 
void DumpFWRulesInCollection(INetFwRule* FwRule,  wchar_t * ruleName, wchar_t * exePath, uint16_t u16Port, bool& ruleExists )
{
	wchar_t  portBuf[ 32 ];
	swprintf( portBuf, sizeof(portBuf) / sizeof(*portBuf), L"%d", u16Port );

	std::wstring wstrPort = portBuf;
	std::wstring wstrRuleName = ruleName;
	std::wstring wstrExePath = exePath;

	std::wstring wstrValue;

	variant_t InterfaceArray;
	variant_t InterfaceString;  

	//VARIANT_BOOL bEnabled;
	BSTR bstrVal;

	long lVal = 0;
	long lProfileBitmask = 0;

	//NET_FW_RULE_DIRECTION fwDirection;
	//NET_FW_ACTION fwAction;

	struct ProfileMapElement 
	{
		NET_FW_PROFILE_TYPE2 Id;
		LPCWSTR Name;
	};

	ProfileMapElement ProfileMap[3];
	ProfileMap[0].Id = NET_FW_PROFILE2_DOMAIN;
	ProfileMap[0].Name = L"Domain";
	ProfileMap[1].Id = NET_FW_PROFILE2_PRIVATE;
	ProfileMap[1].Name = L"Private";
	ProfileMap[2].Id = NET_FW_PROFILE2_PUBLIC;
	ProfileMap[2].Name = L"Public";

	//wprintf(L"---------------------------------------------\n");

	if (SUCCEEDED(FwRule->get_Name(&bstrVal)))
	{
		wprintf(L"Name:             %s\n", bstrVal);
		wstrValue = bstrVal;
		if( wstrValue != wstrRuleName )
		{
			return;
		}
	}
	else
	{
		return;
	}

	//if (SUCCEEDED(FwRule->get_Description(&bstrVal)))
	//{
	//	wprintf(L"Description:      %s\n", bstrVal);
	//}

	if (SUCCEEDED(FwRule->get_ApplicationName(&bstrVal)))
	{
		//wprintf(L"Application Name: %s\n", bstrVal);
		wstrValue = bstrVal;
		if( wstrValue != wstrExePath )
		{
			return;
		}
	}
	else
	{
		return;
	}

	//if (SUCCEEDED(FwRule->get_ServiceName(&bstrVal)))
	//{
	//	//wprintf(L"Service Name:     %s\n", bstrVal);
	//}

	if (SUCCEEDED(FwRule->get_Protocol(&lVal)))
	{
		switch(lVal)
		{
		case NET_FW_IP_PROTOCOL_TCP: 

			//wprintf(L"IP Protocol:      %s\n", NET_FW_IP_PROTOCOL_TCP_NAME);
			break;

		case NET_FW_IP_PROTOCOL_UDP: 

			//wprintf(L"IP Protocol:      %s\n", NET_FW_IP_PROTOCOL_UDP_NAME);
			break;

		default:

			break;
		}

		if(lVal != NET_FW_IP_VERSION_V4 && lVal != NET_FW_IP_VERSION_V6)
		{
			if (SUCCEEDED(FwRule->get_LocalPorts(&bstrVal)))
			{
				//wprintf(L"Local Ports:      %s\n", bstrVal);
				wstrValue = bstrVal;
				if( wstrValue != wstrPort )
				{
					return;
				}
				else
				{
					ruleExists = true;
					return;
				}
			}

			//if (SUCCEEDED(FwRule->get_RemotePorts(&bstrVal)))
			//{
			//	wprintf(L"Remote Ports:      %s\n", bstrVal);
			//}
		}
		//else
		//{
		//	if (SUCCEEDED(FwRule->get_IcmpTypesAndCodes(&bstrVal)))
		//	{
		//		wprintf(L"ICMP TypeCode:      %s\n", bstrVal);
		//	}
		//}
	}

	//if (SUCCEEDED(FwRule->get_LocalAddresses(&bstrVal)))
	//{
	//	wprintf(L"LocalAddresses:   %s\n", bstrVal);
	//}

	//if (SUCCEEDED(FwRule->get_RemoteAddresses(&bstrVal)))
	//{
	//	wprintf(L"RemoteAddresses:  %s\n", bstrVal);
	//}

	//if (SUCCEEDED(FwRule->get_Profiles(&lProfileBitmask)))
	//{
	//	// The returned bitmask can have more than 1 bit set if multiple profiles 
	//	//   are active or current at the same time

	//	for (int i=0; i<3; i++)
	//	{
	//		if ( lProfileBitmask & ProfileMap[i].Id  )
	//		{
	//			wprintf(L"Profile:  %s\n", ProfileMap[i].Name);
	//		}
	//	}
	//}

	//if (SUCCEEDED(FwRule->get_Direction(&fwDirection)))
	//{
	//	switch(fwDirection)
	//	{
	//	case NET_FW_RULE_DIR_IN:

	//		wprintf(L"Direction:        %s\n", NET_FW_RULE_DIR_IN_NAME);
	//		break;

	//	case NET_FW_RULE_DIR_OUT:

	//		wprintf(L"Direction:        %s\n", NET_FW_RULE_DIR_OUT_NAME);
	//		break;

	//	default:

	//		break;
	//	}
	//}

	//if (SUCCEEDED(FwRule->get_Action(&fwAction)))
	//{
	//	switch(fwAction)
	//	{
	//	case NET_FW_ACTION_BLOCK:

	//		wprintf(L"Action:           %s\n", NET_FW_RULE_ACTION_BLOCK_NAME);
	//		break;

	//	case NET_FW_ACTION_ALLOW:

	//		wprintf(L"Action:           %s\n", NET_FW_RULE_ACTION_ALLOW_NAME);
	//		break;

	//	default:

	//		break;
	//	}
	//}

	//if (SUCCEEDED(FwRule->get_Interfaces(&InterfaceArray)))
	//{
	//	if(InterfaceArray.vt != VT_EMPTY)
	//	{
	//		SAFEARRAY    *pSa = NULL;

	//		pSa = InterfaceArray.parray;

	//		for(long index= pSa->rgsabound->lLbound; index < (long)pSa->rgsabound->cElements; index++)
	//		{
	//			SafeArrayGetElement(pSa, &index, &InterfaceString);
	//			wprintf(L"Interfaces:       %s\n", (BSTR)InterfaceString.bstrVal);
	//		}
	//	}
	//}

	//if (SUCCEEDED(FwRule->get_InterfaceTypes(&bstrVal)))
	//{
	//	wprintf(L"Interface Types:  %s\n", bstrVal);
	//}

	//if (SUCCEEDED(FwRule->get_Enabled(&bEnabled)))
	//{
	//	if (bEnabled)
	//	{
	//		wprintf(L"Enabled:          %s\n", NET_FW_RULE_ENABLE_IN_NAME);
	//	}
	//	else
	//	{
	//		wprintf(L"Enabled:          %s\n", NET_FW_RULE_DISABLE_IN_NAME);
	//	}
	//}

	//if (SUCCEEDED(FwRule->get_Grouping(&bstrVal)))
	//{
	//	wprintf(L"Grouping:         %s\n", bstrVal);
	//}

	//if (SUCCEEDED(FwRule->get_EdgeTraversal(&bEnabled)))
	//{
	//	if (bEnabled)
	//	{
	//		wprintf(L"Edge Traversal:   %s\n", NET_FW_RULE_ENABLE_IN_NAME);
	//	}
	//	else
	//	{
	//		wprintf(L"Edge Traversal:   %s\n", NET_FW_RULE_DISABLE_IN_NAME);
	//	}
	//}
}


// Instantiate INetFwPolicy2
HRESULT WFCOMInitialize_FirewallLib(INetFwPolicy2** ppNetFwPolicy2)
{
	HRESULT hr = S_OK;

	hr = CoCreateInstance(
		__uuidof(NetFwPolicy2), 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		__uuidof(INetFwPolicy2), 
		(void**)ppNetFwPolicy2);

	if (FAILED(hr))
	{
		wprintf(L"CoCreateInstance for INetFwPolicy2 failed: 0x%08lx\n", hr);
		goto Cleanup;        
	}

Cleanup:
	return hr;
}

//============================================================================
EAppErr AddOutboundRule( wchar_t * exePath, uint16_t u16Port )
{
	HRESULT hrComInit = S_OK;
	HRESULT hr = S_OK;

	INetFwPolicy2 *pNetFwPolicy2 = NULL;
	INetFwRules *pFwRules = NULL;
	INetFwRule *pFwRule = NULL;

	long CurrentProfilesBitMask = 0;

	if( RuleExists( L"P2PThing Outbound", exePath, u16Port) )
	{
		LogMsg( LOG_DEBUG, "Firewall rule exists\n");
		return eAppErrNone;
	}


	BSTR bstrRuleName = SysAllocString(L"P2PThing Outbound");
	BSTR bstrRuleDescription = SysAllocString(L"Allow outbound network traffic from P2PThing");
	BSTR bstrRuleGroup = SysAllocString(L"Peer To Peer Network");
	BSTR bstrRuleApplication = SysAllocString(exePath);
	BSTR bstrRuleLPorts = SysAllocString(L"0");

	// Initialize COM.
	hrComInit = CoInitializeEx(
		0,
		COINIT_APARTMENTTHREADED
		);

	// Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
	// initialized with a different mode. Since we don't care what the mode is,
	// we'll just use the existing mode.
	if (hrComInit != RPC_E_CHANGED_MODE)
	{
		if (FAILED(hrComInit))
		{
			printf("CoInitializeEx failed: 0x%08lx\n", hrComInit);
			goto Cleanup;
		}
	}

	// Retrieve INetFwPolicy2
	hr = WFCOMInitialize_FirewallLib(&pNetFwPolicy2);
	if (FAILED(hr))
	{
		goto Cleanup;
	}

	// Retrieve INetFwRules
	hr = pNetFwPolicy2->get_Rules(&pFwRules);
	if (FAILED(hr))
	{
		printf("get_Rules failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	// Retrieve Current Profiles bitmask
	hr = pNetFwPolicy2->get_CurrentProfileTypes(&CurrentProfilesBitMask);
	if (FAILED(hr))
	{
		printf("get_CurrentProfileTypes failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	// When possible we avoid adding firewall rules to the Public profile.
	// If Public is currently active and it is not the only active profile, we remove it from the bitmask
	if ((CurrentProfilesBitMask & NET_FW_PROFILE2_PUBLIC) &&
		(CurrentProfilesBitMask != NET_FW_PROFILE2_PUBLIC))
	{
		CurrentProfilesBitMask ^= NET_FW_PROFILE2_PUBLIC;
	}

	// Create a new Firewall Rule object.
	hr = CoCreateInstance(
		__uuidof(NetFwRule),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwRule),
		(void**)&pFwRule);
	if (FAILED(hr))
	{
		printf("CoCreateInstance for Firewall Rule failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	// Populate the Firewall Rule object
	pFwRule->put_Name(bstrRuleName);
	pFwRule->put_Description(bstrRuleDescription);
	pFwRule->put_ApplicationName(bstrRuleApplication);
	pFwRule->put_Protocol(NET_FW_IP_PROTOCOL_TCP);
	pFwRule->put_LocalPorts(bstrRuleLPorts);
	pFwRule->put_Direction(NET_FW_RULE_DIR_OUT);
	pFwRule->put_Grouping(bstrRuleGroup);
	pFwRule->put_Profiles(CurrentProfilesBitMask);
	pFwRule->put_Action(NET_FW_ACTION_ALLOW);
	pFwRule->put_Enabled(VARIANT_TRUE);

	// Add the Firewall Rule
	hr = pFwRules->Add(pFwRule);
	if (FAILED(hr))
	{
		printf("Firewall Rule Add failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

Cleanup:

	// Free BSTR's
	SysFreeString(bstrRuleName);
	SysFreeString(bstrRuleDescription);
	SysFreeString(bstrRuleGroup);
	SysFreeString(bstrRuleApplication);
	SysFreeString(bstrRuleLPorts);

	// Release the INetFwRule object
	if (pFwRule != NULL)
	{
		pFwRule->Release();
	}

	// Release the INetFwRules object
	if (pFwRules != NULL)
	{
		pFwRules->Release();
	}

	// Release the INetFwPolicy2 object
	if (pNetFwPolicy2 != NULL)
	{
		pNetFwPolicy2->Release();
	}

	// Uninitialize COM.
	if (SUCCEEDED(hrComInit))
	{
		CoUninitialize();
	}
	return eAppErrNone;
}

//============================================================================
//============================================================================
EAppErr AddListenPortRule( wchar_t * exePath, uint16_t u16Port )
{

	EAppErr eAppErr = eAppErrUnknown;
	HRESULT hrComInit = S_OK;
	HRESULT hr = S_OK;

	INetFwPolicy2 *pNetFwPolicy2 = NULL;
	INetFwRules *pFwRules = NULL;
	INetFwRule *pFwRule = NULL;

	long CurrentProfilesBitMask = 0;

	wchar_t  portBuf[ 32 ];
    swprintf( portBuf, sizeof(portBuf) / sizeof(*portBuf), L"%d", u16Port );

	if( RuleExists( L"P2PThing Inbound", exePath, u16Port) )
	{
		LogMsg( LOG_DEBUG, "Firewall rule exists\n");
		return eAppErrNone;
	}

	BSTR bstrRuleName = SysAllocString(L"P2PThing Inbound");
	BSTR bstrRuleDescription = SysAllocString(L"Allow incoming network traffic to P2Pthing");
	BSTR bstrRuleGroup = SysAllocString(L"Peer To Peer Network");
	BSTR bstrRuleApplication = SysAllocString(exePath);
	BSTR bstrRuleService = SysAllocString(L"P2PThing");
	BSTR bstrRuleLPorts = SysAllocString(portBuf);


	// Initialize COM.
	hrComInit = CoInitializeEx(
		0,
		COINIT_APARTMENTTHREADED
		);

	// Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
	// initialized with a different mode. Since we don't care what the mode is,
	// we'll just use the existing mode.
	if (hrComInit != RPC_E_CHANGED_MODE)
	{
		if (FAILED(hrComInit))
		{
			LogMsg( LOG_DEBUG, "CoInitializeEx failed: 0x%08lx\n", hrComInit);
			goto Cleanup;
		}
	}

	// Retrieve INetFwPolicy2
	hr = WFCOMInitialize_FirewallLib(&pNetFwPolicy2);
	if (FAILED(hr))
	{
		goto Cleanup;
	}

	// Retrieve INetFwRules
	hr = pNetFwPolicy2->get_Rules(&pFwRules);
	if (FAILED(hr))
	{
		LogMsg( LOG_DEBUG, "get_Rules failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	// Retrieve Current Profiles bitmask
	hr = pNetFwPolicy2->get_CurrentProfileTypes(&CurrentProfilesBitMask);
	if (FAILED(hr))
	{
		LogMsg( LOG_DEBUG, "get_CurrentProfileTypes failed: 0x%08lx\n", hr);
		goto Cleanup;
	} 

	// When possible we avoid adding firewall rules to the Public profile.
	// If Public is currently active and it is not the only active profile, we remove it from the bitmask
	//if ((CurrentProfilesBitMask & NET_FW_PROFILE2_PUBLIC) &&
	//	(CurrentProfilesBitMask != NET_FW_PROFILE2_PUBLIC))
	//{
	//	CurrentProfilesBitMask ^= NET_FW_PROFILE2_PUBLIC;
	//}
	CurrentProfilesBitMask = NET_FW_PROFILE2_ALL;

	// Create a new Firewall Rule object.
	hr = CoCreateInstance(
		__uuidof(NetFwRule),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwRule),
		(void**)&pFwRule);
	if (FAILED(hr))
	{
		LogMsg( LOG_DEBUG, "CoCreateInstance for Firewall Rule failed: 0x%08lx\n", hr);
		goto Cleanup;
	}

	// Populate the Firewall Rule object
	pFwRule->put_Name(bstrRuleName);
	pFwRule->put_Description(bstrRuleDescription);
	pFwRule->put_ApplicationName(bstrRuleApplication);
	pFwRule->put_ServiceName(bstrRuleService);
	pFwRule->put_Protocol(NET_FW_IP_PROTOCOL_TCP);
	pFwRule->put_LocalPorts(bstrRuleLPorts);
	pFwRule->put_Grouping(bstrRuleGroup);
	pFwRule->put_Profiles(CurrentProfilesBitMask);
	pFwRule->put_Action(NET_FW_ACTION_ALLOW);
	pFwRule->put_Enabled(VARIANT_TRUE);

	// Add the Firewall Rule
	hr = pFwRules->Add(pFwRule);
	if (FAILED(hr))
	{
		LogMsg( LOG_DEBUG, "Firewall Rule Add failed: 0x%08lx\n", hr);
		goto Cleanup;
	}
	else
	{
		eAppErr = eAppErrNone;
	}

Cleanup:

	// Free BSTR's
	SysFreeString(bstrRuleName);
	SysFreeString(bstrRuleDescription);
	SysFreeString(bstrRuleGroup);
	SysFreeString(bstrRuleApplication);
	SysFreeString(bstrRuleService);
	SysFreeString(bstrRuleLPorts);

	// Release the INetFwRule object
	if (pFwRule != NULL)
	{
		pFwRule->Release();
	}

	// Release the INetFwRules object
	if (pFwRules != NULL)
	{
		pFwRules->Release();
	}

	// Release the INetFwPolicy2 object
	if (pNetFwPolicy2 != NULL)
	{
		pNetFwPolicy2->Release();
	}

	// Uninitialize COM.
	if (SUCCEEDED(hrComInit))
	{
		CoUninitialize();
	}

	return eAppErr;
}



//============================================================================
EAppErr AddServicePortExeption( wchar_t * exePath, uint16_t u16Port )
{
	EAppErr eAppErr = eAppErrNone;
	HRESULT hr = S_OK;

	INetFwService * fwService = NULL;

	return eAppErr;
}

//============================================================================
EAppErr Firewall::addGlobalPort( uint16_t u16Port )
{
	EAppErr eAppErr = eAppErrNone;
	HRESULT hr = S_OK;
	HRESULT comInit = E_FAIL;
	INetFwProfile* fwProfile = NULL;

	wchar_t exeName[ MAX_PATH ];
	int iExeNameStrLen = GetModuleFileNameW( NULL, exeName, MAX_PATH );


	// Initialize COM.
	comInit = CoInitializeEx(
		0,
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
		);

	// Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
	// initialized with a different mode. Since we don't care what the mode is,
	// we'll just use the existing mode.
	if (comInit != RPC_E_CHANGED_MODE)
	{
		hr = comInit;
		if (FAILED(hr))
		{
			LogMsg( LOG_DEBUG, "CoInitializeEx failed: 0x%08lx\n", hr);
			eAppErr = eAppErrUnknown;
			goto error;
		}
	}

	for( int i = 0; i < 2; i++ )
	{
		// Retrieve the firewall profile currently in effect.
		hr = WindowsFirewallInitialize(&fwProfile, i );
		if (FAILED(hr))
		{
			LogMsg( LOG_DEBUG, "WindowsFirewallInitialize failed: 0x%08lx\n", hr);
			eAppErr = eAppErrUnknown;
			goto error;
		}

		//// Turn off the firewall.
		//hr = WindowsFirewallTurnOff(fwProfile);
		//if (FAILED(hr))
		//{
		//	LogMsg( LOG_DEBUG, "WindowsFirewallTurnOff failed: 0x%08lx\n", hr);
		//	goto error;
		//}

		//// Turn on the firewall.
		//hr = WindowsFirewallTurnOn(fwProfile);
		//if (FAILED(hr))
		//{
		//	LogMsg( LOG_DEBUG, "WindowsFirewallTurnOn failed: 0x%08lx\n", hr);
		//	goto error;
		//}


		// Add P2PThing to the authorized application collection.

		if( 0 != iExeNameStrLen )
		{
			hr = WindowsFirewallAddApp(
				fwProfile,
				exeName,
				L"P2PThing"
				);
			if (FAILED(hr))
			{
				LogMsg( LOG_DEBUG, "WindowsFirewallAddApp failed: 0x%08lx\n", hr);
				eAppErr = eAppErrUnknown;
				if( E_ACCESSDENIED == hr )
				{
					eAppErr = eAppErrAccessDenied;
				}
				//goto error;
			}
			else
			{
				// make sure enabled
				INetFwAuthorizedApplication* pFwProgram = NULL;         
				INetFwAuthorizedApplications* pFWApps = NULL;    

				// Only add the application if it isn't authorized                 
				// Retrieve the authorized application collection                         
				HRESULT hr = fwProfile->get_AuthorizedApplications( &pFWApps );                         
				if( FAILED( hr ) || ! pFWApps )
				{
					// COM not initialized
					LogMsg( LOG_DEBUG, "WindowsFirewallAddApp could not get authorized apps: 0x%08lx\n", hr);
				}

				hr = pFWApps->Item( BSTR( exeName ), &pFwProgram );
				if ( SUCCEEDED( hr ) && pFwProgram )
				{

					// Check the box next to the program
					hr = pFwProgram->put_Enabled( VARIANT_TRUE );
					if ( FAILED( hr ) )
					{
						LogMsg( LOG_DEBUG, "WindowsFirewallAddApp failed to enable our app: 0x%08lx\n", hr);
					}
					else
					{
						LogMsg( LOG_DEBUG, "WindowsFirewallAddApp SUCCESS ENABLING OUR APP\n");
					}
				}
				else
				{
					LogMsg( LOG_DEBUG, "WindowsFirewallAddApp could not get our specific app: 0x%08lx\n", hr);
				}
			}
		}

		// Add TCP:: port to list of globally open ports.
		hr = WindowsFirewallPortAdd(fwProfile, u16Port, NET_FW_IP_PROTOCOL_TCP, L"P2PThing TCP");
		if (FAILED(hr))
		{
			LogMsg( LOG_DEBUG, "WindowsFirewallPortAdd failed: 0x%08lx\n", hr);
			eAppErr = eAppErrUnknown;
			if( E_ACCESSDENIED == hr )
			{
				eAppErr = eAppErrAccessDenied;
			}
		}

		// Add UDP:: port to list of globally open ports.
		hr = WindowsFirewallPortAdd(fwProfile, RC_DEFAULT_MULTICAST_PORT, NET_FW_IP_PROTOCOL_UDP, L"P2PThing Multicast");
		if (FAILED(hr))
		{
			LogMsg( LOG_DEBUG, "WindowsFirewallPortAdd failed: 0x%08lx\n", hr);
			eAppErr = eAppErrUnknown;
			if( E_ACCESSDENIED == hr )
			{
				eAppErr = eAppErrAccessDenied;
			}
		}

error:

		// Release the firewall profile.
		WindowsFirewallCleanup(fwProfile);
	}

	// Uninitialize COM.
	if (SUCCEEDED(comInit))
	{
		CoUninitialize();
	}


	EAppErr eAppErrAddRuleResult = eAppErrUnknown;
	if( 0 != iExeNameStrLen )
	{
		eAppErrAddRuleResult = AddListenPortRule( exeName, u16Port );
	}

	if( eAppErrNone == eAppErr )
	{
		eAppErr = eAppErrAddRuleResult;
	}

	//return eAppErr;

	return eAppErrNone;
}

#endif // TARGET_OS_WINDOWS

