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

	// Тест объекта-указателя. 
	bool PointerTest1();

    bool ReflectorTest();
	xaData f(const xData& obj, void* ID);
	int vv;


	//// Тест исключения DE_PointerIsConst
	//bool ExceptionTest1();
	//// Тест комплексного контейнера
	bool ContainerTest2();
	bool StringTest1();
	// Тест контейнера указателей. 
	bool ContainerTest3();
	bool ContainerTest1();

	bool ContainerTest4();

	bool NameTest1();
	// Тест "спасения" объекта
	bool StoringTest();

	bool CompareTest();

	bool ObjectDescriptionTest();

	//bool StoreAndRestoreTest();

	bool ConvertionTest();

	bool ContainerTest5();

	//bool SyncronizeTest();
	// Тест оператора присваивания
	bool EqualTest();
	//// Тест конструкторов
	//bool ConstructorsTest();

	bool XmlTest();

};

