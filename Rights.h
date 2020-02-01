#include "Authentication.h"
#include <processthreadsapi.h>
#include <handleapi.h>
#include <securitybaseapi.h>

bool Auth::AdminRights()
{
	bool fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) 
	{
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) 
			fRet = Elevation.TokenIsElevated;
	}
	if (hToken) 
		CloseHandle(hToken);

	return fRet;
}