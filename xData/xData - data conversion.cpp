#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include "xData.h"
#include "xDataBaseDll.h"
#include <string>

#define GetThisDataPointer() (((xData*)this)->GetThisDataPointer())
#define EnterCS() (((xData*)this)->EnterCS())
#define LeaveCS() (((xData*)this)->LeaveCS())
#define ThisIsAnsiString() (((xData*)this)->ThisIsAnsiString())
#define IsUnicodeString() (((xData*)this)->IsUnicodeString())
#define IsNumber() (((xData*)this)->IsNumber())
//#define IsNumber(x) (((xData*)this)->IsNumber(x))
#define IsEmpty() (((xData*)this)->IsEmpty())
#define IsPointer() (((xData*)this)->IsPointer())
#define IsContainer() (((xData*)this)->IsContainer())
#define GetTypeSize(x) (((xData*)this)->GetTypeSize(x))
#define AsValue() (((xData*)this)->AsValue())
#define AsValueCs() (((xData*)this)->AsValueCs())
#define AsReflectorData() (((xData*)this)->AsReflectorData())
#define AsControlData() (((xData*)this)->AsControlData())
#define GetThisDataType() (((xData*)this)->GetThisDataType())
#define IsAlone() (((xData*)this)->IsAlone())
#define IsSignedInt() (((xData*)this)->IsSignedInt())
#define IsUnsignedInt() (((xData*)this)->IsUnsignedInt())
#define IsReal() (((xData*)this)->IsReal())

xDataBaseDll::operator bool() const
{
	if (!DataSize) return false;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return ((xDataBaseDll*)GetThisDataPointer())->operator bool();
		Throw(DE_OperationWithEmpty);
		return false;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_BOOL:			return *(bool*)GetThisDataPointer();
	case DT_INT8:			return *(__int8*)GetThisDataPointer()!=0;
	case DT_INT16:			return *(__int16*)GetThisDataPointer()!=0;
	case DT_INT32:			return *(__int32*)GetThisDataPointer()!=0;
	case DT_INT64:			return *(__int64*)GetThisDataPointer()!=0;
	case DT_UINT8:			return *(unsigned __int8*)GetThisDataPointer()!=0;
	case DT_UINT16:			return *(unsigned __int16*)GetThisDataPointer()!=0;
	case DT_UINT32:			return *(unsigned __int32*)GetThisDataPointer()!=0;
	case DT_UINT64:			return *(unsigned __int64*)GetThisDataPointer()!=0;
	case DT_FLOAT:			return *(float*)GetThisDataPointer()!=0;
	case DT_DOUBLE:			return (*(double*)GetThisDataPointer())!=0;
	case DT_STRING:			if (DataSize&0x800000)
								return *((wchar_t*)GetThisDataPointer())==0;
							else
								return *((char*)GetThisDataPointer())==0;
	}
	return false;
}

xDataBaseDll::operator float() const
{
	if (!DataSize) return 0.0f;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (float) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0.0f;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0.0f;
	case DT_STRING:			if (DataSize&0x800000)
							{
#ifdef WINCE
								wchar_t *t;	return (float)wcstod((wchar_t*)GetThisDataPointer(),&t);
#else
								return (float)_wtof((wchar_t*)GetThisDataPointer());
#endif
							} else
								return (float)atof((char*)GetThisDataPointer());

	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1.0f : 0.0f;
	case DT_INT8:			return (float)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (float)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (float)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (float)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (float)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (float)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (float)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (float)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return *(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (float)(*(double*)GetThisDataPointer());
	}
	return 0.0f;
}

xDataBaseDll::operator double() const
{
	if (!DataSize) return 0.0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (double) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0.0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0.0;
	case DT_STRING:			if (DataSize&0x800000)
							{
#ifdef WINCE
								wchar_t *t;	return (double)wcstod((wchar_t*)GetThisDataPointer(),&t);
#else
								return (double)_wtof((wchar_t*)GetThisDataPointer());
#endif
							} else
								return (double)atof((char*)GetThisDataPointer());
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1.0 : 0.0;
	case DT_INT8:			return (double)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (double)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (double)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (double)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (double)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (double)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (double)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (double)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (double)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return *(double*)GetThisDataPointer();
	}
	return 0.0;
}

xDataBaseDll::operator __int8() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
            return ((xDataBaseDll*)GetThisDataPointer())->operator __int8();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return __int8(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return __int8(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (__int8)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (__int8)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (__int8)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (__int8)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (__int8)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (__int8)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (__int8)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (__int8)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (__int8)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (__int8)*(double*)GetThisDataPointer();
	}
	return 0;
}

xDataBaseDll::operator unsigned __int8() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (unsigned __int8) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return unsigned __int8(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return unsigned __int8(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (unsigned __int8)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (unsigned __int8)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (unsigned __int8)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (unsigned __int8)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (unsigned __int8)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (unsigned __int8)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (unsigned __int8)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (unsigned __int8)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (unsigned __int8)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (unsigned __int8)*(double*)GetThisDataPointer();
	}
	return 0;
}

xDataBaseDll::operator __int16() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (__int16) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return __int16(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return __int16(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (__int16)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (__int16)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (__int16)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (__int16)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (__int16)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (__int16)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (__int16)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (__int16)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (__int16)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (__int16)*(double*)GetThisDataPointer();
	}
	return 0;
}

xDataBaseDll::operator unsigned __int16() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (unsigned __int16) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return unsigned __int16(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return unsigned __int16(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (unsigned __int16)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (unsigned __int16)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (unsigned __int16)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (unsigned __int16)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (unsigned __int16)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (unsigned __int16)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (unsigned __int16)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (unsigned __int16)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (unsigned __int16)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (unsigned __int16)*(double*)GetThisDataPointer();
	}
	return 0;
}

xDataBaseDll::operator __int32() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (__int32) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return __int32(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return __int32(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (__int32)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (__int32)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (__int32)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (__int32)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (__int32)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (__int32)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (__int32)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (__int32)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (__int32)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (__int32)*(double*)GetThisDataPointer();
	}
	return 0;
}

xDataBaseDll::operator unsigned __int32() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (unsigned __int32) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return unsigned __int32(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return unsigned __int32(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (unsigned __int32)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (unsigned __int32)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (unsigned __int32)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (unsigned __int32)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (unsigned __int32)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (unsigned __int32)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (unsigned __int32)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (unsigned __int32)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (unsigned __int32)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (unsigned __int32)*(double*)GetThisDataPointer();
	}
	return 0;
}

xDataBaseDll::operator __int64() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (__int64) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return __int64(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return __int64(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (__int64)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (__int64)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (__int64)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (__int64)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (__int64)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (__int64)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (__int64)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (__int64)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (__int64)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (__int64)*(double*)GetThisDataPointer();
	}
	return 0;
}

xDataBaseDll::operator unsigned __int64() const
{
	if (!DataSize) return 0;
	switch (GetThisDataType()){
	case DT_UNDEFINE:
		if (Pointer && DataSize==1)
			return (unsigned __int64) *(xDataBaseDll*)GetThisDataPointer();
		Throw(DE_OperationWithEmpty);
		return 0;
	case DT_IDREFLECTOR:	Throw(DE_ConversionFromReflector);	return 0;
	case DT_STRING:			if (DataSize&0x800000)
								return unsigned __int64(_wtoi64((wchar_t*)GetThisDataPointer()));
							else
								return unsigned __int64(_atoi64((char*)GetThisDataPointer()));
	case DT_BOOL:			return (*(bool*)GetThisDataPointer()) ? 1 : 0;
	case DT_INT8:			return (unsigned __int64)*(__int8*)GetThisDataPointer();
	case DT_INT16:			return (unsigned __int64)*(__int16*)GetThisDataPointer();
	case DT_INT32:			return (unsigned __int64)*(__int32*)GetThisDataPointer();
	case DT_INT64:			return (unsigned __int64)*(__int64*)GetThisDataPointer();
	case DT_UINT8:			return (unsigned __int64)*(unsigned __int8*)GetThisDataPointer();
	case DT_UINT16:			return (unsigned __int64)*(unsigned __int16*)GetThisDataPointer();
	case DT_UINT32:			return (unsigned __int64)*(unsigned __int32*)GetThisDataPointer();
	case DT_UINT64:			return (unsigned __int64)*(unsigned __int64*)GetThisDataPointer();
	case DT_FLOAT:			return (unsigned __int64)*(float*)GetThisDataPointer();
	case DT_DOUBLE:			return (unsigned __int64)*(double*)GetThisDataPointer();
	}
	return 0;
}

char NULLSTRING[2]={0x00,0x00};

xDataBaseDll::operator char*()
{
	if (!DataSize || IsEmpty()) return NULLSTRING;
	if (DataSize==1 && DataType==DT_UNDEFINE)
		return ((xData*)GetThisDataPointer())->operator char *();
	if (ThisIsAnsiString())
		return (char*)GetThisDataPointer();
	ToString(false);
	if (!ThisIsAnsiString())
		return NULLSTRING;
	return (char*)GetThisDataPointer();
}

xDataBaseDll::operator wchar_t*()
{
	if (!DataSize || IsEmpty()) return (wchar_t*)NULLSTRING;
	if (DataSize==1 && DataType==DT_UNDEFINE)
		return ((xData*)GetThisDataPointer())->operator wchar_t *();
	if (IsUnicodeString())
		return (wchar_t*)GetThisDataPointer();
	ToString(true);
	if (!IsUnicodeString())
		return (wchar_t*)NULLSTRING;
	return (wchar_t*)GetThisDataPointer();
}

void* xDataBaseDll::GetPointerToFormatString()
{
	if (Pointer && DataType==DT_UNDEFINE && DataSize==1)
	{
		return ((xDataBaseDll*)GetThisDataPointer())->GetPointerToFormatString();
	}
	if (DataType==DT_STRING && !(DataSize&0x800000) && DataSize>1)
	{
		char* r = strchr((char*)GetThisDataPointer(),'%');
		while (r && r[1]=='%')
			r = strchr(r+2,'%');
		return r;
	}
	if (DataType==DT_STRING && (DataSize&0x800000) && (DataSize&0x7fffff)>1)
	{
		wchar_t* r = wcschr((wchar_t*)GetThisDataPointer(),L'%');
		while (r && r[1]==L'%')
			r = wcschr(r+2,L'%');
		return r;
	}
	return 0;
}

unsigned __int8 xDataBaseDll::ToString(bool Unicode, char* format)
{
char *Buf = new char[128];
double dv=0;
unsigned __int64 iv=0;
int sign=0;
unsigned __int8 FormatStringSize=0;
unsigned __int8 precisionorradix=MAXPRECISION, maxsymbols=31;
bool expalways=false;
bool zerofill=false;
bool rightalign=true;

	if (Pointer)
	{
		if (DataType==DT_UNDEFINE)
		{
			delete Buf;
			return ((xData*)GetThisDataPointer())->ToString(Unicode, format);
		}
		Throw(DE_ConversionFromPointer);
		delete Buf;
		return 0;
	}
	if (ThisIsAnsiString())
	{
		if (!Unicode)
		{
			delete Buf;
			return 0;
		}
		wchar_t * NewString;
		unsigned __int32 s = ansitextlength((char*)GetThisDataPointer(),65536);
		NewString = (wchar_t*)xDataBaseDll::alloc(s*2+2);
		size_t OutSize;
#ifdef WINCE
			OutSize = mbstowcs(NewString, (char*)GetThisDataPointer(),s+1);
#else
	#if _MSC_VER <1400
			OutSize = mbstowcs(NewString, (char*)GetThisDataPointer(),s+1);
	#else
			mbstowcs_s(&OutSize, NewString, s+1, (char*)GetThisDataPointer(), _TRUNCATE);
	#endif
#endif
		Invalidate(false,false,false);
		if (OutSize)
		{
			((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(NewString);
			DataType = DT_STRING;
			DataSize = (s+1)|0x800000;
		}
		delete Buf;
		return 0;
	}
	if (IsUnicodeString())
	{
		if (Unicode)
		{
			delete Buf;
			return 0;
		}
		char * NewString;
		unsigned __int32 s = unicodetextlength((const wchar_t*)GetThisDataPointer(),65536);
		NewString = (char*)xDataBaseDll::alloc(s+1);
		size_t OutSize;
#ifdef WINCE
		OutSize = wcstombs(NewString, (wchar_t*)GetThisDataPointer(),s+1);
#else
	#if _MSC_VER <1400
			OutSize = wcstombs(NewString, (wchar_t*)GetThisDataPointer(),s+1);
	#else
			wcstombs_s(&OutSize, NewString, s+1, (wchar_t*)GetThisDataPointer(), _TRUNCATE);
	#endif
#endif
		Invalidate(false,false,false);
		if (OutSize)
		{
            ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(NewString);
			DataType = DT_STRING;
			DataSize = (s+1);
		}
		delete Buf;
		return 0;
	}

	if (DataType == DT_UNDEFINE || DataType == DT_IDREFLECTOR || DataType == DT_CONTROLDATA)
	{
		Throw(DE_ConversionFromUndefine);
		goto ERR;
	}

	EnterCS();
	switch (DataType){
	case DT_UINT8:
    	iv = (unsigned __int64) *(unsigned __int8*)GetThisDataPointer();
		break;
	case DT_UINT16:
		iv = (unsigned __int64) *(unsigned __int16*)GetThisDataPointer();
		break;
	case DT_UINT32:
		iv = (unsigned __int64) *(unsigned __int32*)GetThisDataPointer();
		break;
	case DT_UINT64:
		iv = (unsigned __int64) *(unsigned __int64*)GetThisDataPointer();
		break;
	case DT_INT8:
		if (*(signed __int8*)GetThisDataPointer()<0) 
		{
			sign=1;
			iv = (unsigned __int64)(- *(signed __int8*)GetThisDataPointer());
		} else
			iv = (unsigned __int64)(*(signed __int8*)GetThisDataPointer());
		break;
	case DT_INT16:
		if (*(__int16*)GetThisDataPointer()<0) 
		{
			sign=1;
			iv = (unsigned __int64)(- *(__int16*)GetThisDataPointer());
		} else
			iv = (unsigned __int64)(*(__int16*)GetThisDataPointer());
		break;
	case DT_INT32:
		if (*(__int32*)GetThisDataPointer()<0) 
		{
			sign=1;
			iv = (unsigned __int64)(- *(__int32*)GetThisDataPointer());
		} else
			iv = (unsigned __int64)(*(__int32*)GetThisDataPointer());
		break;
	case DT_INT64:
		if (*(__int64*)GetThisDataPointer()<0) 
		{
			sign=1;
			iv = (unsigned __int64)(- *(__int64*)GetThisDataPointer());
		} else
        {
			iv = (unsigned __int64)(*(__int64*)GetThisDataPointer());
        }
		break;
	case DT_FLOAT:
		dv = (double)*(float*)GetThisDataPointer();
		break;
	case DT_DOUBLE:
		dv = *(double*)GetThisDataPointer();
		break;
	}

	if (DataType==DT_BOOL)
	{
		if (*(bool*)GetThisDataPointer())
			memcpy(Buf,"true",5);
		else
			memcpy(Buf,"false",6);
	}
	if (DataType==DT_UINT8 || DataType == DT_UINT16 || DataType == DT_UINT32 || DataType == DT_UINT64 ||
		DataType==DT_INT8 || DataType == DT_INT16 || DataType == DT_INT32 || DataType == DT_INT64)
	{
		precisionorradix=10;
		FormatStringSize = ParseFormatStringForNumberAndGetSize(Unicode, format, precisionorradix, maxsymbols, expalways, zerofill, rightalign);
		I2S(iv, sign, Buf, maxsymbols, zerofill, rightalign, precisionorradix);
	}

	if (DataType==DT_FLOAT || DataType == DT_DOUBLE)
	{
		precisionorradix=MAXPRECISION;
		FormatStringSize = ParseFormatStringForNumberAndGetSize(Unicode, format, precisionorradix, maxsymbols, expalways, zerofill, rightalign);
		D2S(dv, Buf, maxsymbols, precisionorradix, expalways, zerofill, rightalign);
	}

	Invalidate(false,false,false);
	if (!Unicode)
	{
		DataType = DT_STRING;
		DataSize = ansitextlength(Buf)+1;
        ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(new signed __int8[DataSize]);
		memcpy((void*)GetThisDataPointer(), Buf, DataSize);
	} else
	{
		DataType = DT_STRING;
		DataSize = (ansitextlength(Buf)+1)*2;
        ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(new signed __int8[DataSize]);
		memset((void*)GetThisDataPointer(),0,DataSize);
		for (unsigned int i=0; i<(DataSize-2)/2; i++)
			*(i+(wchar_t*)GetThisDataPointer())=Buf[i];
        DataSize |= 0x800000;
	}
	LeaveCS();
	delete Buf;
	return FormatStringSize;

ERR:
	Invalidate(false,false,false);
	if (!Unicode)
	{
		DataType = DT_STRING;
		DataSize = 1;
        ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(new signed __int8[DataSize]);
		*((char*)GetThisDataPointer()) = 0;
	} else
	{
		DataType = DT_STRING;
		DataSize = 1;
        ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(new signed __int8[DataSize*2]);
		*((wchar_t*)GetThisDataPointer()) = 0;
        DataSize |= 0x800000;
	} 
	LeaveCS();
	delete Buf;
	return FormatStringSize;
}

bool charcompare(unsigned __int32 Char, void* CharPointer, unsigned __int8 StringShift, bool Unicode)
{
	if (!Unicode)
		return char(Char) == *(StringShift+(char*)CharPointer);
	return wchar_t(Char) == *(StringShift+(wchar_t*)CharPointer);
}

bool isadigit(void* Char, unsigned __int8 CharShift, bool IncludeZero, bool Unicode)
{
	if (!Unicode)
	{
		if (IncludeZero)
			return strchr("0123456789", *(CharShift+(char*)Char)) != NULL;
		else
			return strchr("123456789", *(CharShift+(char*)Char)) != NULL;
	}
	if (IncludeZero)
		return wcschr(L"0123456789", *(CharShift+(wchar_t*)Char)) != NULL;
	return wcschr(L"123456789", *(CharShift+(wchar_t*)Char)) != NULL;
}

unsigned __int8 digittoint(void* Char, unsigned __int8 CharShift, bool Unicode)
{
	if (Unicode)
		return unsigned __int8((*(CharShift+(wchar_t*)Char))-48);
	return (*(CharShift+(char*)Char))-48;
}

unsigned __int8 xDataBaseDll::ParseFormatStringForNumberAndGetSize(bool UnicodeString, void* FormatString, 
		unsigned __int8& precisionorradix, unsigned __int8& maxsymbols, 
		bool& expalways, bool& zerofill, bool& rightalign)
{
unsigned __int8 FormatStringSize=0;
	if (&maxsymbols) maxsymbols=31;
	if (&expalways) expalways=false;
	if (&zerofill) zerofill=false;
	if (&rightalign) rightalign=false;
	if (FormatString && charcompare('%', FormatString, 0, UnicodeString) )
	{
		if (&rightalign) rightalign=true;
		FormatStringSize++;
		if (charcompare('0', FormatString, FormatStringSize, UnicodeString))
		{
			if (!isadigit(FormatString, FormatStringSize+1, false, UnicodeString))
			{
				Throw(DE_NumberFormatError);
				return FormatStringSize;
			}
			if (&zerofill) zerofill=true;
			FormatStringSize++;
		} else
			if (charcompare('-', FormatString, FormatStringSize, UnicodeString))
			{
				if (!isadigit(FormatString, FormatStringSize+1, false, UnicodeString))
				{
					Throw(DE_NumberFormatError);
					return FormatStringSize;
				}
				if (&rightalign) rightalign=false;
				FormatStringSize++;
			}
		if (isadigit(FormatString, FormatStringSize, false, UnicodeString))
		{
			if (&maxsymbols) maxsymbols = digittoint(FormatString, FormatStringSize, UnicodeString);
			FormatStringSize++;
			if (isadigit(FormatString, FormatStringSize, true, UnicodeString))
			{
				if (&maxsymbols) maxsymbols = maxsymbols*10 + digittoint(FormatString, FormatStringSize, UnicodeString);
				FormatStringSize++;
			}
		} else if (&rightalign) rightalign=false;
		if (charcompare('.', FormatString, FormatStringSize, UnicodeString))
		{
			FormatStringSize++;
			if (isadigit(FormatString, FormatStringSize, true, UnicodeString))
			{
				if (&precisionorradix) precisionorradix = digittoint(FormatString, FormatStringSize, UnicodeString);
				FormatStringSize++;
				if (isadigit(FormatString, FormatStringSize, true, UnicodeString))
				{
					if (&precisionorradix) precisionorradix = precisionorradix*10 + digittoint(FormatString, FormatStringSize, UnicodeString);
					FormatStringSize++;
				}
			}
		}
		if (charcompare('v', FormatString, FormatStringSize, UnicodeString) || charcompare('V', FormatString, FormatStringSize, UnicodeString))
		{
			if (&expalways) expalways = false;
		}
		else
			if (charcompare('e', FormatString, FormatStringSize, UnicodeString) || charcompare('E', FormatString, FormatStringSize, UnicodeString))
			{
				if (&expalways) expalways = true;
			}
			else
				if (!charcompare('i', FormatString, FormatStringSize, UnicodeString) || charcompare('I', FormatString, FormatStringSize, UnicodeString))
				{
					Throw(DE_NumberFormatError);
					return FormatStringSize;
				}
		FormatStringSize++;
	}
	return FormatStringSize;
}

bool xDataBaseDll::I2S(unsigned __int64 V, int sign, char* S, 
					unsigned __int8 maxsymbols, bool zerofill, bool rightalign, 
					unsigned __int8 radix)
{
char *Buf= new char[64];
unsigned int len,p=0;
#ifdef WINCE
	_itoa((int)V,Buf,radix);

#else
	#if _MSC_VER <1400
		_ui64toa(V,Buf,radix);
	#else
		_ui64toa_s(V,Buf, 64, radix);
	#endif
#endif
	len = ansitextlength(Buf);
	// Если число не влазит в указанное количество символов
	if (len+sign>maxsymbols) 
	{
		// Если число не влазит даже в экспоненциальном представлении
		if ((sign + 2 + ((len-1>10)?2:1)) > maxsymbols)
		{
			LeaveCS();
			Throw(DE_SizeForDigitsTooSmall);
			return false;
		}
		return D2S(double(V) * (sign?-1.0f:1.0f), S, maxsymbols, MAXPRECISION, true, zerofill, rightalign);
	}
	if (rightalign && !zerofill)
	{
		for (unsigned int i=0;i< maxsymbols-len-sign;i++)
			S[p++]=' ';
	}
	if (sign) S[p++]='-';
	if (zerofill)
	{
		for (unsigned int i=0;i< maxsymbols-len-sign;i++)
			S[p++]='0';
	}
	memcpy(&S[p],(const void*)Buf,len);
	S[p+len]=0;
    //if (p+len<maxsymbols)
    //{
    //    memset(&S[p+len],' ',maxsymbols-len);
    //    S[maxsymbols]=0;
    //}
	delete Buf;
	return true;
}

bool xDataBaseDll::D2S(double V, char* S, unsigned __int8 maxsymbols, unsigned __int8 precision, bool expalways, bool zerofill, bool rightalign)
{
// Нормализованное число имеет одну цифру до запятой и не более 16 знаков после запятой
unsigned __int64 na;	// нормализованное число умноженное на 10е16 
int pw;					// степень нормализованного числа (10 в степени pw)
int sign = 0;			// знак числа
int expsize;			// количество символов в числе, занимаемых экспонентой
int noexpsize;			// количество цифр целой части числа, в числе представляемым без экспоненты
int digits;				// количество цифр в числе
bool expmode=false;		// число будет представлено в экспоненциальном формате

int noexpdigits=0;
int expdigits=0;

char *Buf=new char[64];
int i,p,prec;

	if (maxsymbols<1) 
	{
		Throw(DE_SizeForDigitsTooSmall);
		LeaveCS();
		return false;
	}
	S[0]='0';
	S[1]=0;
	if (expalways && maxsymbols<3) goto ERR;

	// Для числа 0 отдельная обработка (логифм от нуля не вычислим)
	if (V==0)
	{
		S[0]='0';
		if (precision>0 && precision<MAXPRECISION)
		{
			if (maxsymbols<3) return true;
			S[1]='.';
			if (!expalways)
            {
				for (i=0;i<precision && i<maxsymbols;i++) S[2+i]='0';
                S[2+i]=0;
            }
			else
			{
				for (i=0;i<precision && i<maxsymbols-2;i++) S[2+i]='0';
				S[maxsymbols-2]='e';
				S[maxsymbols-1]='0';
				S[maxsymbols]=0;
			}
		}
		return true;
	}

	// Определяем знак числа и удаляем этот знак из числа
	if (V<0) {sign = 1;	V = -V;}

	// определение степени числа
	{
		double lg = log10(V);
		pw = (lg >=0) ? int(lg) : int(lg)-1;

		// необходимое смещение степени для чисел меньше 1, у которых имеется только одна цифра '1'
		// дело в том, что оператор преобразования в целое число просто "отрезает" дробную часть числа, то есть
		// диапазон от 1 до 1.(9) преобразуется в 1. А для отрицательных значений pw требуется преобразование
		// от 1.(0)1 до 2 - должно быть 2, от 2.(0)1 до 3 - должно быть 3 и т.д.
		if (double(int(lg))==lg && V<1) pw++; 
	}

	// расчет нормализованного числа, умноженного на 10е16
	na = (unsigned __int64)(V*pow(double(10), double(-pw)+16));

	// расчет места, занимаемого экспонентой
	expsize = 1;							// символ экспоненты
	if (pw < 0) expsize++;					// знак экспоненты (символ 'e')
	if (abs(pw) < 10) expsize+=1; else		// экспонента из одной цифры
		if (abs(pw) < 100) expsize+=2; else	// экспонента из двух цифр
			expsize+=3;						// экспонента из трех цифр

	// расчет количества цифр целой части числа, в числе представляемым без экспоненты
	noexpsize = (pw <= 0) ? 1 : pw+1;

	// пренебрегаем последней значащей цифрой числа (как правило, она ошибочна)
	na = na / 10;

	// заполнение буфера цифрами числа (без экспоненты, знака минуса и запятой)
	for (i=0; i<16; i++)
	{
		Buf[15-i] = unsigned __int8(na - (na/10)*10) +48;
		na = na / 10;
	}

	// удаление лишних нулей и определение количества значащих цифр числа
	digits = 16;
	for (i=15;i>0;i--)
		if (Buf[i]=='0' && i>pw) 
		{
			Buf[i]=0;
			digits = i;
		} else
			break;

	// устанавливаем признак отображения числа в экспоненциальном формате
	expmode = expalways; 

	// если число меньше нуля, и количество нулей слева от значащих цифр вместе
	// со знаком и точкой меньше доступного количества символов, то отображение числа
	// может быть только в экспоненциальном формате
	if (!expmode && pw<0 && -pw+noexpsize+sign+1>maxsymbols) expmode=true; 

	// если число больше нуля, и количество нулей справа от значащих цифр вместе
	// со знаком больше доступного количества символов, то отображение числа
	// возможно быть также только в экспоненциальном формате
	if (!expmode && pw>=0 && noexpsize+sign>maxsymbols) expmode=true; 

	if (!expmode && noexpsize+sign<=maxsymbols && (precision>16 || pw>=-precision))
	{
		// количество значащих цифр числа, которые могут быть использованы для представления
		// числа без экспоненты в указанном размере строки

		noexpdigits = noexpsize;
		if (pw<0) noexpdigits=0;

		// количество символов, занимаемых дробной частью числа
		int x = (digits - noexpsize < 0 ? 0 : digits - noexpsize) + ( pw<-1 ? -pw-1 : 0) + (pw<0?1:0); 

		if (maxsymbols - noexpsize > 1)
		{
			if (x>precision) x=precision;
			if (x>maxsymbols - noexpsize - 1) x = maxsymbols - noexpsize - 1;
			if (pw<1) x-= -pw-1;
			noexpdigits += x;
		}
	}

	// количество значащих цифр числа, которые могут быть использованы для представления
	// числа с экспонентой в указанном размере строки
	expdigits = maxsymbols-sign-expsize;
	if (expdigits>=2) expdigits--; // если можно запихать только два символа, то один из них - точно точка
	if (expdigits>digits) expdigits=digits; // если количество доступного пбольше количества актуальных цифр
	expdigits = expdigits<0 ? 0 : expdigits;
	if (precision<=16 && expdigits-1>precision)
		expdigits = precision+1;

	if (!noexpdigits && !expdigits) goto ERR; // если число не влазит в указанный размер

	int rounddigits = noexpdigits;
	if (noexpdigits < expdigits) 
	{
		rounddigits = expdigits;
		expmode = true;
	}

	if (rounddigits<digits)
	{
		// Округляем число до rounddigits цифр. При этом количество значащих цифр может стать меньше.
		if (Buf[rounddigits]-48>=5)
			Buf[rounddigits-1]++;
		digits = rounddigits;
		for (i=rounddigits-1;i>0;i--)
		{
			if (Buf[i]-48>9)
			{
				Buf[i-1]++; 
				digits=i;
				Buf[i]=0;
			}
			else
				break;
		}
		Buf[rounddigits]=0;

		// Необходимая коррекция, для чисел, старшая цифра которых после округления становится больше 9
		if (Buf[0]-48>9)
		{
			digits=1;
			Buf[0]='1';
			Buf[1]=0;
			pw++;
		}

		// повторное удаление лишних нулей и определение количества значащих цифр числа
		//for (i=digits-1;i>0;i--)
		//	if (Buf[i]=='0' && (i>pw || expmode)) 
		//	{
		//		Buf[i]=0;
		//		digits = i;
		//	} else
		//		break;

		// Если после округления число умещается в строку как число без экспоненты, то меняем режим
		if (noexpdigits>=digits && !expalways)
			expmode = false;
	}
		
	i=p=0;
	prec=-1;
	if (sign) S[p++]='-';

	if (expmode)
	{
		S[p++]=Buf[i++];
		if ((digits>1 || precision<=16) && maxsymbols-sign-expsize-2>0)
		{
			if (precision>0) S[p++]='.';
			while (p<maxsymbols-expsize && i<digits)
			{
				if (precision<=16 && i-1>=precision)
					break;
				S[p++]=Buf[i++];
			}
			while (precision<MAXPRECISION && i-1<precision && p+expsize<maxsymbols)
			{
				S[p++]='0';
				i++;
			}
		}
		S[p++]='e';
		if (pw<0) 
		{
			S[p++]='-';
			pw = -pw;
		}
		if (pw>=100)
			S[p++] = char((pw/100) + 48);
		if (pw>=10)
			S[p++] = char(((pw - (pw/100)*100)/10) + 48);
		S[p++] = char(pw - (pw/10)*10 + 48);
	} else
	{
		prec=-1;
		// Если требуются нули в начале числа
		while (pw<0)
		{
			prec++;
			S[p++]='0';
			if (precision>0 && prec==0) S[p++]='.';
			pw++;
			if (prec>=precision)
				break;
		}
		while (p<maxsymbols && i<digits)
		{
			if (prec>=precision)
				break;
			S[p++]=Buf[i++];
			if (prec>=0)
				prec++;
			if (pw>-1 && prec<0 && i!=digits)
			{
				pw--;
				if (pw==-1 && p==maxsymbols-1)
					break;
				if (pw==-1 && p<maxsymbols-1)
				{
					prec=0;
					S[p++]='.';
				}
			}
		}
		// Если требуются нули в конце числа
		while (pw>16)
		{
			S[p++]='0';
			pw--;
		}

	}

	// Заполнение нулями слева при необходимости
	if (zerofill && p<maxsymbols)
	{
		int zerocount = maxsymbols-p;
		for (int i=p-1;i>=0;i--)
			S[i+maxsymbols-p]=S[i];
		memset(S+sign, '0', zerocount);
		if (sign) S[0]='-';
		S[maxsymbols]=0;
	} else
	if (rightalign && p<maxsymbols)
	{
		int spacecount = maxsymbols-p-sign;
		for (int i=p-1;i>=0;i--)
			S[i+maxsymbols-p]=S[i];
		memset(S, ' ', spacecount);
		if (sign) S[spacecount]='-';
		S[maxsymbols]=0;
	} else
		S[p++]=0;
	delete Buf;
	return true;

ERR:
	for (i=0;i<maxsymbols;i++) S[i]='*';
	S[maxsymbols]=0;
	delete Buf;
	LeaveCS();
	Throw(DE_SizeForDigitsTooSmall);
	return false;
}

unsigned __int8 xDataBaseDll::DeterminateTypeOfDataInString() const
{
unsigned __int8 ret = 0;
char* Str;
unsigned __int32 i;
xData d;

	if (DataType!=DT_STRING || !DataSize) return DT_UNDEFINE;
	if (DataType&0x800000)
	{
		d.CopyData(*this,true);
		Str = (char*)d;
	} else
		Str = (char*)GetThisDataPointer();

// Поиск первого символа
	for (i=0;i<DataSize;i++)
		if (*(Str+i)!=' ') break;

// Если первый символ не число, то и тип не число
	if (strchr("0123456789-",(int)*(Str+i))==NULL)
		return DT_UNDEFINE;
	if (*(Str+i)=='-') i++;
	if (i==DataSize-1) return DT_UNDEFINE;

// Наверняка это число
	ret = DT_INT32;

// Ищем, где заканчиваются цифры
	for (i++;i<DataSize-1;i++)
		if (strchr("0123456789",(int)*(Str+i))==NULL)
			break;
	
	if (i==DataSize-1) return ret;

// Если после цифр идет точка, то это наверняка вещественное число
	if (*(Str+i)=='.') {
		ret = DT_DOUBLE;
		i++;
	}

	if (i==DataSize-1) return ret;
// Если после всего этого есть не цифры, то это хбз
	for (;i<DataSize;i++)
		if (strchr("0123456789",(int)*(Str+i))==NULL)
		{
			if (*(Str+i)==' ')
				break;
			ret = DT_UNDEFINE;
			break;
		}

// Поиск оставшихся "непробелов"
	for (;i<DataSize-1;i++)
		if (*(Str+i)!=' ') 
		{
			ret=DT_UNDEFINE;
			break;
		}

	return ret;
}

xDataBaseDll* xDataBaseDll::ToComplexContainer(unsigned __int32 AddonElementsCount)
{
xDataCore* d;
unsigned __int32 e=0;
    if (!AddonElementsCount) return 0;
	if (Pointer && DataType==DT_UNDEFINE && DataSize==1)
		return ((xDataBaseDll*)GetThisDataPointer())->ToComplexContainer(AddonElementsCount);

	if (DataType==DT_STRING)
        e=1;
    else
        e=DataSize;

	d = (xDataCore*)xDataBaseDll::alloc((e+AddonElementsCount)*sizeof(xDataCore));
	memset(d,0,(e+AddonElementsCount)*sizeof(xDataBaseDll));
	
    if (DataType==DT_UNDEFINE && DataSize==1)
    {
        ((xData*)d)->CopyData(*(xDataBaseDll*)GetThisDataPointer());
        Invalidate(false,false,false);
    } else
	if (DataType==DT_STRING || DataSize==1)
    {
		((xData*)d)->CopyData(*(xData*)this);
        Invalidate(false,true,false);
    } else

	// Если это контейнер, то инициализируем первый набор элементов
	if (DataSize>1)
	{
		if (DataType==DT_UNDEFINE)
		{
			xDataBaseDll* p = (xDataBaseDll*)GetThisDataPointer();
			for (unsigned __int32 i=0;i<e;i++)
				((xData*)d+i)->MoveData(*((xData*)p+i));
		} else
		{
			for (unsigned __int32 i=0;i<e;i++)
			{
				d[i].Exception = Exception;
				*((xDataBaseDll*)d+i) = (*this)[xData(i)];
			}
		}
        xDataBaseDll::free(GetThisDataPointer());
	}
	DataType=DT_UNDEFINE;
	DataSize = e+AddonElementsCount;
	((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(d);
	return (xDataBaseDll*)&d[e];
}

void xDataBaseDll::Push(const void* Data, unsigned __int32 CountOfElements, unsigned __int8 TypeOfData)
{
    if (Pointer && DataType==DT_UNDEFINE && DataSize==1)
    {
        ((xDataBaseDll*)GetThisDataPointer())->Push(Data, CountOfElements, TypeOfData);
        return;
    }
    if (!CountOfElements)
        return;
    if (CountOfElements==1 && DataSize==0 && TypeOfData!=DT_UNDEFINE)
    {
        switch (TypeOfData){
        case DT_ANSISTRING:		*((xData*)this) = (char*)Data; break;
        case DT_UNICODESTRING:	*((xData*)this) = (wchar_t*)Data; break;
	    case DT_BOOL:			*((xData*)this) = *((bool*)Data); break;
	    case DT_INT8:			*((xData*)this) = *((__int8*)Data); break;
	    case DT_INT16:			*((xData*)this) = *((__int16*)Data); break;
	    case DT_INT32:			*((xData*)this) = *((__int32*)Data); break;
	    case DT_INT64:			*((xData*)this) = *((__int64*)Data); break;
	    case DT_UINT8:			*((xData*)this) = *((unsigned __int8*)Data); break;
	    case DT_UINT16:			*((xData*)this) = *((unsigned __int16*)Data); break;
	    case DT_UINT32:			*((xData*)this) = *((unsigned __int32*)Data); break;
	    case DT_UINT64:			*((xData*)this) = *((unsigned __int64*)Data); break;
	    case DT_FLOAT:			*((xData*)this) = *((float*)Data); break;
	    case DT_DOUBLE:			*((xData*)this) = *((double*)Data); break;
        }
        return;
    }
    if ((DataType == TypeOfData || DataSize==0) && TypeOfData!=DT_UNDEFINE)
    {
        int s = GetTypeSize(TypeOfData);
        void* t = xDataBaseDll::alloc(s*(DataSize+CountOfElements));
        void* p = GetThisDataPointer();
        memcpy(t,p,s*DataSize);
        memcpy(((__int8*)t)+s*DataSize,Data,s*CountOfElements);
        CountOfElements += DataSize;
        Invalidate(false,false,false);
        ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(t);
        DataType = TypeOfData;
        DataSize = CountOfElements;
        return;
    }
    xData* t = (xData*)ToComplexContainer(CountOfElements);
    for (unsigned __int32 i = 0; i<CountOfElements; i++)
    {
        switch(TypeOfData){
        case DT_UNDEFINE:       t[i].CopyData(*(((xData*)Data)+i)); break;
	    case DT_BOOL:			t[i] = *(((bool*)Data)+i); break;
	    case DT_INT8:			t[i] = *(((__int8*)Data)+i); break;
	    case DT_INT16:			t[i] = *(((__int16*)Data)+i); break;
	    case DT_INT32:			t[i] = *(((__int32*)Data)+i); break;
	    case DT_INT64:			t[i] = *(((__int64*)Data)+i); break;
	    case DT_UINT8:			t[i] = *(((unsigned __int8*)Data)+i); break;
	    case DT_UINT16:			t[i] = *(((unsigned __int16*)Data)+i); break;
	    case DT_UINT32:			t[i] = *(((unsigned __int32*)Data)+i); break;
	    case DT_UINT64:			t[i] = *(((unsigned __int64*)Data)+i); break;
	    case DT_FLOAT:			t[i] = *(((float*)Data)+i); break;
	    case DT_DOUBLE:			t[i] = *(((double*)Data)+i); break;
        case DT_ANSISTRING:		t[i] = (char*)Data; break;
        case DT_UNICODESTRING:	t[i] = (wchar_t*)Data; break;
        }
    }
}
//
//bool IsDigit(char Ch, bool HexDigit)
//{
//    switch (Ch){
//        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': case '+': case '-':
//            return true;
//        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': 
//            if (HexDigit)
//                return true;
//        default: break;
//    }
//    return false;
//}
//
xData& xDataBaseDll::ToNumber(unsigned __int8 Type)
{
using namespace std;
bool HexMode=false;
    if (DataType==DT_IDREFLECTOR || DataType==Type)
        return *(xData*)this;
    if (DataType!=0 && DataType<=DT_DOUBLE && Type!=0 && Type<=DT_DOUBLE)
    {
        switch (Type)
        {
        case DT_INT8: (*(xData*)this) = (__int8)*(xData*)this;break;
        case DT_INT16: (*(xData*)this) = (__int16)*(xData*)this;break;
        case DT_INT32: (*(xData*)this) = (__int32)*(xData*)this;break;
        case DT_INT64: (*(xData*)this) = (__int64)*(xData*)this;break;
        case DT_UINT8: (*(xData*)this) = (unsigned __int8)*(xData*)this;break;
        case DT_UINT16: (*(xData*)this) = (unsigned __int16)*(xData*)this;break;
        case DT_UINT32: (*(xData*)this) = (unsigned __int32)*(xData*)this;break;
        case DT_UINT64: (*(xData*)this) = (unsigned __int64)*(xData*)this;break;
        case DT_FLOAT: (*(xData*)this) = (float)*(xData*)this;break;
        case DT_DOUBLE: (*(xData*)this) = (double)*(xData*)this;break;
        }
        return *(xData*)this;
    }
    if (!((xData*)this)->IsString())
        return *(xData*)this;
    xData* o = (xData*)this;
    if (DataSize&0x800000)
    {
        o = new xData;
        *o=*(xData*)this;
    }
    string value;
    string r;
    value = o->operator char *();
    if (DataSize&0x800000)
        delete o;
    const char* str = value.data();
    Invalidate();
    while (true)
    {
        if (!HexMode && (*str=='h' || *str=='H'))
        {
            HexMode =true;
            str++;
            continue;
        }
        if ((*str==' ' || *str==',' || *str==';' || *str==0) && !r.empty())
        {
            xData tp;
            //if (DataSize==1)
            //{
            //    xaData d = *(xData*)this;
            //    Invalidate();
            //    switch (Type)
            //    {
            //    case DT_INT8: (*(xData*)this) << (__int8)d;break;
            //    case DT_INT16: (*(xData*)this) << (__int16)d;break;
            //    case DT_INT32: (*(xData*)this) << (__int32)d;break;
            //    case DT_INT64: (*(xData*)this) << (__int64)d;break;
            //    case DT_UINT8: (*(xData*)this) << (unsigned __int8)d;break;
            //    case DT_UINT16: (*(xData*)this) << (unsigned __int16)d;break;
            //    case DT_UINT32: (*(xData*)this) << (unsigned __int32)d;break;
            //    case DT_UINT64: (*(xData*)this) << (unsigned __int64)d;break;
            //    case DT_FLOAT: (*(xData*)this) << (float)d;break;
            //    case DT_DOUBLE: (*(xData*)this) << (float)d;break;
            //    }
            //}
            if (HexMode)
            {
                char* p;
                if (Type==DT_INT8 || Type==DT_INT16 || Type==DT_INT32 || Type==DT_INT64)
                    tp = _strtoi64(r.data(),&p,16);
                else
                    tp = _strtoui64(r.data(),&p,16);
                tp.ToNumber(Type);
            } else
            {
                tp = r.data();
                switch (Type)
                {
                case DT_INT8: tp = (__int8)tp;break;
                case DT_INT16: tp = (__int16)tp;break;
                case DT_INT32: tp = (__int32)tp;break;
                case DT_INT64: tp = (__int64)tp;break;
                case DT_UINT8: tp = (unsigned __int8)tp;break;
                case DT_UINT16: tp = (unsigned __int16)tp;break;
                case DT_UINT32: tp = (unsigned __int32)tp;break;
                case DT_UINT64: tp = (unsigned __int64)tp;break;
                case DT_FLOAT: tp = (float)tp;break;
                case DT_DOUBLE: tp = (double)tp;break;
                }
            }
            if (DataSize>0)
            {
                switch (Type)
                {
                case DT_INT8: (*(xData*)this) << (__int8)tp;break;
                case DT_INT16: (*(xData*)this) << (__int16)tp;break;
                case DT_INT32: (*(xData*)this) << (__int32)tp;break;
                case DT_INT64: (*(xData*)this) << (__int64)tp;break;
                case DT_UINT8: (*(xData*)this) << (unsigned __int8)tp;break;
                case DT_UINT16: (*(xData*)this) << (unsigned __int16)tp;break;
                case DT_UINT32: (*(xData*)this) << (unsigned __int32)tp;break;
                case DT_UINT64: (*(xData*)this) << (unsigned __int64)tp;break;
                case DT_FLOAT: (*(xData*)this) << (float)tp;break;
                case DT_DOUBLE: (*(xData*)this) << (double)tp;break;
                }
            }
            else
                (*(xData*)this) = tp;
            r.clear();
        }
        else
            if (*str!=' ')
                r+=*str;
        if (*str==0)
            break;
        str++;
    }
    return *(xData*)this;
}