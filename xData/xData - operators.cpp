#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include "xData.h"
#include "xDataBaseDll.h"

#define GET_THIS_DATA_POINTER() (((xData*)this)->GetThisDataPointer())
#define GET_REFERENCE() ( (xDataBaseDll*) ((xData*)this)->GetReference() )

#define EnterCS() (((xData*)this)->EnterCS())
#define LeaveCS() (((xData*)this)->LeaveCS())
#define ThisIsAnsiString() (((xData*)this)->ThisIsAnsiString())
#define IsUnicodeString() (((xData*)this)->IsUnicodeString())
//#define IsNumber() (((xData*)this)->IsNumber())
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

xDataBaseDll& xDataBaseDll::operator=(const xDataBaseDll &V)
{
	if (Pointer)
	{
		if (DataSize>1)
		{
			Throw(DE_EqualToArray);
			return *this;
		}
		switch (DataType)
		{
		case DT_UNDEFINE:    
            if (DataSize==0)
                CopyData(V);
            else
                ((xDataBaseDll*)GET_THIS_DATA_POINTER())->operator=(V); break;
        case DT_INT8:		 *(__int8*)GET_THIS_DATA_POINTER()=V;              break;
		case DT_INT16:		 *(__int16*)GET_THIS_DATA_POINTER()=V;             break;
		case DT_INT32:		 *(__int32*)GET_THIS_DATA_POINTER()=V;             break;
		case DT_INT64:	     *(__int64*)GET_THIS_DATA_POINTER()=V;             break;
		case DT_UINT8:		 *(unsigned __int8*)GET_THIS_DATA_POINTER()=V;     break;
		case DT_UINT16:		 *(unsigned __int16*)GET_THIS_DATA_POINTER()=V;    break;
		case DT_UINT32:		 *(unsigned __int32*)GET_THIS_DATA_POINTER()=V;    break;
		case DT_UINT64:		 *(unsigned __int64*)GET_THIS_DATA_POINTER()=V;    break;
		case DT_FLOAT:		 *(float*)GET_THIS_DATA_POINTER()=V;               break;
		case DT_DOUBLE:		 *(double*)GET_THIS_DATA_POINTER()=V;              break;
		default:                                         	                break;
        }
		return *this;
	}
	if (!IsEmpty())
		Invalidate(false,false,false);
	CopyData(V, Pointer==0);
	return *this;
}

void xDataBaseDll::Remove(const xData &index)
{
    if (((xData*)this)->IsReference())
        return GET_REFERENCE()->Remove(index);
    if (Pointer) return;
unsigned __int32 numindex=-1;
    if (index.DataType==DT_STRING)
    {
        xDataBaseDll IND(index);
        char* ind = IND;
        if (DataType==DT_UNDEFINE)
        {
            xData* p = ((xData*)GET_THIS_DATA_POINTER());
            for (unsigned __int32 i = 0; i<DataSize; i++)
            {
                if (p[i].CompareWithName(ind))
                {
                    numindex=i;
                    break;
                }
            }
        }
    }
    if ( ((const xData*)&index)->IsNumber())
        numindex = index;
    if (numindex>DataSize)
        numindex =-1;
    if (numindex ==-1) return;
    xData* p = ((xData*)GET_THIS_DATA_POINTER());
    if (DataType==DT_UNDEFINE)
    {
        p[numindex].Invalidate();
        for (unsigned __int32 i = numindex; i<DataSize-1; i++)
            p[i].MoveData(p[i+1]);
    } else
    {
        memcpy(p,((char*)p)+GetTypeSize(DataType), GetTypeSize(DataType)*(DataSize-numindex-1));
    }
    DataSize--;
}

xaData xDataBaseDll::operator[](const xDataBaseDll &V) const
{

    if (((xData*)this)->IsReference())
        return GET_REFERENCE()->operator [](V); 
    if (((xData*)&V)->IsReference())
        return operator []( *(xDataBaseDll*)((xData*)&V)->GetReference() ); 
    if ((V.DataType!=DT_STRING && (V.DataSize>1 || V.DataSize==0)) || DataSize==0)
        return xData();
    if (V.DataType==DT_STRING)
    {
        xDataBaseDll IND(V);
        char* index = IND;
	    if (CompareWithThisName(index))
            return xpData((xData*)this);
        if (DataType==DT_UNDEFINE)
            for (unsigned __int32 i = 0; i<DataSize; i++)
            {
                xData* p = ((xData*)GET_THIS_DATA_POINTER())+i;
                if (p->CompareWithName(index))
                    return xpData((xData*)p);
            }
        return xData();
    }
    if (V.DataType>=DT_INT8 && V.DataType<=DT_UINT64)
    {
        unsigned __int32 index = V;
        if (DataType==DT_STRING)
        {
            if (index>=(DataSize&0x7FFFFF))
                return xData();
            if (DataSize&0x800000)
                return xpData( *(((unsigned __int16*)GET_THIS_DATA_POINTER())+index));
            return xpData( *(((char*)GET_THIS_DATA_POINTER())+index));
        }
        if (index>=DataSize)
            return xData();
        switch (DataType)
        {
        case DT_UNDEFINE: return xpData( (((xData*)GET_THIS_DATA_POINTER())+index));
        case DT_BOOL: return xpData( *(((bool*)GET_THIS_DATA_POINTER())+index));
        case DT_INT8: return xpData( *(((__int8*)GET_THIS_DATA_POINTER())+index));
        case DT_INT16: return xpData( *(((__int16*)GET_THIS_DATA_POINTER())+index));
        case DT_INT32: return xpData( *(((__int32*)GET_THIS_DATA_POINTER())+index));
        case DT_INT64: return xpData( *(((__int64*)GET_THIS_DATA_POINTER())+index));
        case DT_UINT8: return xpData( *(((unsigned __int8*)GET_THIS_DATA_POINTER())+index));
        case DT_UINT16: return xpData( *(((unsigned __int16*)GET_THIS_DATA_POINTER())+index));
        case DT_UINT32: return xpData( *(((unsigned __int32*)GET_THIS_DATA_POINTER())+index));
        case DT_UINT64: return xpData( *(((unsigned __int64*)GET_THIS_DATA_POINTER())+index));
        case DT_FLOAT: return xpData( *(((float*)GET_THIS_DATA_POINTER())+index));
        case DT_DOUBLE: return xpData( *(((double*)GET_THIS_DATA_POINTER())+index));
        }
    }
    return xData();
}

xDataBaseDll& xDataBaseDll::operator+=(const xDataBaseDll &V)
{
	if (IsEmpty())
		CopyData(V, Pointer==0);
    if (DataType==DT_UNDEFINE && DataSize==1)
        return ((xDataBaseDll*)GET_THIS_DATA_POINTER())->operator +=(V); 

    EnterCS();
    if (DataType==DT_STRING)
    {
        xData arg;
		arg.CopyData(V,true);
//		arg.ToString();
	    unsigned __int8 FormatStringSize=0;
	    unsigned __int32 Size;
        void* FormatString;
        if (!(DataSize&0x800000))
	    {
		    char* NewString;
		    FormatString = GetPointerToFormatString();
		    FormatStringSize = arg.ToString(false,(char*)FormatString);
			if (!FormatString)
				arg.ToString();
		    NewString = (char*)xDataBaseDll::alloc(DataSize-FormatStringSize+arg.DataSize-1);
		    if (FormatStringSize)
		    {
			    Size = unsigned __int32(FormatString)-unsigned __int32(GET_THIS_DATA_POINTER());
			    memcpy(NewString, GET_THIS_DATA_POINTER(), Size);
			    memcpy(NewString+Size, (char*)arg, arg.DataSize-1);
			    memcpy(NewString+Size+arg.DataSize-1, FormatStringSize+(char*)FormatString, DataSize-Size-FormatStringSize);
		    }
		    else
		    {
			    memcpy(NewString, GET_THIS_DATA_POINTER(), DataSize-1);
			    memcpy(NewString+DataSize-1, (char*)arg, arg.DataSize);
		    }
		    DataSize = DataSize-FormatStringSize+arg.DataSize-1;
		    xDataBaseDll::free(GET_THIS_DATA_POINTER());
            ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(NewString);
		    LeaveCS();
		    return *this;
	    }
//		arg.ToString(true);
		wchar_t* NewString;
		FormatString = GetPointerToFormatString();
		FormatStringSize = arg.ToString(true,(char*)FormatString);
		if (!FormatString)
			arg.ToString(true);
		NewString = (wchar_t*)xDataBaseDll::alloc((DataSize&0x7FFFFF)*2-FormatStringSize+(arg.DataSize&0x7FFFFF)*2-2);
		if (FormatStringSize)
		{
			Size = unsigned __int32(FormatString)-unsigned __int32(GET_THIS_DATA_POINTER());
			memcpy(NewString, GET_THIS_DATA_POINTER(), Size);
			memcpy(NewString+Size/2, (wchar_t*)arg, (arg.DataSize&0x7FFFFF)*2-2);
			memcpy(NewString+Size/2+(arg.DataSize&0x7FFFFF)-1, FormatStringSize/2+(wchar_t*)FormatString, (DataSize&0x7FFFFF)*2-Size-FormatStringSize);
		}
		else
		{
			memcpy(NewString, GET_THIS_DATA_POINTER(), (DataSize&0x7FFFFF)*2-2);
			memcpy(NewString+(DataSize&0x7FFFFF)-1, (wchar_t*)arg, (arg.DataSize&0x7FFFFF)*2);
		}
		DataSize = ((DataSize&0x7FFFFF)-FormatStringSize+(arg.DataSize&0x7FFFFF)-1)|0x800000;
		xDataBaseDll::free(GET_THIS_DATA_POINTER());
        ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(NewString);
		LeaveCS();
		return *this;
    }
    if (DataSize==1)
    {
        xDataBaseDll arg = V;
	    switch (DataType){
	    case DT_BOOL:
		    if ( arg.operator bool())
			    (*(bool*)GET_THIS_DATA_POINTER()) = true;
		    break;
	    case DT_INT8:
		    (*(__int8*)GET_THIS_DATA_POINTER()) = (*(__int8*)GET_THIS_DATA_POINTER()) + (__int8)*(const xData*)&arg;
		    break;
	    case DT_INT16:
		    (*(__int16*)GET_THIS_DATA_POINTER()) = (*(__int16*)GET_THIS_DATA_POINTER()) + (__int16)arg;
		    break;
	    case DT_INT32:
		    (*(__int32*)GET_THIS_DATA_POINTER()) += (__int32)arg;
		    break;
	    case DT_INT64:
		    (*(__int64*)GET_THIS_DATA_POINTER()) += (__int64)arg;
	    case DT_UINT8:
		    (*(unsigned __int8*)GET_THIS_DATA_POINTER()) = (*(unsigned __int8*)GET_THIS_DATA_POINTER()) + (unsigned __int8)arg;
		    break;
	    case DT_UINT16:
		    (*(unsigned __int16*)GET_THIS_DATA_POINTER()) = (*(unsigned __int16*)GET_THIS_DATA_POINTER()) + (unsigned __int16)arg;
		    break;
	    case DT_UINT32:
		    (*(unsigned __int32*)GET_THIS_DATA_POINTER()) += (unsigned __int32)arg;
		    break;
	    case DT_UINT64:
		    (*(unsigned __int64*)GET_THIS_DATA_POINTER()) += (unsigned __int64)arg;
	    case DT_FLOAT:
		    (*(float*)GET_THIS_DATA_POINTER()) += (float)arg;
		    break;
	    case DT_DOUBLE:
		    (*(double*)GET_THIS_DATA_POINTER()) += (double)arg;
		    break;
	    }
    }
	LeaveCS();
    return *this;
}

bool xDataBaseDll::operator==(const xDataBaseDll &V) const
{
    if (DataType==DT_UNDEFINE && DataSize==1)
		return ((xDataBaseDll*)GET_THIS_DATA_POINTER())->operator==(V);
    if (V.DataType==DT_UNDEFINE && V.DataSize==1)
		return this->operator==(*(xDataBaseDll*)((xData*)&V)->GetThisDataPointer());

    if (DataType==DT_UNDEFINE && DataSize>1)
    {
        if (V.DataType!=DT_UNDEFINE || DataSize!=V.DataSize)
            return false;
        xData* s1 = (xData*)((xData*)this)->GetThisDataPointer();
        xData* s2 = (xData*)((const xData*)&V)->GetThisDataPointer();
        for (unsigned int i=0;i<DataSize;i++)
        {
            unsigned int j;
            for (j=0;j<V.DataSize;j++)
            {
                if (s1[i]==s2[j] && s1[i].CompareWithName(s2[j].GetName()))
                    break;
            }
            if (j==V.DataSize)
                return false;
        }
        return true;
    }

	if (DataType == DT_BOOL)
	{
        if ( operator bool() == V.operator bool() ) 
			return true; 
		return false;
	}
    if (DataType == DT_STRING)
    {
        if (((xData*)&V)->IsReal())
        {
            if (V.DataType == DT_FLOAT)
            {
                float V = this->operator float();
                if (V!= (float)*((xData*)&V))
                    return false;
                return true;
            }
            if (V.DataType == DT_DOUBLE)
            {
                double v = this->operator double();
                double s = (double)*((xData*)&V);
                if (v!= (double)*((xData*)&V))
                    return false;
                return true;
            }
        }

	    xData d;
	    void *v1,*v2;
	    unsigned int len1=0,len2=1;
        xData VV;
        VV=*(const xData*)&V;
	    if (!(DataSize&0x800000))
	    {
		    v1 = (char*)GET_THIS_DATA_POINTER();
		    len1 = DataSize;
		    if (!(V.DataSize&0x800000))
		    {
			    v2 = (char*)VV;
			    len2 = VV.DataSize;
		    } else
		    {
			    d=*(xData*)&V;
			    v2 = d.operator char*();
			    len2 = d.DataSize;
		    }
	    } else
	    {
		    v1 = (wchar_t*)GET_THIS_DATA_POINTER();
		    len1 = DataSize;
		    if (V.DataSize&0x800000)
		    {
			    v2 = (wchar_t*)VV;
			    len2 = VV.DataSize;
		    } else
		    {
			    d=*(xData*)&V;
			    v2 = d.operator wchar_t*();
			    len2 = d.DataSize&0x7fffff;
		    }
	    }
	    if (len1!=len2) return false;
	    if (memcmp(v1,v2,len1)!=0) return false;
        return true;
    }
    if (((xData*)this)->IsUnsignedInt())
    {
        unsigned __int64 V1 = operator unsigned __int64();
        unsigned __int64 V2;
        if (((xData*)&V)->IsReal())
        {
            double V3 = *((xData*)&V);
            if (V3!= double(__int64(V3)))
                return false;
            V2=(unsigned __int64)V3;
        }
        if (((xData*)&V)->IsSignedInt())
        {
            __int64 V3 = *((xData*)&V);
            if (V3<0) return false;
            V2=V3;
        } else
            V2 = *((xData*)&V);
        if (V1!=V2) return false;
        return true;
    }
    if (((xData*)this)->IsSignedInt())
    {
        __int64 V1 = operator __int64();
        __int64 V2;
        if (((xData*)&V)->IsReal())
        {
            double V3 = *((xData*)&V);
            if (V3!= double(__int64(V3)))
                return false;
            V2=(__int64)V3;
        }
        if (V1<0 && ((xData*)&V)->IsUnsignedInt())
            return false;
        V2 = *((xData*)&V);
        if (V1!=V2) return false;
        return true;
    }
    if (((xData*)this)->IsReal())
    {
        if ( operator double() == double(*(xData*)&V) )
            return true;
    }
    return false;
}

void xDataBaseDll::Allocate(unsigned __int32 count, const xData& value)
{
    Invalidate();
    if (count==0) return;
    xDataBaseDll* t = (xDataBaseDll*)alloc(count*sizeof(xDataCore));
    memset(t,0,count*sizeof(xDataCore));
    for (unsigned __int32 i=0;i<count;i++)
        t[i]=value;
    DataSize=count;
    ((xData*)this)->SetThisDataPointer(t);
}