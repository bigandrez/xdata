// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
//#include "xData\xData.h"
#include "xDataTest.h"


#include <stdlib.h>

#include <time.h>
#include <crtdbg.h>


struct A{
	A()
	{
		Sleep(100);
	};
};

struct B{
	B()
	{
		Sleep(100);
	};
};

struct C{
	C()
	{
		Sleep(100);
	};
};

class __declspec(novtable) D:public A, public B, public C{
public:
	D() : A(), B(), C()
	{
		Sleep(100);
	};
};


int main(int /*argc*/, char* /*argv[]*/)
{

	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);	
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint( &s1 );

	xDataTest *o = new xDataTest;

/************************************************************/
/**************** начало тестовой зоны **********************/
    //for (int i=0;i<10000;i++)
    //{
    //start();

	if (!o->ContainerTest1()) return 0;
	if (!o->EqualTest()) return 0;
	if (!o->InitTest()) return 0;
	if (!o->ConvertionTest()) return 0;
	if (!o->ObjectDescriptionTest()) return 0;
	if (!o->StringTest1()) return 0;
	if (!o->NameTest1()) return 0;
	if (!o->ContainerTest4()) return 0;
	if (!o->PointerTest1()) return 0;
	if (!o->ContainerTest2()) return 0;
	if (!o->ContainerTest3()) return 0;
	if (!o->ContainerTest5()) return 0;
	if (!o->ReflectorTest()) return 0;
	if (!o->StoringTest()) return 0;
    if (!o->XmlTest()) return 0;
    //end();

    //Sleep(10);
    //}

	//if (!o->ConstructorsTest()) return 0;
	//if (!o->SyncronizeTest()) return 0;
	//if (!o->StoreAndRestoreTest()) return 0;
	//if (!o->ExceptionTest1()) return 0;
	if (!o->CompareTest()) return 0;
	delete o;

/**************** конец тестовой зоны ***********************/
/************************************************************/
	_CrtMemCheckpoint( &s2 );
	_CrtDumpMemoryLeaks();
	if ( _CrtMemDifference( &s3, &s1, &s2 ) )
		_CrtMemDumpStatistics( &s3 );	



	//xcpData d1((unsigned __int64)3, "abc");
	//int a= int(true);
	//if (a==3)
	//	Beep(440,10);
	//xcpData d2(d1);
	//int i = sizeof(d2);
	//d1.EnterCS();
	//d1.LeaveCS();
	//d1 =d2;
//	xcData d3;
//	bool r = d.IsEmpty();
	
}
