// stdafx.cpp : source file that includes just the standard includes
// xData.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

int main()
{
 return 0;
}

#include <locale.h>

class INIT{
public:
	INIT(){
#ifndef _WIN32_WCE
		setlocale(LC_CTYPE,"Russian");
#endif
	};
} init;

//#ifndef _WIN32_WCE
//  BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//					 )
//#else
//  #include <windows.h>
//  #include <commctrl.h>
//
//  BOOL APIENTRY DllMain( HANDLE hModule, 
//                       DWORD  ul_reason_for_call, 
//                       LPVOID lpReserved
//					 )
//#endif
//
//{
//HRESULT hRes;
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//		break;
//	case DLL_PROCESS_DETACH:
//		break;
//	case DLL_THREAD_DETACH:
//	case DLL_THREAD_ATTACH:
//		break;
//	}
//    return TRUE;
//}

//SHEAP HEAP;