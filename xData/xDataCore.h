/*********************************************************************************************************************************/
// Ядро объекта
/*********************************************************************************************************************************/

#ifndef XDATACORE
#define XDATACORE
#include <Windows.h>

#pragma warning( disable : 4200 )

#define DT_UNDEFINE		0
#define DT_BOOL         1
#define DT_INT8			2
#define DT_INT16		3
#define DT_INT32		4
#define DT_INT64		5
#define DT_UINT8		6
#define DT_UINT16		7
#define DT_UINT32		8
#define DT_UINT64		9
#define DT_FLOAT		10
#define DT_DOUBLE		11
#define DT_STRING		12
#define DT_IDREFLECTOR	13	// Идентифицированный рефлектор
#define DT_CONTROLDATA  14	// "Управляемые" данные. Для данных, изменение которых полылается по рефлектору

#define DT_ANSISTRING		16
#define DT_UNICODESTRING	17

#define MAXPRECISION 17	// Максимальная точность вещественного числа

struct xValue
{ 
	inline xValue() : ExtDataValue(0){};
	unsigned __int32 ExtDataValue;
	__int8 Name[0];        // имя объекта, если используется
};

struct xValueCs
{ 
	inline xValueCs() : ExtDataValue(0){};
	unsigned __int32 ExtDataValue;
	CRITICAL_SECTION cs;            // Критическая секция
	__int8 Name[0];        // имя объекта, если используется
};

struct xControlData
{
	unsigned __int64 ExtDataValue;
	unsigned __int32 rpObject;		// Относительная ссылка на объект
	unsigned __int32 rpMethod;		// Относительная ссылка на метод
	unsigned __int32 ExtDataType:4;	
	__int8 Name[0];        // имя объекта, если используется
};

class XDATA_API __declspec(novtable) xDataCore{
public:
	__forceinline xDataCore() {};
//protected:
	unsigned __int32 DataType       :  4;	// Тип данных. При CONTROLDATA тип данных находится в xExtent
	unsigned __int32 Pointer        :  1;	// Объект является указателем
	unsigned __int32 Exception      :  1;	// Поддержка исключений в объекте
	unsigned __int32 Named          :  1;	
	unsigned __int32 CS	            :  1;	
	unsigned __int32 DataSize       : 24;

	unsigned __int32 DataValue;				// Данные или относительная ссылка на данные или дополнительное поле
};

#endif
