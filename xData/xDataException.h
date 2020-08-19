#pragma once

#define DE_PointerIsConst			1		// ������� ������������� �������-������ �� ���������
#define DE_InvalidLength			2		// ������� ������������� �������-������ �� ���������
#define DE_ConversionFromUndefine	3		// ������� �������������� �� ������� ��������������� ����
#define DE_ConversionFromContainer	4		// ������� �������������� � ������� ��� �� ������� ����������
#define DE_ConversionFromHandle		5		// ������� �������������� � ������� ��� �� ������� ����
#define DE_ConversionFromReflector	19		// ������� �������������� � ������� ��� �� ������� ����
#define DE_ConversionFromFiletime	6		// ������� �������������� � ������� ��� �� ���� FILETIME
#define DE_ConversionFromHResult	7		// ������� �������������� � ������� ��� �� ���� HResult
#define DE_ValueOutOfRange			8		// ����� �������� �� ������� ����������� ���������
#define DE_ConversionFromPointer    9
#define DE_SizeForDigitsTooSmall    10
#define DE_NumberFormatError	    11
#define DE_BadTypeForMathOperation	12
#define DE_ChangeStringOnPointer	13		// ������� ������ � ������, ������ ��� ������� �������� ��� ������� �������
#define DE_Equal2EmptyPointer   	14
#define DE_EqualToArray			   	15		// ������� ������������ ��������� �� ������
#define DE_OperationWithEmpty	   	16		// ������� ���������� �������� � "������" ��������
#define DE_IllegalUseOperatorShift 	17		// ������������ ������������� ��������� << ������������� � �������-���������
#define DE_IllegalUseOperatorPush 	18		// ������������ ������������� ��������� Push ������������� � �������-���������
#define DE_IllegalUseEqual			19		// ������� ������������ ��������� �� ������ ��������


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
	unsigned __int32 NumberDE;	// ����� ����������
};
