#include <string>
#ifndef XDATA_CLASS
#define XDATA_CLASS

#ifdef XDATA_EXPORTS
#define XDATA_API __declspec(dllexport)
#else
#define XDATA_API __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Поддержка версий
XDATA_API const wchar_t* GetXVersion();

class __declspec(novtable) xData;
template <class CONSTRUCTOR> class __declspec(novtable) xDataTemplate;
template <__int32 I, class T> class __declspec(novtable) x_InitWithoutCs;
template <__int32 I, class T> class __declspec(novtable) x_InitWithCs;
class __declspec(novtable) x_CopyDataWithoutPointers;
class __declspec(novtable) x_MoveData;
class __declspec(novtable) xDataReflectorTemplate;
class __declspec(novtable) xDataXmlTemplate;
struct xReflectorData;
class xReflector;
template <class CONSTRUCTOR> class __declspec(novtable) xDataTemplate;
template <__int32 I, class T> class __declspec(novtable) x_InitWithoutCs;
class __declspec(novtable) x_CopyData;
template <class T> class __declspec(novtable) x_Pointer;
template <class T> class __declspec(novtable) x_PointerCs;

XDATA_API bool ParseXmlFile(xData FileName, xData& RetData, xData& RetError);
XDATA_API bool CompileHtmlFile(xData InFileName, xData OutFileName, xData& Xml, xData& RetError);
XDATA_API xData CompileHtmlFile(xData InFileName, xData& Xml, xData& RetError);
XDATA_API void start();
XDATA_API void end();

typedef xDataTemplate<x_Pointer<x_InitWithoutCs<0x10,x_CopyData>>>		xpData;
typedef xDataTemplate<x_InitWithoutCs<0x20,x_CopyDataWithoutPointers>>	xeData;
typedef xDataTemplate<x_Pointer<x_InitWithoutCs<0x30,x_CopyData>>>		xpeData;
typedef xDataTemplate<x_InitWithCs   <0x00,x_CopyDataWithoutPointers>>	xcData;
typedef xDataTemplate<x_PointerCs<x_InitWithCs   <0x10,x_CopyData>>>	xcpData;
typedef xDataTemplate<x_InitWithCs   <0x20,x_CopyDataWithoutPointers>>	xceData;
typedef xDataTemplate<x_PointerCs<x_InitWithCs   <0x30,x_CopyData>>>	xcpeData;
typedef xDataTemplate<x_InitWithoutCs<0x00,x_MoveData>>					xaData;
typedef xDataReflectorTemplate                        					xrData;
typedef xDataXmlTemplate                        				        xmlData;

#include "windows.h"
#include "Defines.h"
#include "xDataCore.h"
#include "xDataSupport.h"
#include "stdio.h"
#include "xDataException.h"
#include "xDataBaseDll.h"
#include <vector>

#pragma warning( disable : 4127 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4003 )


class REF;

class __declspec(novtable) xData : public xDataBaseDll
{
	inline unsigned __int32& InitValue(char* Name=NULL){
		if (Name)
		{
			unsigned __int32 l = ansitextlength(Name);
			xValue* v = (xValue*)xDataBaseDll::alloc(sizeof(xValue) + (l?l+1:0));
			textcopy<char>(v->Name,Name);
			((xData*)this)->DataValue = unsigned __int32(v) - unsigned __int32(this);
			((xData*)this)->Named=1;
			return v->ExtDataValue;
		}
		return ((xData*)this)->DataValue;
	};
	template <class TYPE>
	inline void ArrayInit(unsigned __int32& DV, const TYPE* Data, unsigned __int32 Count, bool AsString=false){
		((xData*)this)->DataSize=Count;
		TYPE* t;
		if (Count>1 || sizeof(TYPE)>4 || AsString)
		{
			t = (TYPE*)xDataBaseDll::alloc((((xData*)this)->DataSize)*sizeof(TYPE));
			DV=unsigned __int32(t)-unsigned __int32(this);
		} else
			t=(TYPE*)&DV;
		memcpy(t,Data,Count*sizeof(TYPE));
	};
public:
	// Вход в критическую секцию
	inline void EnterCS() const;

	// Выход из критической секции
	inline void LeaveCS() const;

	// Если объект является строкой
	inline bool IsString() const;

    // Если объект или вложенный объект является ANSI строкой
	inline bool IsAnsiString() const;

    // Если объект является ANSI строкой
	inline bool ThisIsAnsiString() const;

	// Если объект является UNICODE строкой
	inline bool IsUnicodeString() const;

	// Если объект является числом
	inline bool IsNumber() const;

	// Если указанный тип объект является числом
	inline bool IsNumber(unsigned __int32 type) const;

	// Возвращает истину, если объект "пустой"
	inline bool IsEmpty() const;

	// Возвращает истину, если объект является указателем
	inline bool IsPointer() const {return Pointer==1;};

	// Возвращает количество байт, занимаемых одним элементом указанного типа
	inline unsigned __int32 GetTypeSize(unsigned __int8 TypeOfData) const;

	// Возвращает количество байт, занимаемых одним элементом 
	inline unsigned __int32 GetTypeSize() const;

    // Возвращает количество данных
    inline unsigned __int32 GetDataSize() const {
        if (DataType==DT_STRING) 
            return (DataSize&0x7FFFFF)-1; 
        return DataSize;};

    inline xValue* AsValue() const {return (xValue*)(DataValue+unsigned __int32(this));};
    inline xValueCs* AsValueCs() const {return (xValueCs*)(DataValue+unsigned __int32(this));};
    inline xReflectorData* AsReflectorData() const {return (xReflectorData*)(DataValue+unsigned __int32(this));};
    inline xControlData* AsControlData() const {return (xControlData*)(DataValue+unsigned __int32(this));};

	// Возвращает тип данных объекта (используется также DT_ANSISTRING и DT_UNICODESTRING)
	inline unsigned __int8 GetThisDataType() const;

	// Истина, если объект занимает в памяти только 8 байт (не содержит внешние данные)
	inline bool IsAlone() const;

	// Если число знаковое целое
	inline bool IsSignedInt() const;

	// Если число беззнаковое целое
	inline bool IsUnsignedInt() const;

	// Если число вещественное
	inline bool IsReal() const;

	// Указатель на данные текущего объекта
	inline void* GetThisDataPointer() const;

    // Возвращает количество объектов в последнем вложенном объекте
    inline unsigned __int32 GetContainerSize() const;

    // Возвращает DataSize вложенного объекта
    inline unsigned __int32 GetReferenceContainerSize() const;

	// Установить DataValue (или ExtDataValue при CS или Named) в значение относительного указателя
	inline void SetThisDataPointer(void* V);

    inline bool IsReference() const {return Pointer && DataSize==1 && DataType==DT_UNDEFINE;};
//    inline bool IsReference() const {return Pointer && DataSize==0 && DataType==DT_UNDEFINE && 
//        ((CS || Named)? AsValue()->ExtDataValue!=0 : DataValue!=0);};

    // Если объект является ссылкой на другой объект, то возвращается указатель на этот объект. Иначе NULL
	inline xData* GetReference() const;

	// Перемещение данных
	inline void MoveData(xData& V);

    

    template <typename T> xData& Push(const T& V){CHECKTYPE(V);xDataBaseDll::Push(&V,1,CHECKTYPE(V));return *this;};
    template <typename T> xData& Push(const T*V, unsigned __int32 Count){CHECKTYPE(*V);xDataBaseDll::Push(V,Count,CHECKTYPE(*V));return *this;};

    template <typename T> inline xData& operator+=(const T &V) {return *(xData*)&xDataBaseDll::operator +=(TOX(V));};
    template <typename T> inline xData& operator+=(const T* const V) {return *(xData*)&xDataBaseDll::operator +=(TOX((const T*)V));};

    template <typename T> inline xData& operator<<(const T &V) {CHECKTYPE(V);Push(V); return *this;};

    inline const xaData operator[](const REF &V) const;
    template <typename T> inline xaData operator[](const T &V) const {return  xDataBaseDll::operator [](TOX(V));};
    template <typename T> inline xaData operator[](const T* const V) const {return  xDataBaseDll::operator [](TOX((const T*)V));};

    template <typename T> inline xaData operator()(const T &V);
    template <typename T> inline xaData operator()(const T* const V) {return operator()(TOX((const T*)V));};

    template <typename T> bool operator==(const T &V) const {return xDataBaseDll::operator ==(TOX(V));};
    template <typename T> bool operator==(const T* const V) const {return xDataBaseDll::operator ==(TOX((const T*)V));};

    template <typename T> bool operator!=(const T &V) const {return !(xDataBaseDll::operator ==(TOX(V)));};
    template <typename T> bool operator!=(const T* const V) const {return !(xDataBaseDll::operator ==(TOX((const T*)V)));};

// ==================================================================================================================================================
// Конструкторы
    __forceinline xData(int,int,int){};
    __forceinline xData(int,int,int, char* V){*(__int64*)this=0;FromXml(V);};
	__forceinline xData(const xData& V){*(__int64*)this=0;CopyData(V,true);};
    __forceinline xData() {*(__int32*)this=0;*(1+(__int32*)this)=0;};

	__forceinline xData(const             bool &V) {*((__int32*)(1+(__int32*)this))=__int32(V);*(__int32*)this=  DT_BOOL|0x100;};
	__forceinline xData(const           __int8 &V) {*((__int32*)(1+(__int32*)this))=__int32(V);*(__int32*)this=  DT_INT8|0x100;};
	xData(const unsigned  __int8 &V) 
    {*((unsigned __int32*)(1+(__int32*)this))=unsigned __int32(V);
    *(__int32*)this= DT_UINT8|0x100;
    };
	__forceinline xData(const          __int16 &V) {*((__int32*)(1+(__int32*)this))=__int32(V);*(__int32*)this= DT_INT16|0x100;};
	__forceinline xData(const unsigned __int16 &V) {*((unsigned __int32*)(1+(__int32*)this))=unsigned __int32(V);*(__int32*)this=DT_UINT16|0x100;};
	__forceinline xData(const          __int32 &V) {*((__int32*)(1+(__int32*)this))=V;*(__int32*)this= DT_INT32|0x100;};
	__forceinline xData(const unsigned __int32 &V) {*((unsigned __int32*)(1+(__int32*)this))=V;*(__int32*)this=DT_UINT32|0x100;};
	__forceinline xData(const          __int64 &V) {ArrayInit<__int64>(DataValue,&V,1);*(__int32*)this= DT_INT64|0x100;};
	__forceinline xData(const unsigned __int64 &V) {ArrayInit<unsigned __int64>(DataValue,&V,1);*(__int32*)this= DT_UINT64|0x100;};
	__forceinline xData(const            float &V) {*((float*)(1+(__int32*)this))=V;*(__int32*)this= DT_FLOAT|0x100;};
	__forceinline xData(const           double &V) {ArrayInit<double>(DataValue,&V,1);*(__int32*)this= DT_DOUBLE|0x100;};
	__forceinline xData(const             char* V) {*(__int32*)this=DT_STRING;const char*v=V?V:"";ArrayInit<char>(DataValue,v,ansitextlength(v)+1,true);};
	__forceinline xData(const          wchar_t* V) {*(__int32*)this=DT_STRING;const wchar_t*v=V?V:L"";ArrayInit<wchar_t>(DataValue,v,unicodetextlength(v)+1,true);DataSize|=0x800000;};

	__forceinline xData(             char* const V) {*(__int32*)this=DT_STRING;const char*v=V?V:"";ArrayInit<char>(DataValue,v,ansitextlength(v)+1,true);};
	__forceinline xData(          wchar_t* const V) {*(__int32*)this=DT_STRING;const wchar_t*v=V?V:L"";ArrayInit<wchar_t>(DataValue,v,unicodetextlength(v)+1,true);DataSize|=0x800000;};

	__forceinline xData(const            xData &V, char* Name) {*(__int32*)this= 0;operator=(V);SetName(Name);};
	__forceinline xData(const             bool &V, char* Name) {*(__int32*)this=  DT_BOOL|0x100;*((__int32*)&InitValue(Name))=__int32(V);};
	__forceinline xData(const           __int8 &V, char* Name) {*(__int32*)this=  DT_INT8|0x100;*((__int32*)&InitValue(Name))=__int32(V);};
	__forceinline xData(const unsigned  __int8 &V, char* Name) {*(__int32*)this= DT_UINT8|0x100;*((unsigned __int32*)&InitValue(Name))=unsigned __int32(V);};
	__forceinline xData(const          __int16 &V, char* Name) {*(__int32*)this= DT_INT16|0x100;*((__int32*)&InitValue(Name))=__int32(V);};
	__forceinline xData(const unsigned __int16 &V, char* Name) {*(__int32*)this=DT_UINT16|0x100;*((unsigned __int32*)&InitValue(Name))=unsigned __int32(V);};
	__forceinline xData(const          __int32 &V, char* Name) {*(__int32*)this= DT_INT32|0x100;*((__int32*)&InitValue(Name))=V;};
	__forceinline xData(const unsigned __int32 &V, char* Name) {*(__int32*)this=DT_UINT32|0x100;InitValue(Name)=V;};
	__forceinline xData(const          __int64 &V, char* Name) {*(__int32*)this= DT_INT64|0x100;ArrayInit<__int64>(InitValue(Name),&V,1);};
	__forceinline xData(const unsigned __int64 &V, char* Name) {*(__int32*)this= DT_UINT64|0x100;ArrayInit<unsigned __int64>(InitValue(Name),&V,1);};
	__forceinline xData(const            float &V, char* Name) {*(__int32*)this= DT_FLOAT|0x100;*((float*)&InitValue(Name))=V;};
	__forceinline xData(const           double &V, char* Name) {*(__int32*)this= DT_DOUBLE|0x100;ArrayInit<double>(InitValue(Name),&V,1);};
    __forceinline xData(const             char* V, char* Name) {*(__int32*)this= DT_STRING; const char* v= V ? V : ""; ArrayInit<char>(InitValue(Name),v,ansitextlength(v)+1,true);};
	__forceinline xData(const          wchar_t* V, char* Name) {*(__int32*)this= DT_STRING; const wchar_t* v= V ? V : L""; ArrayInit<wchar_t>(InitValue(Name),v,unicodetextlength(v)+1,true);DataSize|=0x800000;};

 	template <typename T> __forceinline xData(const T* V, unsigned __int32 len) {*(unsigned __int32*)this= CHECKTYPE(*V);ArrayInit<T>(DATAVALUE,V,len);};
	template <typename T> __forceinline xData(const T* V, unsigned __int32 len, char* Name) {*(unsigned __int32*)this= CHECKTYPE(*V);ArrayInit<T>(InitValue(Name),V,len);};

};


template <typename T> inline xaData xData::operator()(const T &V)
{
//    if (Pointer && DataType==DT_UNDEFINE)
//        return ((xData*)GetThisDataPointer())->operator ()(TOX(V));
    if (DataType==DT_IDREFLECTOR)
    {
        if (DataSize==1)
            return AsReflectorData()->reflector(TOX(V));
        else
        {
            const xData& arg = TOX(V);
            xReflectorData* p = (xReflectorData*)GetThisDataPointer();
            for (unsigned __int32 i = 0; i<DataSize; i++)
                (p+i)->reflector(arg);
        }
    }
    if (DataType==DT_UNDEFINE && DataSize)
    {
        const xData& arg = TOX(V);
        xData* p = (xData*)GetThisDataPointer();
        for (unsigned __int32 i = 0; i<DataSize; i++)
            (p+i)->operator ()(arg);
    }

    return xData();
}

#include "xData - core functions.h"

// Используется для инициализации с критической секцией
template <__int32 I, class T>
class __declspec(novtable) x_InitWithCs: public T {
	inline unsigned __int32& InitValue(char* Name=NULL){
		unsigned __int32 l = ansitextlength(Name);
		xValueCs* v = (xValueCs*)xDataBaseDll::alloc(sizeof(xValueCs) + (l?l+1:0));
		memset(v,0,sizeof(xValueCs) + (l?l+1:0));
		textcopy<char>(v->Name,Name);
		((xData*)this)->DataValue = unsigned __int32(v) - unsigned __int32(this);
		InitializeCriticalSection((LPCRITICAL_SECTION)&((xValueCs*)v)->cs);
		if (Name) ((xData*)this)->Named=1;
		return v->ExtDataValue;
	};
	template <class TYPE>
	inline void ArrayInit(unsigned __int32& DV, const TYPE* Data, unsigned __int32 Count, bool AsString=false){
		((xData*)this)->DataSize=Count;
		TYPE* t;
		if (Count>1 || sizeof(TYPE)>4 || AsString)
		{
			t = (TYPE*)xDataBaseDll::alloc((((xData*)this)->DataSize)*sizeof(TYPE));
			DV=unsigned __int32(t)-unsigned __int32(this);
		} else
			t=(TYPE*)&DV;
		memcpy(t,Data,Count*sizeof(TYPE));
	};
public:
	__forceinline x_InitWithCs(xData& V){CopyData(V);};
	__forceinline x_InitWithCs(){InitValue();*(__int32*)this= I|DT_UNDEFINE|0x80;};
	__forceinline x_InitWithCs(const int a, const int) {*(__int32*)this=I|a|0x80;InitValue();};
	__forceinline x_InitWithCs(const int a, const int, char* Name) {*(__int32*)this=I|a|0x80;InitValue(Name);};

	__forceinline x_InitWithCs(const             bool &V){ *(__int32*)this=  (I&0xe0)|DT_BOOL|0x80|0x100;*(__int32*)&InitValue() =__int32(V);};
	__forceinline x_InitWithCs(const           __int8 &V){ *(__int32*)this=  (I&0xe0)|DT_INT8|0x80|0x100;*(__int32*)&InitValue() =__int32(V);};
	__forceinline x_InitWithCs(const unsigned  __int8 &V){ *(__int32*)this= (I&0xe0)|DT_UINT8|0x80|0x100;InitValue()=unsigned __int32(V);};
	__forceinline x_InitWithCs(const          __int16 &V){ *(__int32*)this= (I&0xe0)|DT_INT16|0x80|0x100;*(__int32*)&InitValue() =__int32(V);};
	__forceinline x_InitWithCs(const unsigned __int16 &V){ *(__int32*)this=(I&0xe0)|DT_UINT16|0x80|0x100;InitValue()=unsigned __int32(V);};
	__forceinline x_InitWithCs(const          __int32 &V){ *(__int32*)this= (I&0xe0)|DT_INT32|0x80|0x100;*(__int32*)&InitValue() =__int32(V);};
	__forceinline x_InitWithCs(const unsigned __int32 &V){ *(__int32*)this=(I&0xe0)|DT_UINT32|0x80|0x100;InitValue()=V;};
	__forceinline x_InitWithCs(const          __int64 &V){ ArrayInit<__int64>(InitValue(),&V,1);*(__int32*)this= (I&0xe0)|DT_INT64|0x80|0x100;};
	__forceinline x_InitWithCs(const unsigned __int64 &V){ ArrayInit<unsigned __int64>(InitValue(),&V,1);*(__int32*)this= (I&0xe0)|DT_UINT64|0x80|0x100;};
	__forceinline x_InitWithCs(const            float &V){ *(__int32*)this= (I&0xe0)|DT_FLOAT|0x80|0x100;*(float*)&InitValue() =V;};
	__forceinline x_InitWithCs(const           double &V){ ArrayInit<double>(InitValue(),&V,1);*(__int32*)this= (I&0xe0)|DT_DOUBLE|0x80|0x100;};
	__forceinline x_InitWithCs(const             char* V){ *(__int32*)this=(I&0xe0)|DT_STRING|0x80;ArrayInit<char>(InitValue(),V,ansitextlength(V)+1,true);};
	__forceinline x_InitWithCs(const          wchar_t* V){ *(__int32*)this=(I&0xe0)|DT_STRING|0x80;ArrayInit<wchar_t>(InitValue(),V,unicodetextlength(V)+1,true);((xData*)this)->DataSize|=0x800000;};

	__forceinline x_InitWithCs(const             bool &V, char* Name){ *(__int32*)this=  (I&0xe0)|DT_BOOL|0x80|0x100;*(__int32*)&InitValue(Name) =__int32(V);};
	__forceinline x_InitWithCs(const           __int8 &V, char* Name){ *(__int32*)this=  (I&0xe0)|DT_INT8|0x80|0x100;*(__int32*)&InitValue(Name) =__int32(V);};
	__forceinline x_InitWithCs(const unsigned  __int8 &V, char* Name){ *(__int32*)this= (I&0xe0)|DT_UINT8|0x80|0x100;InitValue(Name)=unsigned __int32(V);};
	__forceinline x_InitWithCs(const          __int16 &V, char* Name){ *(__int32*)this= (I&0xe0)|DT_INT16|0x80|0x100;*(__int32*)&InitValue(Name) =__int32(V);};
	__forceinline x_InitWithCs(const unsigned __int16 &V, char* Name){ *(__int32*)this=(I&0xe0)|DT_UINT16|0x80|0x100;InitValue(Name)=unsigned __int32(V);};
	__forceinline x_InitWithCs(const          __int32 &V, char* Name){ *(__int32*)this= (I&0xe0)|DT_INT32|0x80|0x100;*(__int32*)&InitValue(Name) =__int32(V);};
	__forceinline x_InitWithCs(const unsigned __int32 &V, char* Name){ *(__int32*)this=(I&0xe0)|DT_UINT32|0x80|0x100;InitValue(Name)=V;};
	__forceinline x_InitWithCs(const          __int64 &V, char* Name){ *(__int32*)this= (I&0xe0)|DT_INT64|0x80|0x100;ArrayInit<__int64>(InitValue(Name),&V,1);};
	__forceinline x_InitWithCs(const unsigned __int64 &V, char* Name){ *(__int32*)this= (I&0xe0)|DT_UINT64|0x80|0x100;ArrayInit<unsigned __int64>(InitValue(Name),&V,1);};
	__forceinline x_InitWithCs(const            float &V, char* Name){ *(__int32*)this= (I&0xe0)|DT_FLOAT|0x80|0x100;*(float*)&InitValue(Name) =V;};
	__forceinline x_InitWithCs(const           double &V, char* Name){ *(__int32*)this= (I&0xe0)|DT_DOUBLE|0x80|0x100;ArrayInit<double>(InitValue(Name),&V,1);};
	__forceinline x_InitWithCs(const             char* V, char* Name){ *(__int32*)this=(I&0xe0)|DT_STRING|0x80;ArrayInit<char>(InitValue(Name),V,ansitextlength(V)+1,true);};
	__forceinline x_InitWithCs(const          wchar_t* V, char* Name){ *(__int32*)this=(I&0xe0)|DT_STRING|0x80;ArrayInit<wchar_t>(InitValue(Name),V,unicodetextlength(V)+1,true);((xData*)this)->DataSize|=0x800000;};

	template <typename T> __forceinline x_InitWithCs(const T* V, unsigned __int32 len, char* Name=NULL) {*(unsigned __int32*)this=(I&0xe0)|0x80|CHECKTYPE(*V);ArrayInit<T>(InitValue(Name),V,len);};
};

// Используется для инициализации без критической секции
template <__int32 I, class T>
class __declspec(novtable) x_InitWithoutCs: public T {

	inline unsigned __int32& InitValue(char* Name=NULL){
		if (Name)
		{
			unsigned __int32 l = ansitextlength(Name);
			xValue* v = (xValue*)xDataBaseDll::alloc(sizeof(xValue) + (l?l+1:0));
			textcopy<char>(v->Name,Name);
			((xData*)this)->DataValue = unsigned __int32(v) - unsigned __int32(this);
			((xData*)this)->Named=1;
			return v->ExtDataValue;
		}
		return ((xData*)this)->DataValue;
	};
	template <class TYPE>
	inline void ArrayInit(unsigned __int32& DV, const TYPE* Data, unsigned __int32 Count, bool AsString=false){
		((xData*)this)->DataSize=Count;
		TYPE* t;
		if (Count>1 || sizeof(TYPE)>4 || AsString)
		{
			t = (TYPE*)xDataBaseDll::alloc((((xData*)this)->DataSize)*sizeof(TYPE));
			DV=unsigned __int32(t)-unsigned __int32(this);
		} else
			t=(TYPE*)&DV;
		memcpy(t,Data,Count*sizeof(TYPE));
	};
public:
	__forceinline x_InitWithoutCs(xData& V):T(V,false){};
	__forceinline x_InitWithoutCs() : T() {*(__int32*)this=I|DT_UNDEFINE;*(1+(__int32*)this)=0;};
	__forceinline x_InitWithoutCs(const int a, const int) : T() {*(__int32*)this=I|a|0x100;};
	__forceinline x_InitWithoutCs(const int a, const int, char* Name) : T() {*(__int32*)this=I|a|0x100;InitValue(Name);};

	__forceinline x_InitWithoutCs(const             bool &V) {*((__int32*)(1+(__int32*)this))=__int32(V);*(__int32*)this=  (I&0xe0)|DT_BOOL|0x100;};
	__forceinline x_InitWithoutCs(const           __int8 &V) {*((__int32*)(1+(__int32*)this))=__int32(V);*(__int32*)this=  (I&0xe0)|DT_INT8|0x100;};
	__forceinline x_InitWithoutCs(const unsigned  __int8 &V) {*((unsigned __int32*)(1+(__int32*)this))=unsigned __int32(V);*(__int32*)this= (I&0xe0)|DT_UINT8|0x100;};
	__forceinline x_InitWithoutCs(const          __int16 &V) {*((__int32*)(1+(__int32*)this))=__int32(V);*(__int32*)this= (I&0xe0)|DT_INT16|0x100;};
	__forceinline x_InitWithoutCs(const unsigned __int16 &V) {*((unsigned __int32*)(1+(__int32*)this))=unsigned __int32(V);*(__int32*)this=(I&0xe0)|DT_UINT16|0x100;};
	__forceinline x_InitWithoutCs(const          __int32 &V) {*((__int32*)(1+(__int32*)this))=V;*(__int32*)this= (I&0xe0)|DT_INT32|0x100;};
	__forceinline x_InitWithoutCs(const unsigned __int32 &V) {*((unsigned __int32*)(1+(__int32*)this))=V;*(__int32*)this=(I&0xe0)|DT_UINT32|0x100;};
	__forceinline x_InitWithoutCs(const          __int64 &V) {ArrayInit<__int64>(((xData*)this)->DataValue,&V,1);*(__int32*)this= (I&0xe0)|DT_INT64|0x100;};
	__forceinline x_InitWithoutCs(const unsigned __int64 &V) {ArrayInit<unsigned __int64>(((xData*)this)->DataValue,&V,1);*(__int32*)this= (I&0xe0)|DT_UINT64|0x100;};
	__forceinline x_InitWithoutCs(const            float &V) {*((float*)(1+(__int32*)this))=V;*(__int32*)this= (I&0xe0)|DT_FLOAT|0x100;};
	__forceinline x_InitWithoutCs(const           double &V) {ArrayInit<double>(((xData*)this)->DataValue,&V,1);*(__int32*)this= (I&0xe0)|DT_DOUBLE|0x100;};
	__forceinline x_InitWithoutCs(const             char* V) {*(__int32*)this=(I&0xe0)|DT_STRING;ArrayInit<char>(((xData*)this)->DataValue,V,ansitextlength(V)+1,true);};
	__forceinline x_InitWithoutCs(const          wchar_t* V) {*(__int32*)this=(I&0xe0)|DT_STRING;ArrayInit<wchar_t>(((xData*)this)->DataValue,V,unicodetextlength(V)+1,true);((xData*)this)->DataSize|=0x800000;};

	__forceinline x_InitWithoutCs(const             bool &V, char* Name) {*(__int32*)this=  (I&0xe0)|DT_BOOL|0x100;*((__int32*)&InitValue(Name))=__int32(V);};
	__forceinline x_InitWithoutCs(const           __int8 &V, char* Name) {*(__int32*)this=  (I&0xe0)|DT_INT8|0x100;*((__int32*)&InitValue(Name))=__int32(V);};
	__forceinline x_InitWithoutCs(const unsigned  __int8 &V, char* Name) {*(__int32*)this= (I&0xe0)|DT_UINT8|0x100;*((unsigned __int32*)&InitValue(Name))=unsigned __int32(V);};
	__forceinline x_InitWithoutCs(const          __int16 &V, char* Name) {*(__int32*)this= (I&0xe0)|DT_INT16|0x100;*((__int32*)&InitValue(Name))=__int32(V);};
	__forceinline x_InitWithoutCs(const unsigned __int16 &V, char* Name) {*(__int32*)this=(I&0xe0)|DT_UINT16|0x100;*((unsigned __int32*)&InitValue(Name))=unsigned __int32(V);};
	__forceinline x_InitWithoutCs(const          __int32 &V, char* Name) {*(__int32*)this= (I&0xe0)|DT_INT32|0x100;*((__int32*)&InitValue(Name))=V;};
	__forceinline x_InitWithoutCs(const unsigned __int32 &V, char* Name) {*(__int32*)this=(I&0xe0)|DT_UINT32|0x100;InitValue(Name)=V;};
	__forceinline x_InitWithoutCs(const          __int64 &V, char* Name) {*(__int32*)this= (I&0xe0)|DT_INT64|0x100;ArrayInit<__int64>(InitValue(Name),&V,1);};
	__forceinline x_InitWithoutCs(const unsigned __int64 &V, char* Name) {*(__int32*)this= (I&0xe0)|DT_UINT64|0x100;ArrayInit<unsigned __int64>(InitValue(Name),&V,1);};
	__forceinline x_InitWithoutCs(const            float &V, char* Name) {*(__int32*)this= (I&0xe0)|DT_FLOAT|0x100;*((float*)&InitValue(Name))=V;};
	__forceinline x_InitWithoutCs(const           double &V, char* Name) {*(__int32*)this= (I&0xe0)|DT_DOUBLE|0x100;ArrayInit<double>(InitValue(Name),&V,1);};
	__forceinline x_InitWithoutCs(const             char* V, char* Name) {*(__int32*)this= (I&0xe0)|DT_STRING;ArrayInit<char>(InitValue(Name),V,ansitextlength(V)+1,true);};
	__forceinline x_InitWithoutCs(const          wchar_t* V, char* Name) {*(__int32*)this= (I&0xe0)|DT_STRING;ArrayInit<wchar_t>(InitValue(Name),V,unicodetextlength(V)+1,true);((xData*)this)->DataSize|=0x800000;};

	template <typename T> __forceinline x_InitWithoutCs(const T* V, unsigned __int32 len) {*(unsigned __int32*)this= (I&0xe0)|CHECKTYPE(*V);ArrayInit<T>(DATAVALUE,V,len);};
	template <typename T> __forceinline x_InitWithoutCs(const T* V, unsigned __int32 len, char* Name) {*(unsigned __int32*)this= (I&0xe0)|CHECKTYPE(*V);ArrayInit<T>(InitValue(Name),V,len);};
};

class __declspec(novtable) x_CopyData{
public:
	__forceinline x_CopyData(){};
	__forceinline x_CopyData(xData& V, bool UseCS) {
        if (UseCS)
        {
            *(__int32*)this = 0x80;
		    xValueCs* v = (xValueCs*)xDataBaseDll::alloc(sizeof(xValueCs));
		    memset(v,0,sizeof(xValueCs));
            DATAVALUE=CREATELINK(v);
		    InitializeCriticalSection((LPCRITICAL_SECTION)&((xValueCs*)v)->cs);
        } else *(__int32*)this = 0;
        ((xData*)this)->CopyData(V);
    };
};

class __declspec(novtable) x_CopyDataWithoutPointers{
public:
	__forceinline x_CopyDataWithoutPointers(){};
	__forceinline x_CopyDataWithoutPointers(xData& V, bool UseCS) {
        if (UseCS)
        {
            *(__int32*)this = 0x80;
		    xValueCs* v = (xValueCs*)xDataBaseDll::alloc(sizeof(xValueCs));
		    memset(v,0,sizeof(xValueCs));
            DATAVALUE=CREATELINK(v);
		    InitializeCriticalSection((LPCRITICAL_SECTION)&((xValueCs*)v)->cs);
        } else *(__int32*)this = 0;
        ((xData*)this)->CopyData(V,true);
    };
}; 

class __declspec(novtable) x_MoveData{
public:
	__forceinline x_MoveData(){};
	__forceinline x_MoveData(xData& V, bool) {((xData*)this)->MoveData(V);};
};

template <class T>
class __declspec(novtable) x_Pointer : public T{
public:
    x_Pointer() : T() {};
//    __forceinline x_Pointer(const xData& V): T(V) {};
    __forceinline x_Pointer(xData& V): T(0,0) {((xData*)this)->CopyData(V,false);};
    __forceinline x_Pointer(xData* V): T(0,0) {DATAVALUE = CREATELINK(V);};
	CONST_INIT_CONSTRUCTOR(__forceinline x_Pointer,T)
	CONST_INIT_ARRAY_CONSTRUCTOR(__forceinline x_Pointer,T)
	CONST_NAMED_INIT_CONSTRUCTOR(__forceinline x_Pointer,T)
	CONST_NAMED_INIT_ARRAY_CONSTRUCTOR(__forceinline x_Pointer,T)
	INIT_POINTER_CONSTRUCTOR(__forceinline x_Pointer,T,DATAVALUE = CREATELINK(&V))
	INIT_ARRAY_POINTER_CONSTRUCTOR(__forceinline x_Pointer,T,DATAVALUE = CREATELINK(V);((xData*)this)->DataSize=len)
	INIT_NAMED_POINTER_CONSTRUCTOR(__forceinline x_Pointer,T,(Name==0 ? DATAVALUE : EXTDATAVALUE) = CREATELINK(&V))
	INIT_NAMED_ARRAY_POINTER_CONSTRUCTOR(__forceinline x_Pointer,T,(Name==0 ? DATAVALUE : EXTDATAVALUE) = CREATELINK(V);((xData*)this)->DataSize=len)

	__forceinline x_Pointer(char* V):T(DT_STRING,0) {DATAVALUE = CREATELINK(V);((xData*)this)->DataSize=ansitextlength(V);};
	__forceinline x_Pointer(char* V, char* Name):T(DT_STRING,0,Name) { (Name==NULL ? DATAVALUE : EXTDATAVALUE) = CREATELINK(V);((xData*)this)->DataSize=ansitextlength(V);};
	__forceinline x_Pointer(wchar_t* V):T(DT_STRING,0) {DATAVALUE = CREATELINK(V);((xData*)this)->DataSize=unicodetextlength(V)|0x80000000;};
	__forceinline x_Pointer(wchar_t* V, char* Name):T(DT_STRING,0,Name) {(Name==NULL ? DATAVALUE : EXTDATAVALUE) = CREATELINK(V);((xData*)this)->DataSize=unicodetextlength(V)|0x80000000;};
};

template <class T>
class __declspec(novtable) x_PointerCs : public T{
public:
    x_PointerCs() : T() {};
    __forceinline x_PointerCs(xData& V): T(0,0) {CopyData(V,false);};
    __forceinline x_PointerCs(xData* V): T(0,0) {EXTDATAVALUE = CREATELINK(&V);};
	CONST_INIT_CONSTRUCTOR(__forceinline x_PointerCs,T)
	CONST_INIT_ARRAY_CONSTRUCTOR(__forceinline x_PointerCs,T)
	CONST_NAMED_INIT_CONSTRUCTOR(__forceinline x_PointerCs,T)
	CONST_NAMED_INIT_ARRAY_CONSTRUCTOR(__forceinline x_PointerCs,T)
	INIT_POINTER_CONSTRUCTOR(__forceinline x_PointerCs,T,EXTDATAVALUE = CREATELINK(&V))
	INIT_ARRAY_POINTER_CONSTRUCTOR(__forceinline x_PointerCs,T,EXTDATAVALUE = CREATELINK(V);((xData*)this)->DataSize=len)
	INIT_NAMED_POINTER_CONSTRUCTOR(__forceinline x_PointerCs,T,EXTDATAVALUE = CREATELINK(&V))
	INIT_NAMED_ARRAY_POINTER_CONSTRUCTOR(__forceinline x_PointerCs,T,EXTDATAVALUE = CREATELINK(V);((xData*)this)->DataSize=len)

	__forceinline x_PointerCs(char* V):T(DT_STRING,0) {EXTDATAVALUE = CREATELINK(V);((xData*)this)->DataSize=ansitextlength(V);};
	__forceinline x_PointerCs(char* V, char* Name):T(DT_STRING,0,Name) {EXTDATAVALUE = CREATELINK(V);((xData*)this)->DataSize=ansitextlength(V);};
	__forceinline x_PointerCs(wchar_t* V):T(DT_STRING,0) {EXTDATAVALUE = CREATELINK(V);((xData*)this)->DataSize=unicodetextlength(V)|0x80000000;};
	__forceinline x_PointerCs(wchar_t* V, char* Name):T(DT_STRING,0,Name) {EXTDATAVALUE = CREATELINK(V);((xData*)this)->DataSize=unicodetextlength(V)|0x80000000;};
};

template <class CONSTRUCTOR>
class __declspec(novtable) xDataTemplate : public CONSTRUCTOR, public xData
{
  public:
	__forceinline xDataTemplate(): CONSTRUCTOR(), xData(0,0,0) {}; 
	__forceinline xDataTemplate(const xData& V) :  CONSTRUCTOR(*(xData*)&V), xData(0,0,0) {}; 
	__forceinline xDataTemplate(xData* V) :  CONSTRUCTOR(V), xData(0,0,0) {}; 
	__forceinline xDataTemplate(const xDataTemplate& V): CONSTRUCTOR(*(xData*)&V), xData(0,0,0) {}; 

    xData& operator=(xData &V){xDataBaseDll::operator=(V);return *this;};

    SIMPLE_TYPE_CONSTRUCTOR( )
	SIMPLE_TYPE_CONSTRUCTOR( const )
    SIMPLE_TYPE_NAMED_CONSTRUCTOR( )
    SIMPLE_TYPE_NAMED_CONSTRUCTOR( const )
	SIMPLE_TYPE_ARRAY_CONSTRUCTOR( )
	SIMPLE_TYPE_ARRAY_CONSTRUCTOR( const )
	SIMPLE_TYPE_NAMED_ARRAY_CONSTRUCTOR( )
	SIMPLE_TYPE_NAMED_ARRAY_CONSTRUCTOR( const )
};

class __declspec(novtable) xDataReflectorTemplate : public xData{
    template <typename F> __forceinline void Init(void* t, F f, void* ID, char* Name)
    {
        *(unsigned __int32*)this = DT_IDREFLECTOR;
        unsigned __int32 l = Name ? ansitextlength(Name) : 0;
        xReflectorData* v = (xReflectorData*)xDataBaseDll::alloc(sizeof(xReflectorData) + (l?l+1:0));;
		if (Name)
		{
			textcopy<char>(v->Name,Name);
			((xData*)this)->DataValue = unsigned __int32(v) - unsigned __int32(this);
			((xData*)this)->Named=1;
		}
        v->reflector = xReflector(t,f,ID);
        DATAVALUE = CREATELINK(v);
        DataSize=1;
	};
public:
    template <typename F> __forceinline xDataReflectorTemplate(void* t, F f, void* ID , char* Name) {Init(t,f,ID,Name);};
    template <typename F> __forceinline xDataReflectorTemplate(void* t, F f, const char* Name) {Init(t,f,0,Name);};
    template <typename F> __forceinline xDataReflectorTemplate(void* t, F f, void* ID) {Init(t,f,ID,NULL);};
    template <typename F> __forceinline xDataReflectorTemplate(void* t, F f) {Init(t,f,0,NULL);};
};

class __declspec(novtable) xDataXmlTemplate : public xData{
public:
    __forceinline xDataXmlTemplate(char* XmlText) : xData(0,0,0,XmlText) {};
};


DECLARE_GLOBAL_UNARY_OPERATOR1(+,+=)
DECLARE_GLOBAL_UNARY_OPERATOR2(+,+=)
DECLARE_GLOBAL_COMPARE_OPERATOR(!=)
DECLARE_GLOBAL_COMPARE_OPERATOR(==)

class xReflectorBase{
public:
	xaData func(const xData&, void*){};
};

template <class T>
class xReflectorT{
	typedef xaData (T::*F)(const xData&, void*);
public:
	__forceinline xReflectorT(): m_pT(0), m_f(0), data(0){};
	__forceinline xReflectorT(const xReflectorT& arg): m_pT(arg.m_pT), m_f(arg.m_f), data(arg.data){};
	__forceinline xReflectorT(void* t, void* f, void* ID): m_pT(t), m_f(f), data(ID){};
    __forceinline xReflectorT& operator=(const xReflectorT<T>& arg){m_pT=arg.m_pT;m_f=arg.m_f;data=arg.data;return *this;};
	__forceinline xaData operator()(const xData& arg) {if(!m_pT || !m_f) return 0; return (((T*)m_pT)->*(*(F*)&m_f))(arg, data);};
    __forceinline bool operator==(xReflectorT<T>& arg){return (m_pT==arg.m_pT && m_f==arg.m_f && data==arg.data);};
    __forceinline bool IsEmpty(){return m_f==0;};
	void *m_pT;
	void *m_f;
    void* data;
};
 
class xReflector : public xReflectorT<xReflectorBase>{
public:
    __forceinline xReflector() : xReflectorT() {};
	template <typename F> __forceinline xReflector(void* t, F f, void* d=0) : xReflectorT((void*)t,*(void**)&f,d){};
};

// Для DT_IDREFLECTOR
struct xReflectorData
{ 
    xReflector reflector;
	__int8 Name[0];        // имя объекта, если используется
};

class REF : public std::vector<xData>
{
public:
	REF(const char* ref)
	{
		const char* r = ref;
		std::string buf;
		while (*r)
		{
			if (*r=='\\')
			{
				if (buf[0]=='0' && buf.size()==1)
					push_back(xData(0));
				else
					if (int a = atoi(buf.data()))
						push_back(xData(a));
					else
						push_back(xData(buf.data()));
				buf.clear();
				r++;
				continue;
			}
			buf += *r;
			r++;
		}
		if (!buf.empty())
			if (buf[0]=='0' && buf.size()==1)
				push_back(xData(0));
			else
				if (int a = atoi(buf.data()))
					push_back(xData(a));
				else
					push_back(xData(buf.data()));
	};
};


inline const xaData xData::operator[](const REF &V) const {
	if (V.empty()) return xData();
	std::vector<xData>::const_iterator iter = V.begin();
	xaData r = operator[](*iter);
	iter++;
	for (;iter!=V.end();iter++)
		r.MoveData(r[*iter]);
	return  r;
};

#pragma warning( default : 4127 )
#pragma warning( default : 4100 )
#pragma warning( default : 4003 )

#endif