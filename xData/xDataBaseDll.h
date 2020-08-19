#ifndef xDataBaseDll_class
#define xDataBaseDll_class

class __declspec(novtable) xDataBase;
template <class CONSTRUCTOR> class __declspec(novtable) xDataTemplate;
template <__int32 I, class T> class __declspec(novtable) x_InitWithoutCs;
class __declspec(novtable) x_CopyDataWithoutPointers;
class __declspec(novtable) x_MoveData;
class __declspec(novtable) x_CopyData;
class REF;
class XDATA_API __declspec(novtable) xDataBaseDll : public xDataCore{
	friend class __declspec(novtable) x_CopyData;
	friend class __declspec(novtable) x_CopyDataWithoutPointers;
public:
	__forceinline xDataBaseDll () {};
    __forceinline xDataBaseDll(const xDataBaseDll& V) {*(__int32*)this=0;CopyData(V,Pointer);};

	__forceinline void Throw(unsigned __int32 /*ExceptionNumber*/) const
	{
		#ifndef _WIN32_WCE
		//if (Exception)	
		//	throw new xDataException(ExceptionNumber);
		#endif
	};

    ~xDataBaseDll() {
        Invalidate(false,true,true);
    };

	void Invalidate(
		bool AsPointered=false, // Объект после разружения должен быть указательным
		bool ResetName=false,   // Сброс имени объекта
		bool ResetCS=false);    // Сброс критической секции

	static void* alloc(size_t size);
	static void free(void* ptr);

	operator bool() const;
	operator float() const;
	operator double() const;
	operator unsigned __int8() const;
	operator __int8() const;
	operator unsigned __int16() const;
	operator __int16() const;
	operator unsigned __int32() const;
	operator __int32() const;
	operator unsigned __int64() const;
	operator __int64() const;
	operator char*();
	operator wchar_t*();

	// Возвращается указатель на имя объекта
	char* GetName() const;					
	// Установка имени объекта
	void SetName(const char* Name=NULL);			
    // Сравнение с именем объекта
    bool CompareWithName(const char* Name) const;
    bool CompareWithThisName(const char* Name) const;

	unsigned __int8 DeterminateTypeOfDataInString() const;

	xDataBaseDll& operator=(const xDataBaseDll &V);
    xDataBaseDll& operator+=(const xDataBaseDll &V);

    xaData operator[](const xDataBaseDll &V) const;
    xDataBaseDll& operator<<(const xDataBaseDll &V);

   	bool operator==(const xDataBaseDll &V) const;

	inline xData GetObjectDescriptor(bool UseCrLf=true, int level=0) const;
	char* GetDebugObjectDescriptor() const;

    bool FromXml(char* XmlText, xData& ErrDescr=*(xData*)NULL);

    // Возвращает простое значение объекта (если возможно).
    xaData GetSimpleValue(const xData& Index = *(xData*)0, const xData& DefValue = *(xData*)0) const;

    // Копирование данных с удалением указателей и рефлекторов, если RemovePointers==true
	// Если копируемый объект имеет имя, то имя тоже копируется. Иначе имя не изменяется
	// Предполагается, что объект не содержит данных и не является указательным, но может иметь имя и критическую секцию
	xDataBaseDll& CopyData(const xDataBaseDll& V, bool RemovePointers=false);

    // Сохранение линейного представления объекта
    unsigned __int32 GetLinearObject(   // использованный размер буфера. 0, если буфер недостаточен
        void * buf,                     // буфер, в котором будет сохранено линейное представление объекта
        unsigned __int32 size);   // размер этого буфера

    // Вспомогательная функция для сохранения объекта в линейный буфер
    unsigned __int32 SaveToLinear(			// Возвращается количество записанных байт в области данных
		__int8* Buffer,						// Буфер, в который будет сохранено состояние объекта
		__int8* ChildBuffer,				// Записывается место, откуда можно записывать дочерние объекты
											// Возвращается первое свободное место
		__int8* Data,                       // Буфер, в который будут записаны данные объекта, при необходимости
        unsigned __int32 size);             // количество оставшихся байт

	// Требуемое количество байт для хранения линейного представления объекта
	unsigned __int32 GetLinearSize() const;

    void Remove(const xData &index);

    void Allocate(unsigned __int32 count, const xData& value);

protected:
    void Push(const void* Data, unsigned __int32 CountOfElements, unsigned __int8 TypeOfData);


	// Указатель на строку, являющейся форматом (описанием) для другой строки (для char и wchat_t)
	void* GetPointerToFormatString();

    // для чисел формат %[nn|-nn|0nn][.pp]v|e|i
	// '-' - выравнивание числа по левому краю, '0' - заполнение числа слева нулями
	// nn - количество символов, отведенных под отображение числа (вместе со знаком, точкой и экспонентой)
	// по умолчанию, при указании размера числа используется выравнивание по правому краю
	// pp - точность числа (максимальное количество знаков после точки),
	// при использовании типа 'i' - определяет основание системы исчисления
	// v - число (по возможности без экспоненты), e - число с экспонентой, 
	// i - целое число в заданной системе исчисления (по умолчанию - 10). 
public:

    xData& ToNumber(unsigned __int8 Type);

	unsigned __int8 ToString(					// возвращается использованное количество символов из строки формата
		bool Unicode = false,					// тип строки
		char* format = NULL);					// формат строки для чисел

	// Определение формата вывода строки
	unsigned __int8 ParseFormatStringForNumberAndGetSize(
		bool UnicodeString,
		void* FormatString,
		unsigned __int8& precisionorradix = *(unsigned __int8*)0,  // не изменяется, если нет в строке
		unsigned __int8& maxsymbols = *(unsigned __int8*)0,
		bool& expalways = *(bool*)0,
		bool& zerofill = *(bool*)0,
		bool& rightalign = *(bool*)0);

	// Преобразование целого числа в ANSI строку 
	bool I2S(unsigned __int64 V,			// Число
		int sign,							// Знак числа
		char* S,							// Буфер для строки
		unsigned __int8 maxsymbols,			// Максимальное количество символов
		bool zerofill=false,				// Заполнение нулями слева
		bool rightalign=false,				// Выравнивание числа вправо
		unsigned __int8 radix = 10);		// основание числа

	// Преобразование вещественного числа в ANSI строку. Если expalways==false и precision=MAXPRECISION, 
	// то предпочтение будет отдано в сторону более точного представления числа
	// Размер буфера под строку - не менее 24 байт.
	bool D2S(double V,								// Число
		char* S,									// Буфер для строки
		unsigned __int8 maxsymbols = 24,			// Максимальное количество символов для числа
		unsigned __int8 precision=MAXPRECISION,		// Количество значащих цифр после точки
		bool expalways=false,						// Представление числа в виде экспоненты
		bool zerofill=false,						// Заполнение нулями слева
		bool rightalign=false);						// Выравнивание числа вправо
public:
    // Преобразование в комплексный контейнер.
	xDataBaseDll* ToComplexContainer(			// Возвращается указатель на первый добавленный элемент
		unsigned __int32 AddonElementsCount);	// Количество добавляемых элементов (кроме уже имеющихся)

};

#endif