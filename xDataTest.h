//#include "stdafx.h"
#pragma once
#include <stdlib.h>
#include <Windows.h>
#include "xData\xData.h"

class xDataTest
{
public:
	xDataTest(void);
	~xDataTest(void);

	bool InitTest();

	// ���� �������-���������. 
	bool PointerTest1();

    bool ReflectorTest();
	xaData f(const xData& obj, void* ID);
	int vv;


	//// ���� ���������� DE_PointerIsConst
	//bool ExceptionTest1();
	//// ���� ������������ ����������
	bool ContainerTest2();
	bool StringTest1();
	// ���� ���������� ����������. 
	bool ContainerTest3();
	bool ContainerTest1();

	bool ContainerTest4();

	bool NameTest1();
	// ���� "��������" �������
	bool StoringTest();

	bool CompareTest();

	bool ObjectDescriptionTest();

	//bool StoreAndRestoreTest();

	bool ConvertionTest();

	bool ContainerTest5();

	//bool SyncronizeTest();
	// ���� ��������� ������������
	bool EqualTest();
	//// ���� �������������
	//bool ConstructorsTest();

	bool XmlTest();

};

