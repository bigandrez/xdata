inline unsigned __int8 CHECKTYPE(const bool&){return DT_BOOL;};
inline unsigned __int8 CHECKTYPE(const __int8&){return DT_INT8;};
inline unsigned __int8 CHECKTYPE(const __int16&){return DT_INT16;};
inline unsigned __int8 CHECKTYPE(const __int32&){return DT_INT32;};
inline unsigned __int8 CHECKTYPE(const __int64&){return DT_INT64;};
inline unsigned __int8 CHECKTYPE(const unsigned __int8&){return DT_UINT8;};
inline unsigned __int8 CHECKTYPE(const unsigned __int16&){return DT_UINT16;};
inline unsigned __int8 CHECKTYPE(const unsigned __int32&){return DT_UINT32;};
inline unsigned __int8 CHECKTYPE(const unsigned __int64&){return DT_UINT64;};
inline unsigned __int8 CHECKTYPE(const double&){return DT_DOUBLE;};
inline unsigned __int8 CHECKTYPE(const float&){return DT_FLOAT;};
inline unsigned __int8 CHECKTYPE(const char*){return DT_ANSISTRING;};
inline unsigned __int8 CHECKTYPE(const wchar_t*){return DT_UNICODESTRING;};
inline unsigned __int8 CHECKTYPE(const xDataBaseDll&){return DT_UNDEFINE;};

inline const xData TOX(const bool& V){return V;};
inline const xData TOX(const __int8& V){return V;};
inline const xData TOX(const __int16& V){return V;};
inline const xData TOX(const __int32& V){return V;};
inline const xData TOX(const __int64& V){return V;};
inline const xData TOX(const unsigned __int8& V){return V;};
inline const xData TOX(const unsigned __int16& V){return V;};
inline const xData TOX(const unsigned __int32& V){return V;};
inline const xData TOX(const unsigned __int64& V){return V;};
inline const xData TOX(const double& V) {return V;};
inline const xData TOX(const float& V){return V;};
inline const xData TOX(const char*& V){return V;};
inline const xData TOX(const wchar_t*& V){return V;};
inline const xData& TOX(const xDataBaseDll& V){return *(xData*)&V;};

inline void xData::EnterCS() const
{
	if (!CS) return;
	::EnterCriticalSection((LPCRITICAL_SECTION)&((xValueCs*)(DataValue+unsigned __int32(this)))->cs);
}

inline void xData::LeaveCS() const
{
	if (!CS) return;
	::LeaveCriticalSection((LPCRITICAL_SECTION)&((xValueCs*)(DataValue+unsigned __int32(this)))->cs);
}

inline bool xData::IsString() const
{
	if (DataType==DT_STRING)
		return true;
	//if (DataType==DT_CONTROLDATA && AsControlData()->ExtDataType==DT_STRING)
	//	return true;
	return false;
}

inline bool xData::IsAnsiString() const
{
    if (IsReference())
        return GetReference()->IsAnsiString();
	if (DataType==DT_STRING && !(DataSize&0x800000))
		return true;
	//if (DataType==DT_CONTROLDATA && AsControlData()->ExtDataType==DT_STRING && !(DataSize&0x800000))
	//	return true;
	return false;
}

inline bool xData::ThisIsAnsiString() const
{
	if (DataType==DT_STRING && !(DataSize&0x800000))
		return true;
	//if (DataType==DT_CONTROLDATA && AsControlData()->ExtDataType==DT_STRING && !(DataSize&0x800000))
	//	return true;
	return false;
}

inline bool xData::IsUnicodeString() const
{
	if (DataType==DT_STRING && (DataSize&0x800000))
		return true;
	//if (DataType==DT_CONTROLDATA && AsControlData()->ExtDataType==DT_STRING && (DataSize&0x800000))
	//	return true;
	return false;
}

inline bool xData::IsNumber() const 
{
	return IsNumber(DataType);
}

inline bool xData::IsNumber(unsigned __int32 type) const
{
	//if (type==DT_INT8 || type==DT_INT16 || type==DT_INT32 || type==DT_INT64 ||
	//	type==DT_UINT8 || type==DT_UINT16 || type==DT_UINT32 || type==DT_UINT64 ||
	//	type==DT_FLOAT || type==DT_DOUBLE) 
	if (type==0 || type>DT_DOUBLE) 
		return false; 
	return true;
}

inline bool xData::IsEmpty() const 
{
	if (DataSize!=0) return false;
	return true;
}

inline unsigned __int32 xData::GetTypeSize() const
{
	switch (DataType)
	{
	case DT_UNDEFINE: return sizeof(xData);
	case DT_BOOL:     return sizeof(bool);
	case DT_INT8:     return sizeof(__int8);
	case DT_INT16:    return sizeof(__int16);
	case DT_INT32:    return sizeof(__int32);
	case DT_INT64:    return sizeof(__int64);
	case DT_UINT8:    return sizeof(unsigned __int8);
	case DT_UINT16:   return sizeof(unsigned __int16);
	case DT_UINT32:   return sizeof(unsigned __int32);
	case DT_UINT64:   return sizeof(unsigned __int64);
	case DT_FLOAT:    return sizeof(float);
	case DT_DOUBLE:   return sizeof(double);
	case DT_IDREFLECTOR: return sizeof(/*xReflectorData*/32*4);
	case DT_STRING: 
        if (ThisIsAnsiString())
            return 1;
        return 2;
	}
	return 0;
}

inline unsigned __int32 xData::GetTypeSize(unsigned __int8 TypeOfData) const
{
	switch (TypeOfData)
	{
	case DT_UNDEFINE: return sizeof(xData);
	case DT_BOOL:     return sizeof(bool);
	case DT_INT8:     return sizeof(__int8);
	case DT_INT16:    return sizeof(__int16);
	case DT_INT32:    return sizeof(__int32);
	case DT_INT64:    return sizeof(__int64);
	case DT_UINT8:    return sizeof(unsigned __int8);
	case DT_UINT16:   return sizeof(unsigned __int16);
	case DT_UINT32:   return sizeof(unsigned __int32);
	case DT_UINT64:   return sizeof(unsigned __int64);
	case DT_FLOAT:    return sizeof(float);
	case DT_DOUBLE:   return sizeof(double);
	case DT_IDREFLECTOR: return sizeof(/*xReflectorData*/32*4);
//	case DT_CONTROLDATA: return sizeof(HRESULT);
	}
	return 0;
}

inline unsigned __int8 xData::GetThisDataType() const
{
	if (DataType!=DT_CONTROLDATA)
		return DataType;
	return AsControlData()->ExtDataType;
}

inline bool xData::IsAlone() const
{
	if (Named || CS || DataType == DT_DOUBLE || DataType == DT_UINT64 || 
		DataType==DT_INT64 || DataType == DT_IDREFLECTOR || DataType == DT_CONTROLDATA)
		return false;
	if (DataType==DT_STRING && (DataSize&0x7fffff)>1)
		return false;
	if (DataSize>1)
		return false;
	if (Pointer && DataValue!=0)
		return false;
	return true;
}

inline bool xData::IsSignedInt() const
{
	if (DataType == DT_INT8 || DataType == DT_INT16 || DataType == DT_INT32 || DataType == DT_INT64) 
		return true; 
	if (DataType == DT_CONTROLDATA)
	{
		__int8 dt= AsControlData()->ExtDataType;
		if (dt == DT_INT8 || dt == DT_INT16 || dt == DT_INT32 || dt == DT_INT64) 
			return true; 
	}
	return false;
}

inline bool xData::IsUnsignedInt() const
{
	if (DataType == DT_UINT8 || DataType == DT_UINT16 || DataType == DT_UINT32 || DataType == DT_UINT64) 
		return true; 
	if (DataType == DT_CONTROLDATA)
	{
		__int8 dt= AsControlData()->ExtDataType;
		if (dt == DT_UINT8 || dt == DT_UINT16 || dt == DT_UINT32 || dt == DT_UINT64) 
			return true; 
	}
	return false;
}

inline bool xData::IsReal() const
{
	if (DataType == DT_FLOAT || DataType == DT_DOUBLE) 
		return true; 
	if (DataType == DT_CONTROLDATA)
	{
		__int8 dt= AsControlData()->ExtDataType;
		if (dt == DT_FLOAT || dt == DT_DOUBLE) 
			return true; 
	}
	return false;
}

inline void* xData::GetThisDataPointer() const
{
void* ret;

	if (CS || Named)
		ret = &AsValue()->ExtDataValue;
	else
		ret = (void*)&DataValue;

	if (Pointer && !*(unsigned __int32*)ret)
		return NULL;

	if ( DataSize>1 || Pointer || (DataType == DT_UNDEFINE && DataSize>0) || DataType == DT_DOUBLE || DataType == DT_UINT64 || 
        DataType==DT_INT64 || DataType==DT_IDREFLECTOR ||(DataType==DT_STRING && (DataSize&0x7FFFFF)>0))
		ret = (void*) ((*(unsigned __int32*)ret) + unsigned __int32(this));

	return ret;
}

inline void xData::SetThisDataPointer(void* V)
{
unsigned __int32* ret;
	if (CS || Named)
		ret = &AsValue()->ExtDataValue;
	else
		ret = &DataValue;
	*ret = unsigned __int32(V) - unsigned __int32(this);
}

inline unsigned __int32 xData::GetContainerSize() const
{
    if (DataType==DT_STRING)
        return DataSize & 0x7fffff;
    return DataSize;
}

inline unsigned __int32 xData::GetReferenceContainerSize() const
{
    if (!IsReference())
        return 0;
    return GetReference()->GetContainerSize();
}

inline xData* xData::GetReference() const
{
	if (!IsReference())
		return NULL;
	if (Named || CS)
	{
		if (((xValue*)(DataValue+unsigned __int32(this)))->ExtDataValue)
			return (xData*) (((xValue*)(DataValue+unsigned __int32(this)))->ExtDataValue +unsigned __int32(this));
	} else
		if (DataValue)
			return (xData*) (DataValue+unsigned __int32(this));
	return NULL;
}

inline void xData::MoveData(xData& V)
{
    *(__int64*)this = *(__int64*)&V;
    if (CS || Named)
        DataValue = CREATELINK(V.DataValue+unsigned __int32(&V));
        
    if (DataSize && (DataSize>1 || Pointer || DataType==DT_UNDEFINE|| DataType==DT_INT64 || DataType==DT_UINT64 || DataType==DT_DOUBLE || DataType==DT_STRING || DataType==DT_IDREFLECTOR || DataType==DT_CONTROLDATA))
        ((CS || Named) ? EXTDATAVALUE : DataValue) = CREATELINK(V.GetThisDataPointer());
    *(__int64*)&V = 0;
}

//inline XADATA xData::operator()(const xData& arg)
//{
//    return *this;
//}

