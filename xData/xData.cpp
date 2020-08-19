#include "stdafx.h"
//#include "xDataBaseDll.h"
#include "stdlib.h"
#include "defines.h"

const wchar_t* GetXVersion()
{ 
	return XDATA_VERSION;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
     return TRUE;
}

//class SHEAP{
//public:
//	SHEAP(){
//		hHeap = GetProcessHeap();
//	};
//	HANDLE hHeap;
//} HEAP;
//
//inline void * operator new[]( size_t cb )
//{
//	HANDLE he = GetProcessHeap();
//    void *res = HeapAlloc(GetProcessHeap(),0,cb);
//    return res;
//}
//
//inline void operator delete[]( void * p )
//{
//    HeapFree(GetProcessHeap(),0,p);
//}
//
//inline void operator delete( void * p )
//{
//    HeapFree(GetProcessHeap(),0,p);
//}

