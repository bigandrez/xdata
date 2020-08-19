#include "stdafx.h"
#include "stdlib.h"
#include "xData.h"
#include "xDataBaseDll.h"

void xDataBaseDll::Invalidate(bool AsPointered, bool ResetName, bool ResetCS)
{
	((xData*)this)->EnterCS();
	if (!Pointer && DataType==DT_UNDEFINE && DataSize>0)
	{
		// удаление комплексного контейнера
		xData* p = (xData*)((xData*)this)->GetThisDataPointer();
		for (unsigned __int32 i=0;i<DataSize;i++)
			p[i].Invalidate(false,true,true);
	}
	if (!Pointer)
	{
        if (DataSize==1)
        {
		    switch (DataType)
		    {
			    case DT_STRING:
				    if (!(DataSize&0x7FFFFF)) break;
			    case DT_UNDEFINE:
			    case DT_INT64:
			    case DT_UINT64:
			    case DT_DOUBLE:
			    case DT_IDREFLECTOR:
			    case DT_CONTROLDATA:
				    if (((xData*)this)->GetThisDataPointer())
					    free(((xData*)this)->GetThisDataPointer());
			    default:
				    break;
		    }
        }
        if (DataSize>1)
            free(((xData*)this)->GetThisDataPointer());
	}
	Pointer = AsPointered;
	void *v = NULL; // Указатель на расширенную структуру, если она необходима
	if (CS || Named)
	{
		char* n = NULL;
		int len = 0;
		LPCRITICAL_SECTION c = NULL;
		if (Named & !ResetName)
		{
			if (CS)
				n = ((xData*)this)->AsValueCs()->Name;
			else
				n = ((xData*)this)->AsValue()->Name;
			if (n)
				len = ansitextlength(n)+1;
		} else
			Named=0;
		if (CS)
		{
			c = (LPCRITICAL_SECTION)&((xData*)this)->AsValueCs()->cs;
			if (ResetCS)
			{
				::DeleteCriticalSection(c);
				c = NULL;
				CS=0;
			}
		}
		if ( c || n )
		{
			 v = new __int8[(c ? sizeof(xValueCs) : sizeof(xValue)) + len];
			if (c)
				memcpy(&((xValueCs*)v)->cs, c, sizeof(CRITICAL_SECTION));
			if (n)
				memcpy( c ? ((xValueCs*)v)->Name : ((xValue*)v)->Name, n, len);
		}
		free(((xData*)this)->AsValueCs());
	}
	if (v)
		DataValue = unsigned __int32(v) - unsigned __int32(this);
	else
		DataValue=0;
	DataSize  = 0;
	DataType  = DT_UNDEFINE;
	Pointer = AsPointered;
	((xData*)this)->LeaveCS();
}

void* xDataBaseDll::alloc(size_t size)
{
	return new __int8[size];
}

void xDataBaseDll::free(void* ptr)
{
	delete (__int8*)ptr;
}

char* xDataBaseDll::GetName() const
{
    if (!Named && DataType==DT_UNDEFINE && DataSize==1)
        return ((xDataBaseDll*)((xData*)this)->GetThisDataPointer())->GetName(); 
	if (!Named || !DataValue) return NULL;
	if (CS)
		return ((xData*)this)->AsValueCs()->Name;
	return ((xData*)this)->AsValue()->Name;
}

void xDataBaseDll::SetName(const char* Name)
{
void* v=NULL;
unsigned __int32 len = 0;

	xDataBaseDll* ref = ((xData*)this)->GetReference();
	if (ref)
	{
		ref->SetName(Name);
		return;
	}
	// Если имени нет, и его нужно сбросить
	if (!Name && !Named)
		return;

	((xData*)this)->EnterCS();
	if (Name)
		len = ansitextlength(Name)+1;

	if (CS)
	{
		v = alloc(sizeof(xValueCs)+len);
		memcpy( &((xValueCs*)v)->cs, &((xValueCs*)(unsigned __int32(this)+DataValue))->cs, sizeof(CRITICAL_SECTION));
		memcpy( &((xValueCs*)v)->Name, Name, len);
        ((xValueCs*)v)->ExtDataValue = ((xValueCs*)(unsigned __int32(this)+DataValue))->ExtDataValue;
		free(((xValue*)(unsigned __int32(this)+DataValue)));
		DataValue = CREATELINK(v);
	} else
	{
		if (Name)
		{
			v = alloc(sizeof(xValue)+len);
			memcpy( &((xValue*)v)->Name, Name, len);
			if (!Named)
				((xValue*)v)->ExtDataValue = DataValue;
			else
			{
				((xValue*)v)->ExtDataValue = ((xValue*)(unsigned __int32(this)+DataValue))->ExtDataValue;
				free(((xValue*)(unsigned __int32(this)+DataValue)));
			}
			DataValue = CREATELINK(v);
		} else
		{
			unsigned __int32 val = ((xValue*)(unsigned __int32(this)+DataValue))->ExtDataValue;
			free(((xValue*)(unsigned __int32(this)+DataValue)));
			DataValue = val;
		}
	}

	Named = Name ? 1 : 0;
	((xData*)this)->LeaveCS();
}

bool xDataBaseDll::CompareWithName(const char* Name) const
{
    if (Named)
    {
        char* name = GetName();
	    unsigned __int32 len = ansitextlength(name)+1;
	    if (!memcmp(name, Name, len)) return true;
    }
	if (DataType==DT_UNDEFINE && DataSize==1)
		return ((xDataBaseDll*) (((xData*)this)->GetThisDataPointer()))->CompareWithName(Name);
	return false;
}

bool xDataBaseDll::CompareWithThisName(const char* Name) const
{
    if (!Named)
        return false;
    char* name = GetName();
    unsigned __int32 len = ansitextlength(name)+1;
    if (!memcmp(name, Name, len)) return true;
	return false;
}

xDataBaseDll& xDataBaseDll::CopyData(const xDataBaseDll& V, bool RemovePointers)
{
    if (RemovePointers && ((xData*)&V)->IsReference())
	{
        CopyData( *(xDataBaseDll*)((xData*)&V)->GetReference(), RemovePointers);
		return *this;
	}
    Invalidate(false,false,false);
	if (V.Named)
		SetName(V.GetName());
	if (!V.DataSize)
		return *this;
	if (!RemovePointers && V.Pointer)
	{
		DataSize = V.DataSize;
		Pointer = 1;
		DataType = V.DataType;
		((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK( ((xData*)&V)->GetThisDataPointer());
		return *this;
	}
	DataSize = V.DataSize;
	DataType = V.DataType;
	if (V.DataType==DT_UNDEFINE)
	{
		unsigned __int32 i;
		xDataBaseDll* t = (xDataBaseDll*)alloc(V.DataSize*sizeof(xDataCore));
		memset(t,0,V.DataSize*sizeof(xDataCore));
		xDataBaseDll* s = (xDataBaseDll*)((xData*)&V)->GetThisDataPointer();
		for (i=0;i<V.DataSize;i++)
			t[i].CopyData(s[i], RemovePointers);
		if (t) ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(t);
		return *this;
	}
	if (V.DataType==DT_STRING)
	{
		void* t;
		if (DataSize&0x800000)
		{
			t = alloc((V.DataSize&0x7fffff)*sizeof(wchar_t));
			memcpy(t,((xData*)&V)->GetThisDataPointer(),(V.DataSize&0x7fffff)*sizeof(wchar_t));
		} else
		{
			t = alloc(V.DataSize*sizeof(char));
			memcpy(t,((xData*)&V)->GetThisDataPointer(),V.DataSize*sizeof(char));
		}
		if (t) ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(t);
		return *this;
	}
	if (V.DataType==DT_IDREFLECTOR)
	{
        void* t = alloc(V.DataSize*sizeof(xReflectorData));
        memcpy(t,((xData*)&V)->GetThisDataPointer(),V.DataSize*sizeof(xReflectorData));
        if (t) ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(t);
		return *this;
	}
	if (V.DataType==DT_CONTROLDATA)
	{
		return *this;
	}
	if (DataSize>1 || DataType==DT_INT64 || DataType==DT_UINT64 || DataType==DT_DOUBLE)
	{
		void* t = alloc(V.DataSize*((xData*)this)->GetTypeSize(DataType));
		memcpy(t,((xData*)&V)->GetThisDataPointer(),V.DataSize*((xData*)this)->GetTypeSize(DataType));
		if (t) ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(t);
	} else
    {
        if (V.Pointer)
        {
            switch (DataType){
                case DT_BOOL: ((CS || Named) ? EXTDATAVALUE : DataValue) = *(bool*)((xData*)&V)->GetThisDataPointer();break;
                case DT_INT8: ((CS || Named) ? EXTDATAVALUE : DataValue) = *(__int8*)((xData*)&V)->GetThisDataPointer();break;
                case DT_UINT8: ((CS || Named) ? EXTDATAVALUE : DataValue) = *(unsigned __int8*)((xData*)&V)->GetThisDataPointer();break;
                case DT_INT16: ((CS || Named) ? EXTDATAVALUE : DataValue) = *(__int16*)((xData*)&V)->GetThisDataPointer();break;
                case DT_UINT16: ((CS || Named) ? EXTDATAVALUE : DataValue) = *(unsigned __int16*)((xData*)&V)->GetThisDataPointer();break;
                case DT_INT32: 
                    *(unsigned __int32*)((xData*)&V)->GetThisDataPointer();
                    ((CS || Named) ? EXTDATAVALUE : DataValue) = *(unsigned __int32*)((xData*)&V)->GetThisDataPointer();
                    break;
                case DT_UINT32: ((CS || Named) ? EXTDATAVALUE : DataValue) = *(unsigned __int32*)((xData*)&V)->GetThisDataPointer();break;
                case DT_FLOAT: ((CS || Named) ? EXTDATAVALUE : DataValue) = *(unsigned __int32*)((xData*)&V)->GetThisDataPointer();break;
            }
        }
        else
		    ((CS || Named) ? EXTDATAVALUE : DataValue) = ((V.CS || V.Named) ? (((xValue*)(V.DataValue+unsigned __int32(&V)))->ExtDataValue) : V.DataValue);
    }
	return *this;
}

char* xDataBaseDll::GetDebugObjectDescriptor() const
{
	return "asdfaljk";
}

xData xDataBaseDll::GetObjectDescriptor(bool UseCrLf, int level) const
{
xData ret="";
xData type;
xData name;
char CRLF[3]={0x0d,0x0a,0};
//	if (((xData*)this)->IsEmpty()) return "Empty";
	switch (DataType)
	{
		case DT_UNDEFINE:	type = "DT_UNDEFINE"; break;
		case DT_BOOL:		type = "DT_BOOL"; break;
		case DT_INT8:		type = "DT_INT8"; break;
		case DT_INT16:		type = "DT_INT16"; break;
		case DT_INT32:		type = "DT_INT32"; break;
		case DT_INT64:		type = "DT_INT64"; break;
		case DT_UINT8:		type = "DT_UINT8"; break;
		case DT_UINT16:		type = "DT_UINT16"; break;
		case DT_UINT32:		type = "DT_UINT32"; break;
		case DT_UINT64:		type = "DT_UINT64"; break;
		case DT_FLOAT:		type = "DT_FLOAT"; break;
		case DT_DOUBLE:		type = "DT_DOUBLE"; break;
		case DT_STRING:		type = "DT_STRING"; break;
		case DT_CONTROLDATA:type = "DT_CONTROLDATA"; break;
		case DT_IDREFLECTOR:type = "DT_IDREFLECTOR"; break;
	}
	
    for (int i=0;i<level;i++) ret += "   ";

	if (Named)
	{
		ret += "[";
		if (DataType==DT_IDREFLECTOR)
			ret += ((xData*)this)->AsReflectorData()->Name;
        else
		if (DataType==DT_CONTROLDATA)
			ret += ((xData*)this)->AsControlData()->Name;
        else
        if (CS)
            ret += ((xData*)this)->AsValueCs()->Name;
        else
            ret += ((xData*)this)->AsValue()->Name;
		ret += "]";
	}

	if (Pointer)
	{
		if (!Named)
			ret += "Pointer(";
		else
			ret += " pointer(";
		ret += type;
        ret +=")";
	} else
	{
		if (!Named)
			ret += type;
		else
		{
			ret += " ";
			ret += type;
		}
	}

    if (DataType==DT_STRING)
        ret += (DataSize&0x800000) ? ", unicode" : ", ansi";
	if (Exception) 
		ret += ", exception";
	if (CS) ret += ", CS";
	ret += ", size=%i";
    if (DataType!=DT_STRING)
        ret += DataSize;
    else
        ret += DataSize&0x7fffff;

    if (DataType==DT_UNDEFINE)
    {
        for (unsigned int i=0;i<DataSize;i++)
        {
            ret += UseCrLf ? CRLF : "\n";
			xDataBaseDll& V = *(((xDataBaseDll*)((xData*)this)->GetThisDataPointer())+i);
            ret += V.GetObjectDescriptor(UseCrLf, level+1);
        }
        return ret;
    } else
    if (DataSize==1 || DataType==DT_STRING)
    {
        xData t = *(xData*)this;
		ret += ", value=";
		ret += (const char*)t;
        return ret;
    }

    ret += UseCrLf ? CRLF : "\n";
    for (int i=0;i<level+1;i++) ret += "   ";

	for (unsigned __int32 i=0;i<DataSize;i++)
	{
		if (i!=0) ret +=", ";
        xData s = ((xData*)this)->operator [](xData(i));
        ret += (const char*)s;
	}

//	if (GetThisDataType()==DT_BOOL)
//	{
//		ret += " value=";
//		t = (*this);
//		ret += t;
//	}
//	if (!Container && (IsNumber() || IsAnsiString() || IsUnicodeString()))
//	{
//		ret += " value=";
//		t = (*this);
//		ret += t;
//	}
//
//	if (Container && IsNumber())
//	{
//		if (UseCrLf) ret += CRLF; else ret += "\n";
//		for (int i=0;i<level;i++) ret += "   ";
//		ret += "   ";
//
//		for (int i=0;i<(int)GetContainerSize();i++)
//		{
//			if (i!=0) ret +=", ";
//			t = (*this)[i];
//			ret += t;
//		}
//	}
//	if (IsReference())
//	{
//		if (!IsPointerValid())
//		{
//			ret += "::ERROR-EMPTY POINTER::";
//			if (UseCrLf) ret += CRLF; else ret += "\n";
//		} else
//		{
//			if (UseCrLf) ret += CRLF; else ret += "\n";
//			for (int i=0;i<level;i++) ret += "   ";
//			ret += "   ";
//			ret += ((xData*)GetThisDataPointer())->GetObjectDescriptor(UseCrLf, level+1);
//		}
//	}
//	if (Container && !DataType)
//	{
//		for (int i=0;i<(int)GetContainerSize();i++)
//		{
//			if (UseCrLf) ret += CRLF; else ret += "\n";
//			for (int j=0;j<level;j++) ret += "   ";
//			ret += "   ";
//			ret += "[";
//			ret += i;
//			ret += "]";
//			ret += (i+(xData*)GetThisDataPointer())->GetObjectDescriptor(UseCrLf, level+1);
//		}
//	}
//	//if (Pointer)
//	//{
//	//	ret += " pointer to 0x%08.16i";
//	//	ret += (unsigned __int32)GetThisDataPointer();
//	//}

	return ret;
}


xaData xDataBaseDll::GetSimpleValue(const xData& Index, const xData& DefValue) const
{
    if ( ((xData*)this)->IsReference() )
        return ((xDataBaseDll*)((xData*)this)->GetReference())->GetSimpleValue(Index, DefValue);
    if (&Index)
    {
        xData d(operator [](Index));
        if (d.DataType>=DT_BOOL && d.DataType <=DT_STRING)
            return d;
        if (&DefValue)
            return xData(DefValue);
        return xData();
    }
    if (DataType>=DT_BOOL && DataType <=DT_STRING)
        return xData(*(xData*)this);
    if (&DefValue)
        return xData(DefValue);
    return xData();
}

unsigned __int32 xDataBaseDll::GetLinearObject(void * buf, unsigned __int32 size)
{
    unsigned __int32 c = (DataType==DT_UNDEFINE && DataSize>0) ? DataSize : 0;
    if (size<(c+1)*sizeof(xData)) return -1;
	__int8* b = ((__int8*)buf)+8;
    unsigned __int32 us = SaveToLinear((__int8*)buf, b, ((__int8*)buf)+8+c*sizeof(xData), size-(c+1)*sizeof(xData) );
    if (us==-1) return 0;
	return us+sizeof(xData);
}

	// Вспомогательная функция для сохранения объекта в линейный буфер
unsigned __int32 xDataBaseDll::SaveToLinear(__int8* Buffer,__int8* ChildBuffer,__int8* Data, unsigned __int32 size) 
{
unsigned __int32 UsedSize=0;
xData *Object = ((xData*)Buffer);

    ((xData*)this)->EnterCS();

	// Сохранение базовых признаков
	*(__int64*)Object = *(__int64*)this;
	Object->Pointer = 0;
	Object->Exception = 0;
	Object->CS = 0;

    if (Named)
    {
		Object->DataValue = unsigned __int32(Data)-unsigned __int32(Object);
		unsigned __int32 NameLen = ansitextlength(GetName(), 1024)+1;
        if (size<NameLen+sizeof(xValue)) return -1;
		memcpy(Data, (char*)(DataValue+unsigned __int32(this)), sizeof(xValue)+NameLen);
		UsedSize += sizeof(xValue)+NameLen;
    }
	if (DataSize && DataType==DT_UNDEFINE)
	{
		// Сохранение комплексного контейнера
		xDataBaseDll* v = (xDataBaseDll*)((xData*)this)->GetThisDataPointer();
		// Данные контейнера находятся на shift*8 байт дальше
        Object->SetThisDataPointer(ChildBuffer);
		for (unsigned __int32 i=0;i<DataSize;i++)
		{
            unsigned __int32 us = v->SaveToLinear(ChildBuffer+i*sizeof(xData), Data+UsedSize, 
                Data+ (v->DataType==DT_UNDEFINE?v->DataSize*sizeof(xData):0)+UsedSize,
                size-(v->DataType==DT_UNDEFINE?v->DataSize*sizeof(xData):0)-UsedSize);
            if (us==-1) return -1;
	        UsedSize += us;
			v++;
		}
		((xData*)this)->LeaveCS();
		return UsedSize+DataSize*sizeof(xData);
	}
	if (((xData*)this)->ThisIsAnsiString() || ((xData*)this)->IsUnicodeString() || DataSize>1 || DataType==DT_DOUBLE || DataType==DT_INT64 || DataType==DT_UINT64)
	{
		Object->SetThisDataPointer(UsedSize+Data);
        unsigned __int32 s;
        if (DataType==DT_STRING)
            s = (DataSize&0x7FFFFF)*(DataSize&0x800000?2:1);
        else
            s = ((xData*)this)->GetTypeSize(DataType)*DataSize; // Количество байт
        if (size<s) return -1;
		memcpy(Data+UsedSize, ((xData*)this)->GetThisDataPointer(), s);
        UsedSize += s;
		((xData*)this)->LeaveCS();
		return UsedSize;
	}

    ((xData*)this)->LeaveCS();
    return UsedSize;
}

unsigned __int32 xDataBaseDll::GetLinearSize() const
{
unsigned __int32 ret=8;
    if (Named)
		ret += ansitextlength(GetName(), 1024)+1+sizeof(xValue);
    if (DataType==DT_STRING)
        return ret+ (DataSize&0x7FFFFF)*(DataSize&0x800000?2:1);
    if (DataType==DT_UNDEFINE)
    {
		xDataBaseDll* v = (xDataBaseDll*)((xData*)this)->GetThisDataPointer();
		for (unsigned __int32 i=0;i<DataSize;i++)
            ret += v[i].GetLinearSize();
    } else
        if (((xData*)this)->GetTypeSize(DataType)>4 || DataSize>1)
            ret+= ((xData*)this)->GetTypeSize(DataType)*DataSize; // Количество байт
    return ret;
}