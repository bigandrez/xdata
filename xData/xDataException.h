#pragma once

#define DE_PointerIsConst			1		// Попытка инициализации объекта-ссылки из константы
#define DE_InvalidLength			2		// Попытка инициализации объекта-ссылки из константы
#define DE_ConversionFromUndefine	3		// Попытка преобразования из объекта неопределенного типа
#define DE_ConversionFromContainer	4		// Попытка преобразования в простой тип из объекта контейнера
#define DE_ConversionFromHandle		5		// Попытка преобразования в простой тип из объекта ядра
#define DE_ConversionFromReflector	19		// Попытка преобразования в простой тип из объекта ядра
#define DE_ConversionFromFiletime	6		// Попытка преобразования в простой тип из типа FILETIME
#define DE_ConversionFromHResult	7		// Попытка преобразования в простой тип из типа HResult
#define DE_ValueOutOfRange			8		// выход значения за пределы допустимого диапазона
#define DE_ConversionFromPointer    9
#define DE_SizeForDigitsTooSmall    10
#define DE_NumberFormatError	    11
#define DE_BadTypeForMathOperation	12
#define DE_ChangeStringOnPointer	13		// Попытка записи в строку, память под которую выделена вне методов объекта
#define DE_Equal2EmptyPointer   	14
#define DE_EqualToArray			   	15		// Попытка присваивание указателю на массив
#define DE_OperationWithEmpty	   	16		// Попытка выполнения операции с "пустым" объектом
#define DE_IllegalUseOperatorShift 	17		// Недопустимое использование оператора << применительно к объекту-указателю
#define DE_IllegalUseOperatorPush 	18		// Недопустимое использование оператора Push применительно к объекту-указателю
#define DE_IllegalUseEqual			19		// Попытка присваивания указателю на массив объектов


class XDATA_API xDataException
{
public:
	xDataException();
	xDataException(unsigned __int32 ExceptionNumber);
	xDataException(xDataException& Object);
	~xDataException(void);
	char* GetDescription();
	inline unsigned __int32 GetNumber(){return NumberDE;};
private:
	unsigned __int32 NumberDE;	// Номер исключения
};
