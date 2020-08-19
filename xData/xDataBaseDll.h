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
		bool AsPointered=false, // ������ ����� ���������� ������ ���� ������������
		bool ResetName=false,   // ����� ����� �������
		bool ResetCS=false);    // ����� ����������� ������

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

	// ������������ ��������� �� ��� �������
	char* GetName() const;					
	// ��������� ����� �������
	void SetName(const char* Name=NULL);			
    // ��������� � ������ �������
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

    // ���������� ������� �������� ������� (���� ��������).
    xaData GetSimpleValue(const xData& Index = *(xData*)0, const xData& DefValue = *(xData*)0) const;

    // ����������� ������ � ��������� ���������� � �����������, ���� RemovePointers==true
	// ���� ���������� ������ ����� ���, �� ��� ���� ����������. ����� ��� �� ����������
	// ��������������, ��� ������ �� �������� ������ � �� �������� ������������, �� ����� ����� ��� � ����������� ������
	xDataBaseDll& CopyData(const xDataBaseDll& V, bool RemovePointers=false);

    // ���������� ��������� ������������� �������
    unsigned __int32 GetLinearObject(   // �������������� ������ ������. 0, ���� ����� ������������
        void * buf,                     // �����, � ������� ����� ��������� �������� ������������� �������
        unsigned __int32 size);   // ������ ����� ������

    // ��������������� ������� ��� ���������� ������� � �������� �����
    unsigned __int32 SaveToLinear(			// ������������ ���������� ���������� ���� � ������� ������
		__int8* Buffer,						// �����, � ������� ����� ��������� ��������� �������
		__int8* ChildBuffer,				// ������������ �����, ������ ����� ���������� �������� �������
											// ������������ ������ ��������� �����
		__int8* Data,                       // �����, � ������� ����� �������� ������ �������, ��� �������������
        unsigned __int32 size);             // ���������� ���������� ����

	// ��������� ���������� ���� ��� �������� ��������� ������������� �������
	unsigned __int32 GetLinearSize() const;

    void Remove(const xData &index);

    void Allocate(unsigned __int32 count, const xData& value);

protected:
    void Push(const void* Data, unsigned __int32 CountOfElements, unsigned __int8 TypeOfData);


	// ��������� �� ������, ���������� �������� (���������) ��� ������ ������ (��� char � wchat_t)
	void* GetPointerToFormatString();

    // ��� ����� ������ %[nn|-nn|0nn][.pp]v|e|i
	// '-' - ������������ ����� �� ������ ����, '0' - ���������� ����� ����� ������
	// nn - ���������� ��������, ���������� ��� ����������� ����� (������ �� ������, ������ � �����������)
	// �� ���������, ��� �������� ������� ����� ������������ ������������ �� ������� ����
	// pp - �������� ����� (������������ ���������� ������ ����� �����),
	// ��� ������������� ���� 'i' - ���������� ��������� ������� ����������
	// v - ����� (�� ����������� ��� ����������), e - ����� � �����������, 
	// i - ����� ����� � �������� ������� ���������� (�� ��������� - 10). 
public:

    xData& ToNumber(unsigned __int8 Type);

	unsigned __int8 ToString(					// ������������ �������������� ���������� �������� �� ������ �������
		bool Unicode = false,					// ��� ������
		char* format = NULL);					// ������ ������ ��� �����

	// ����������� ������� ������ ������
	unsigned __int8 ParseFormatStringForNumberAndGetSize(
		bool UnicodeString,
		void* FormatString,
		unsigned __int8& precisionorradix = *(unsigned __int8*)0,  // �� ����������, ���� ��� � ������
		unsigned __int8& maxsymbols = *(unsigned __int8*)0,
		bool& expalways = *(bool*)0,
		bool& zerofill = *(bool*)0,
		bool& rightalign = *(bool*)0);

	// �������������� ������ ����� � ANSI ������ 
	bool I2S(unsigned __int64 V,			// �����
		int sign,							// ���� �����
		char* S,							// ����� ��� ������
		unsigned __int8 maxsymbols,			// ������������ ���������� ��������
		bool zerofill=false,				// ���������� ������ �����
		bool rightalign=false,				// ������������ ����� ������
		unsigned __int8 radix = 10);		// ��������� �����

	// �������������� ������������� ����� � ANSI ������. ���� expalways==false � precision=MAXPRECISION, 
	// �� ������������ ����� ������ � ������� ����� ������� ������������� �����
	// ������ ������ ��� ������ - �� ����� 24 ����.
	bool D2S(double V,								// �����
		char* S,									// ����� ��� ������
		unsigned __int8 maxsymbols = 24,			// ������������ ���������� �������� ��� �����
		unsigned __int8 precision=MAXPRECISION,		// ���������� �������� ���� ����� �����
		bool expalways=false,						// ������������� ����� � ���� ����������
		bool zerofill=false,						// ���������� ������ �����
		bool rightalign=false);						// ������������ ����� ������
public:
    // �������������� � ����������� ���������.
	xDataBaseDll* ToComplexContainer(			// ������������ ��������� �� ������ ����������� �������
		unsigned __int32 AddonElementsCount);	// ���������� ����������� ��������� (����� ��� ���������)

};

#endif