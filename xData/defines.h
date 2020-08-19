// Версия библиотеки
#define XDATA_VERSION L"0, 2, 0, 6"
#define XDATA_VERSIOND 0, 2, 0, 6

#include "stdlib.h"

#define DATAVALUE ((xData*)this)->DataValue
#define EXTDATAVALUE ((xValue*)(((xData*)this)->DataValue+unsigned __int32(this)))->ExtDataValue
#define CREATELINK(V) (unsigned __int32(V) - unsigned __int32(this))

#define CONST_INIT_CONSTRUCTOR(X,T)\
 ;##X(const             bool &V): ##T(V) {};\
 ;##X(const           __int8 &V): ##T(V) {};\
 ;##X(const unsigned  __int8 &V): ##T(V) {};\
 ;##X(const          __int16 &V): ##T(V) {};\
 ;##X(const unsigned __int16 &V): ##T(V) {};\
 ;##X(const          __int32 &V): ##T(V) {};\
 ;##X(const unsigned __int32 &V): ##T(V) {};\
 ;##X(const          __int64 &V): ##T(V) {};\
 ;##X(const unsigned __int64 &V): ##T(V) {};\
 ;##X(const            float &V): ##T(V) {};\
 ;##X(const           double &V): ##T(V) {};\
 ;##X(const             char* V): ##T(V) {};\
 ;##X(const          wchar_t* V): ##T(V) {};

#define CONST_INIT_ARRAY_CONSTRUCTOR(X,T)\
 ;##X(const             bool *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const           __int8 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const unsigned  __int8 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const          __int16 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const unsigned __int16 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const          __int32 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const unsigned __int32 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const          __int64 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const unsigned __int64 *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const            float *V, unsigned __int32 len): ##T(V, len) {};\
 ;##X(const           double *V, unsigned __int32 len): ##T(V, len) {};

#define CONST_NAMED_INIT_CONSTRUCTOR(X,T)\
 ;##X(const             bool &V, char* Name): ##T(V,Name) {};\
 ;##X(const           __int8 &V, char* Name): ##T(V,Name) {};\
 ;##X(const unsigned  __int8 &V, char* Name): ##T(V,Name) {};\
 ;##X(const          __int16 &V, char* Name): ##T(V,Name) {};\
 ;##X(const unsigned __int16 &V, char* Name): ##T(V,Name) {};\
 ;##X(const          __int32 &V, char* Name): ##T(V,Name) {};\
 ;##X(const unsigned __int32 &V, char* Name): ##T(V,Name) {};\
 ;##X(const          __int64 &V, char* Name): ##T(V,Name) {};\
 ;##X(const unsigned __int64 &V, char* Name): ##T(V,Name) {};\
 ;##X(const            float &V, char* Name): ##T(V,Name) {};\
 ;##X(const           double &V, char* Name): ##T(V,Name) {};\
 ;##X(const             char* V, char* Name): ##T(V,Name) {};\
 ;##X(const          wchar_t* V, char* Name): ##T(V,Name) {};

#define CONST_NAMED_INIT_ARRAY_CONSTRUCTOR(X,T)\
 ;##X(const             bool *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const           __int8 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const unsigned  __int8 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const          __int16 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const unsigned __int16 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const          __int32 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const unsigned __int32 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const          __int64 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const unsigned __int64 *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const            float *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};\
 ;##X(const           double *V, unsigned __int32 len, char* Name): ##T(V, len, Name) {};

#define INIT_POINTER_CONSTRUCTOR(x1,x2,x3) \
;##x1(            bool &V): ##x2(DT_BOOL,0) {##x3;};\
;##x1(          __int8 &V): ##x2(DT_INT8,0) {##x3;};\
;##x1(unsigned  __int8 &V): ##x2(DT_UINT8,0) {##x3;};\
;##x1(         __int16 &V): ##x2(DT_INT16,0) {##x3;};\
;##x1(unsigned __int16 &V): ##x2(DT_UINT16,0) {##x3;};\
;##x1(         __int32 &V): ##x2(DT_INT32,0) {##x3;};\
;##x1(unsigned __int32 &V): ##x2(DT_UINT32,0) {##x3;};\
;##x1(         __int64 &V): ##x2(DT_INT64,0) {##x3;};\
;##x1(unsigned __int64 &V): ##x2(DT_UINT64,0) {##x3;};\
;##x1(           float &V): ##x2(DT_FLOAT,0) {##x3;};\
;##x1(          double &V): ##x2(DT_DOUBLE,0) {##x3;};

#define INIT_NAMED_POINTER_CONSTRUCTOR(x1,x2,x3) \
;##x1(            bool &V, char* Name): ##x2(DT_BOOL,0,Name) {##x3;};\
;##x1(          __int8 &V, char* Name): ##x2(DT_INT8,0,Name) {##x3;};\
;##x1(unsigned  __int8 &V, char* Name): ##x2(DT_UINT8,0,Name) {##x3;};\
;##x1(         __int16 &V, char* Name): ##x2(DT_INT16,0,Name) {##x3;};\
;##x1(unsigned __int16 &V, char* Name): ##x2(DT_UINT16,0,Name) {##x3;};\
;##x1(         __int32 &V, char* Name): ##x2(DT_INT32,0,Name) {##x3;};\
;##x1(unsigned __int32 &V, char* Name): ##x2(DT_UINT32,0,Name) {##x3;};\
;##x1(         __int64 &V, char* Name): ##x2(DT_INT64,0,Name) {##x3;};\
;##x1(unsigned __int64 &V, char* Name): ##x2(DT_UINT64,0,Name) {##x3;};\
;##x1(           float &V, char* Name): ##x2(DT_FLOAT,0,Name) {##x3;};\
;##x1(          double &V, char* Name): ##x2(DT_DOUBLE,0,Name) {##x3;};

#define INIT_ARRAY_POINTER_CONSTRUCTOR(x1,x2,x3) \
;##x1(            bool *V, unsigned __int32 len): ##x2(DT_BOOL,0) {##x3;};\
;##x1(          __int8 *V, unsigned __int32 len): ##x2(DT_INT8,0) {##x3;};\
;##x1(unsigned  __int8 *V, unsigned __int32 len): ##x2(DT_UINT8,0) {##x3;};\
;##x1(         __int16 *V, unsigned __int32 len): ##x2(DT_INT16,0) {##x3;};\
;##x1(unsigned __int16 *V, unsigned __int32 len): ##x2(DT_UINT16,0) {##x3;};\
;##x1(         __int32 *V, unsigned __int32 len): ##x2(DT_INT32,0) {##x3;};\
;##x1(unsigned __int32 *V, unsigned __int32 len): ##x2(DT_UINT32,0) {##x3;};\
;##x1(         __int64 *V, unsigned __int32 len): ##x2(DT_INT64,0) {##x3;};\
;##x1(unsigned __int64 *V, unsigned __int32 len): ##x2(DT_UINT64,0) {##x3;};\
;##x1(           float *V, unsigned __int32 len): ##x2(DT_FLOAT,0) {##x3;};\
;##x1(          double *V, unsigned __int32 len): ##x2(DT_DOUBLE,0) {##x3;};

#define INIT_NAMED_ARRAY_POINTER_CONSTRUCTOR(x1,x2,x3) \
;##x1(            bool *V, unsigned __int32 len, char* Name): ##x2(DT_BOOL,0,Name) {##x3;};\
;##x1(          __int8 *V, unsigned __int32 len, char* Name): ##x2(DT_INT8,0,Name) {##x3;};\
;##x1(unsigned  __int8 *V, unsigned __int32 len, char* Name): ##x2(DT_UINT8,0,Name) {##x3;};\
;##x1(         __int16 *V, unsigned __int32 len, char* Name): ##x2(DT_INT16,0,Name) {##x3;};\
;##x1(unsigned __int16 *V, unsigned __int32 len, char* Name): ##x2(DT_UINT16,0,Name) {##x3;};\
;##x1(         __int32 *V, unsigned __int32 len, char* Name): ##x2(DT_INT32,0,Name) {##x3;};\
;##x1(unsigned __int32 *V, unsigned __int32 len, char* Name): ##x2(DT_UINT32,0,Name) {##x3;};\
;##x1(         __int64 *V, unsigned __int32 len, char* Name): ##x2(DT_INT64,0,Name) {##x3;};\
;##x1(unsigned __int64 *V, unsigned __int32 len, char* Name): ##x2(DT_UINT64,0,Name) {##x3;};\
;##x1(           float *V, unsigned __int32 len, char* Name): ##x2(DT_FLOAT,0,Name) {##x3;};\
;##x1(          double *V, unsigned __int32 len, char* Name): ##x2(DT_DOUBLE,0,Name) {##x3;};

#define SIMPLE_TYPE_CONSTRUCTOR(CN)\
    __forceinline xDataTemplate(##CN             bool &V) : CONSTRUCTOR(V), xData(0,0,0){}\
	__forceinline xDataTemplate(##CN           __int8 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned  __int8 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int16 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int16 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int32 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int32 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int64 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int64 &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN            float &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN           double &V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN             char* V) : CONSTRUCTOR(V), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          wchar_t* V) : CONSTRUCTOR(V), xData(0,0,0){};

#define SIMPLE_TYPE_NAMED_CONSTRUCTOR(CN)\
	__forceinline xDataTemplate(##CN             bool &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN           __int8 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned  __int8 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int16 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int16 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int32 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int32 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int64 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int64 &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN            float &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN           double &V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN             char* V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          wchar_t* V, char* Name) : CONSTRUCTOR(V, Name), xData(0,0,0){};

#define SIMPLE_TYPE_ARRAY_CONSTRUCTOR(CN)\
	__forceinline xDataTemplate(##CN             bool *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN           __int8 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned  __int8 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int16 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int16 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int32 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int32 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int64 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int64 *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN            float *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN           double *V, unsigned __int32 len) : CONSTRUCTOR(V,len), xData(0,0,0){};

#define SIMPLE_TYPE_NAMED_ARRAY_CONSTRUCTOR(CN)\
	__forceinline xDataTemplate(##CN             bool *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN           __int8 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned  __int8 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int16 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int16 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int32 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int32 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN          __int64 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN unsigned __int64 *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN            float *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};\
	__forceinline xDataTemplate(##CN           double *V, unsigned __int32 len, char* Name) : CONSTRUCTOR(V,len,Name), xData(0,0,0){};

#define DECLARE_GLOBAL_UNARY_OPERATOR1(op,sop)\
    inline xaData operator ##op(const xData& a, const             bool b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const           __int8 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const unsigned  __int8 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const          __int16 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const unsigned __int16 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const          __int32 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const unsigned __int32 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};

#define DECLARE_GLOBAL_UNARY_OPERATOR2(op,sop)\
    inline xaData operator ##op(const xData& a, const          __int64 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const unsigned __int64 b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const            float b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const           double b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const            char* b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
    inline xaData operator ##op(const xData& a, const         wchar_t* b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(xData(b)));};\
	inline xaData operator ##op(const xData& a, const       xData& b){return *(xData*)&(xData().CopyData((const xDataBaseDll&)a,true).operator ##sop(*(xDataBaseDll*)&b));};

#define DECLARE_GLOBAL_COMPARE_OPERATOR(Operator) \
	inline bool operator ##Operator(__int8 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(unsigned __int8 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(__int16 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(unsigned __int16 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(__int32 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(unsigned __int32 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(__int64 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(unsigned __int64 v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(float v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(double v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(char* v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};\
	inline bool operator ##Operator(wchar_t* v1, const xData& v2) {return xData(v1).operator  ##Operator(xData(v2));};


/*9039153643 Эдуард от Дениса  GPS и GPRS*/
/*16:00 Пушкина 61 оф. 15*/
/* 517532 инком*/