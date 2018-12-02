/* Sed: http://msdn.microsoft.com/library/en-us/shellcc/platform/shell/programmersguide/versions.asp */
#include <WinSock2.h>
#include <windows.h>
#include <shlwapi.h>
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) HRESULT DllGetVersion (DLLVERSIONINFO2 *pdvi);
#ifdef __cplusplus
}
#endif

HRESULT DllGetVersion (DLLVERSIONINFO2 *pdvi)
{
	if ( !pdvi || (pdvi->info1.cbSize != sizeof (*pdvi)) )
		return (E_INVALIDARG);
	pdvi->info1.dwMajorVersion = 2;
	pdvi->info1.dwMinorVersion = 3;
	pdvi->info1.dwBuildNumber = 5;
	pdvi->info1.dwPlatformID = DLLVER_PLATFORM_WINDOWS;
	if (pdvi->info1.cbSize == sizeof (DLLVERSIONINFO2))
		pdvi->ullVersion = MAKEDLLVERULL (2, 3, 5, 2742);
	return S_OK;
}
