#include "xDataTest.h"

#pragma warning( disable : 4189 )

xDataTest::xDataTest(void)
{
}

xDataTest::~xDataTest(void)
{
}


bool xDataTest::ReflectorTest()
{
	{
        xData d2;
		xrData d1(this, &xDataTest::f, (void*)1);
		vv=0;
		// Производится вызов функции f
		d1(1);
		if (vv!=1) goto ERR;
		d2 = d1;
        d2 << 1;
		d2[0](2);
		if (vv!=2) goto ERR;
	}

	// Добавить поддержку DT_REFLECTOR в остальные функции
	return true;
ERR:
	printf("ReflectorTest не пройден");
	return false;
}

xaData xDataTest::f(const xData& obj, void* /*ID*/)
{
	vv=obj;
	return vv;
}

bool xDataTest::InitTest()
{
xaData str;
	{
		int mas[3]={111,2,3};
		const double cmas[3]={1,2,3};

		xData d1(__int16(1));
		xpData d2(__int64(1));
		xeData d3("123");
		xpeData d4(L"456");

		xData d5(__int8(3),"name");
		xpData d6(__int32(3),"name");
		xeData d7("123","name");
		xpeData d8(L"456","name");

		xData d9(cmas,3);
		xData d10(cmas,2,"name");
		xpData d11(cmas,3);
		xpData d12(mas,3,0);

        str = d1.GetObjectDescriptor(false)+"\n"+d2.GetObjectDescriptor(false)+"\n"+d3.GetObjectDescriptor(false)+"\n"+d4.GetObjectDescriptor(false)+"\n"+
			d5.GetObjectDescriptor(false)+"\n"+d6.GetObjectDescriptor(false)+"\n"+d7.GetObjectDescriptor(false)+"\n"+d8.GetObjectDescriptor(false)+"\n"+
            d9.GetObjectDescriptor(false)+"\n"+d10.GetObjectDescriptor(false)+"\n"+d11.GetObjectDescriptor(false)+"\n"+d12.GetObjectDescriptor(false);

        char* c = str;
        char z[]=\
"DT_INT16, size=1, value=1\nDT_INT64, size=1, value=1\nDT_STRING, ansi, exception, size=4, value=123\nDT_STRING, unicode, exception, size=4, value=456\n"\
"[name] DT_INT8, size=1, value=3\n[name] DT_INT32, size=1, value=3\n[name] DT_STRING, ansi, exception, size=4, value=123\n"\
"[name] DT_STRING, unicode, exception, size=4, value=456\nDT_DOUBLE, size=3\n   1, 2, 3\n[name] DT_DOUBLE, size=2\n   1, 2\nDT_DOUBLE, size=3\n   1, 2, 3\n"\
"Pointer(DT_INT32), size=3\n   111, 2, 3";
        if (strcmp(z,c)!=0) goto ERR;
	}

	{
		__int64 mas[3]={111,2,3};
		const int cmas[3]={1,2,3};

		xcData d1(__int16(1));
		xcpData d2(__int64(1));
		xceData d3("123");
		xcpeData d4(L"456");

		xcData d5(__int8(1),"name");
		xcpData d6(__int32(1),"name");
		xceData d7("123","name");
		xcpeData d8(L"456","name");

		xcData d9(mas,3);
		xcData d10(cmas,1,"name");
		xcpData d11(cmas,3);
		xcpData d12(mas,3,0);

    str = d1.GetObjectDescriptor(false)+"\n"+d2.GetObjectDescriptor(false)+"\n"+d3.GetObjectDescriptor(false)+"\n"+d4.GetObjectDescriptor(false)+"\n"+
			d5.GetObjectDescriptor(false)+"\n"+d6.GetObjectDescriptor(false)+"\n"+d7.GetObjectDescriptor(false)+"\n"+d8.GetObjectDescriptor(false)+"\n"+
            d9.GetObjectDescriptor(false)+"\n"+d10.GetObjectDescriptor(false)+"\n"+d11.GetObjectDescriptor(false)+"\n"+d12.GetObjectDescriptor(false);

        char* c = str;
        char z[]=
"DT_INT16, CS, size=1, value=1\nDT_INT64, CS, size=1, value=1\nDT_STRING, ansi, exception, CS, size=4, value=123\nDT_STRING, unicode, exception, CS, size=4, value=456\n"\
"[name] DT_INT8, CS, size=1, value=1\n[name] DT_INT32, CS, size=1, value=1\n[name] DT_STRING, ansi, exception, CS, size=4, value=123\n"\
"[name] DT_STRING, unicode, exception, CS, size=4, value=456\nDT_INT64, CS, size=3\n   111, 2, 3\n[name] DT_INT32, CS, size=1, value=1\nDT_INT32, CS, size=3\n   1, 2, 3\n"\
"Pointer(DT_INT64), CS, size=3\n   111, 2, 3";
   
        if (strcmp(z,c)!=0) goto ERR;
	}

	// Тест инициализации константы
	{
		xData d1(1);
		xpData d2(1);	// Должно произойти копирование данных, как в предыдущей строке
		if (int(d1)!=1) goto ERR;
		if (int(d2)!=1) goto ERR;
	}
	// Текст инициализации строки
	{
		xData  d1("abc");
		xpData d2("abc");// Должно произойти копирование данных, как в предыдущей строке
		xData  d3(L"abc");
		xpData d4(L"abc");// Должно произойти копирование данных, как в предыдущей строке

		if (strcmp(d1,"abc")!=0) goto ERR;
		if (strcmp(d2,"abc")!=0) goto ERR;
		if (wcscmp(d3,L"abc")!=0) goto ERR;
		if (wcscmp(d4,L"abc")!=0) goto ERR;
	}
	// Тест инициализации переменной
	{
		float p=1.0f;
		xData d1(p);
		xpData d2(p);
		xData r1 = d1.GetObjectDescriptor(false);
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor(false);
		char* c2 = r2;
		if (d1.IsPointer()) goto ERR;
		if (!d2.IsPointer()) goto ERR;
	}
	// Тест инициализации простого объекта
	{
		xData d1(3);
	    xData d2(d1);
		xpData d3(&d1);
	    xData r1 = d2.GetObjectDescriptor(false);
		char* c1 = r1;
		xData r2 = d3.GetObjectDescriptor(false);
	    char* c2 = r2;
		if (d2.IsPointer() || int(d2)!=3) goto ERR;
		if (!d3.IsPointer() || int(d3)!=3) goto ERR;
	}
	{
		xData d1;
		float p=3.14f;
		float e=2.81f;
		d1 << xpData(p,"pi");
		d1 << xpData(e,"e");
		xData d2(d1);
		xpData d3(&d1);

		xData r1 = d1.GetObjectDescriptor(false);
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor(false);
		char* c2 = r2;
		xData r3 = d3.GetObjectDescriptor(false);
		char* c3 = r3;

		if (strcmp(c3,"Pointer(DT_UNDEFINE), size=1\n   DT_UNDEFINE, size=2\n      [pi] pointer(DT_FLOAT), size=1, value=3.140000104904174\n"
"      [e] pointer(DT_FLOAT), size=1, value=2.809999942779541")!=0) goto ERR;
	}

	// Тест инициализации объекта-указателя
	{
		float p=3.14f;
		xpData d1(p);
		xData d2(d1);
		xpData d3(&d1);
		xData r1 = d2.GetObjectDescriptor(false);
		char* c1 = r1;
		xData r2 = d3.GetObjectDescriptor(false);
		char* c2 = r2;
        float dd = float(d3);
		if (d2.IsPointer() || double(d2)!=3.1400001049041748) goto ERR;
		if (!d3.IsPointer()) goto ERR;
		if (float(d3)!=3.1400001f) goto ERR;
	}
	// Тест инициализации простого объекта
	{
		float p=3.14f;
		xpData d1(p);
		xData d2(d1);
		xpData d3(&d1);
		if (d2.IsPointer() || float(d2)!=3.14f) goto ERR;
		if (!d3.IsPointer() || float(d3)!=3.14f) goto ERR;
	}

	return true;
ERR:
	printf("InitTest не пройден");
	return false;
}

bool xDataTest::ConvertionTest()
{
	// Тест преобразования строки в число
	{
		int v1 = xData("1.2");
		if (v1!=1) goto ERR;
		float v2 = xpData(L"3.14");
		if (v2!=3.14f) goto ERR;
	}
	// Тест преобразования числа в строку
	{
		xData d1(1);
		char* c1 = d1;
        if (strcmp(c1,"1")!=0) goto ERR;
		xData d2(1.3);
		wchar_t* c2 = d2;
        if (wcscmp(c2,L"1.3")!=0) goto ERR;
		xData d3(3.14f);
		char* c3 = d3;
        if (strcmp(c3,"3.140000104904174")!=0) goto ERR;
		xData d4(true);
		wchar_t* c4 = d4;
        if (wcscmp(c4,L"true")!=0) goto ERR;
	}
	return true;
ERR:
	printf("ConvertionTest не пройден");
	return false;
}


//// Тест исключения DE_PointerIsConst
//
//bool xDataTest::ExceptionTest1()
//{
//	try{xpeData d1(100);goto ERR;}
//	catch (xDataException* e){	
//		if (e->GetNumber()!=DE_PointerIsConst) 
//		{//delete e;
//		goto ERR;}	
////		delete e;	
//	}
//	return true;
//ERR:
//	printf("ExceptionTest1 не пройден");
//	return false;
//}
//
//// Тест простого контейнера
bool xDataTest::ContainerTest1()
{

    {
        xData d1;
        xData d2="name";
        d1 << xData(123,"name");
        xData r1 = d1.GetObjectDescriptor(false); char *c1=r1;
        if (d1["name"]!=123) goto ERR;
        if (d1[xpData(d2)]!=123) goto ERR;
    }
    
    {
        xData d1 = 3.14;
        xpData d2(d1);
        xData d3 = d2.GetSimpleValue();
        xData r1 = d3.GetObjectDescriptor(false); char *c1=r1;
        if (d3.GetThisDataType()!=DT_DOUBLE || d3!=3.14) goto ERR;
    }
    
    {
        xData d1(3.14,"pi");
        xpData d2(&d1);
        xpData d3 = d2.GetSimpleValue("pii",3.1415);
        xData r1 = d3.GetObjectDescriptor(false); char *c1=r1;
        if (d3.GetThisDataType()!=DT_DOUBLE || d3!=3.1415) goto ERR;
    }

    return true;
ERR:
	printf("ContainerTest1 не пройден");
	return false;
}


// Тест комплексного контейнера
bool xDataTest::ContainerTest2()
{
	{
		xData d1;
        d1 << __int8(1);
        d1.operator << (__int8(3));

        xData r1 = d1.GetObjectDescriptor(false);
		char* c = r1;

        char z[]="DT_INT8, size=2\n   1, 3";
        if (strcmp(z,c)!=0) goto ERR;
	}
	{
		xData d1;
        float mas[3]={1,2,3};
        d1 << L"abcdef";
        d1 << xData("COM1","name");
        d1 << xData(3.14,"pi");
        d1 << __int8(1); 
        d1.Push(mas,3);

        xData r1 = d1.GetObjectDescriptor(false);
		char* c = r1;

        char z[]="DT_UNDEFINE, size=7\n   DT_STRING, unicode, size=7, value=abcdef\n   [name] DT_STRING, ansi, size=5, value=COM1\n"\
            "   [pi] DT_DOUBLE, size=1, value=3.14\n   DT_INT8, size=1, value=1\n   DT_FLOAT, size=1, value=1\n   DT_FLOAT, size=1, value=2\n   DT_FLOAT, size=1, value=3";
        if (strcmp(z,c)!=0) goto ERR;
	}

	return true;
ERR:
	printf("ContainerTest2 не пройден");
	return false;
}

// Тест объекта-указателя. 
bool xDataTest::PointerTest1()
{
	// Проверка присваивания переменной через двойную вложенную ссылку на переменную
	{
		int i=1;
		xpData d1(i);
		xpData d2(d1);
		d2 = 2;

		if (i!=2)
			goto ERR;
	}

	// Проверка присваивания переменной через двойную вложенную ссылку на объект данных
	{
		xData d1(double(10.3));
		xpData d2(d1);
		int v1=d2;
		float v2=d2;
		if (v1!=10 && v2!=10.3)
			goto ERR;
	}

	// Смешанная проверка
	{
		__int8 v1=3;
		xpData d1(v1);
		xpData d2(d1);
		unsigned __int64 v2=0;
		xpData d3(v2);
		xpData d4(d3);
		d4=d2;// Должно произойти присваивание v2=v1
		if (v1!=__int8(v2))
			goto ERR;
	}
	return true;
ERR:
	printf("PointerTest1 не пройден");
	return false;
}
//
bool xDataTest::StringTest1()
{
	{
		xData b;
		xData a(1);
		b = a + "3.14";
		char* t = b;
		if (!t || t[0]!='4') goto ERR;
	}
	// Тест форматированного вывода числа
	{
		xData a;
		xData b;
		b = "string %03v test";
		a = b + 3;
		b += 3.1415;
		if (strcmp((char*)a, "string 003 test")!=0) goto ERR;
		if (strcmp((char*)b, "string 3.1 test")!=0) goto ERR;
	}
	// Тест адресации символов в ANSI строке
	{
		xData d("0123456789");
		for (int i=0;i<10;i++)
			d[i] = i+65;
		char* t = d;
		if (memcmp(t,"ABCDEFGHIJ",10)!=0) goto ERR;
	}
	// Тест адресации символов в UNICODE строке
	{
		xData d(L"0123456789");
		for (int i=0;i<10;i++)
			d[i] = L'A'+i;
		wchar_t* t = (wchar_t*)d;
		if (memcmp(t,L"ABCDEFGHIJ",20)!=0) goto ERR;
	}
	return true;
ERR:
	printf("StringTest1 не пройден");
	return false;
}

// Тест контейнера указателей. 
bool xDataTest::ContainerTest3()
{
	__int16 v1=1;
	unsigned __int32 v2=2;
	unsigned __int8 v3;
	float v4;
	double v5;

	xData d1;

	d1 << xData(v1);
	d1 << xpData(v2);
	d1 << "123";


	v1=3;
	v2=4;
	v3 = d1[0];
	v4 = d1[1];
	v5 = d1[2];

    if (v3!=1 || v4!=4 || v5!=123)
		goto ERR;

	return true;
ERR:
	printf("ContainerTest3 не пройден");
	return false;
}

bool xDataTest::ContainerTest4()
{
	{
		xData d1,d2;
		d1 << 1;
		d1 << 2;
		d2 = d1;
		if (int(d1[0])!=int(d2[0])) goto ERR;
		if (int(d1[1])!=int(d2[1])) goto ERR;
	}
	{
		xpData d2;
		xData d1;
		int v1=1;
		float v2=2;
		d1 << xpData(v1);
		d1 << xpData(v2);

		d2 = d1;

		v1=3;v2=4;

		xData r1 = d1.GetObjectDescriptor(false);
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor(false);
		char* c2 = r2;

		if (int(d2[0])!=3) goto ERR;
 		if (int(d1[1])!=4) goto ERR;
	}
    {
        xData d1;
        double pi=3.14;
        d1 << xData(1);
        d1 << xData("string");
        d1 << xpData(pi,"pi");
        d1 << xData("test","test");

        d1.Remove("pi");
        xData r1 = d1.GetObjectDescriptor(false);
        char* c1 = r1;
        d1.Remove(L"test");
        xData r2 = d1.GetObjectDescriptor(false);
        char* c2 = r2;
        d1.Remove(0);
        xData r3 = d1.GetObjectDescriptor(false);
        char* c3 = r3;
        if (strcmp(c3,"DT_UNDEFINE, size=1\n   DT_STRING, ansi, size=7, value=string")!=0) goto ERR;
    }


	return true;
ERR:
	printf("ContainerTest4 не пройден");
	return false;
}

bool xDataTest::NameTest1()
{
	{
		xData d1(1);				// Не именованный
		xData d2(2,"Именованый");	// Именованный
		d1=d2;	// Должно скопироваться не только содержимое, но и имя
		if (strcmp((char*)d1.GetName(), (char*)d2.GetName())!=0) goto ERR;
		if (int(d1)!=int(d2)) goto ERR;
	}
	{
		xData d1(1,"Тоже именованный");	// Именованный
		xData d2(2,"Именованый");		// Именованный
		d1=d2;	// Должно скопироваться не только содержимое, но и имя
		if (strcmp((char*)d1.GetName(), (char*)d2.GetName())!=0) goto ERR;
		if (int(d1)!=int(d2)) goto ERR;
	}
	{
		xData d1(1,"Именованый");	// Именованный
		xData d2(2);				// Не именованный
		d1=d2;	// Имя должно должно остаться
		if (int(d1)!=int(d2)) goto ERR;
	}
	{
		int v1=1;
		double v2=2;
		xData d1(v1);				// Не именованный
		xpData d2(v2,"Именованый");	// Именованный
		d1=d2;	// Должно скопироваться не только содержимое, но и имя
		if (int(d1)!=int(d2)) goto ERR;
		if (strcmp((char*)d1.GetName(), (char*)d2.GetName())!=0) goto ERR;
	}
	{
		// При добавлении именованного объекта должен создаться комплексный контейнер
		xData d1;
		d1 << xData("abc","name");
		if (strcmp(d1[0].GetName(),"name")!=0) goto ERR;
	}

	return true;
ERR:
	printf("NameTest1 не пройден");
	return false;
}

// Тест "спасения" объекта
bool xDataTest::StoringTest()
{
	{
		__int8 buf[1024]={0};
		xData d1(10);
        unsigned __int32 usedsize = d1.GetLinearObject(buf,sizeof(buf));
        unsigned __int32 linearsize = d1.GetLinearSize();
        if (linearsize!=usedsize) goto ERR;
		xData *d2 = (xData *)buf;
        xData r = d2->GetObjectDescriptor(false);
        char* c = r;
        if (strcmp(c,"DT_INT32, size=1, value=10")!=0) goto ERR;
	}
	{
		// Тест для именованного объекта со строкой
		__int8 buf[1024]={0};
		xData d1(L"100", "name");
        unsigned __int32 usedsize = d1.GetLinearObject(buf,sizeof(buf));
        unsigned __int32 linearsize = d1.GetLinearSize();
        if (linearsize!=usedsize) goto ERR;
		xData *d2 = (xData *)buf;
        xData r = d2->GetObjectDescriptor(false);
        char* c = r;
        if (strcmp(c,"[name] DT_STRING, unicode, size=4, value=100")!=0) goto ERR;
	}
	{
		// Тест для простого контейнера
		__int8 buf[1024]={0};
		__int16 ar[10];
		for (int i=0;i<10;i++) ar[i] = (__int16)i;
		xData d1(ar,10,"array");
		unsigned __int32 usedsize = d1.GetLinearObject(buf,sizeof(buf));
        unsigned __int32 linearsize = d1.GetLinearSize();
        if (linearsize!=usedsize) goto ERR;
		xData *d2 = (xData *)buf;
        xData r = d2->GetObjectDescriptor(false);
        char* c = r;
        if (strcmp(c,"[array] DT_INT16, size=10\n   0, 1, 2, 3, 4, 5, 6, 7, 8, 9")!=0) goto ERR;
	}
	// Тест указателя на объект
	{
		xData d1(float(3),"name");
		xpData d2(&d1);
		d1=3.14;
		__int8 buf[1024]={0};
		unsigned __int32 usedsize = d2.GetLinearObject(buf,33);
        unsigned __int32 linearsize = d2.GetLinearSize();
        if (linearsize!=usedsize) goto ERR;
		xData *d3 = (xData *)buf;
        xData r = d3->GetObjectDescriptor(false);
        char* c = r;
        if (strcmp(c,"DT_UNDEFINE, size=1\n   [name] DT_DOUBLE, size=1, value=3.14")!=0) goto ERR;
	}
	// Тест указателя на объект
	{
		xData d1;
        d1 << xData(1,"name1");
        d1 << xData(2,"name2");
		__int8 buf[1024];
		unsigned __int32 usedsize = d1.GetLinearObject(buf,44);
        unsigned __int32 linearsize = d1.GetLinearSize();
        if (linearsize!=usedsize) goto ERR;
		xData *d3 = (xData *)buf;
        xData r = d3->GetObjectDescriptor(false);
        char* c = r;
        if (strcmp(c,"DT_UNDEFINE, size=2\n   [name1] DT_INT32, size=1, value=1\n   [name2] DT_INT32, size=1, value=2")!=0) goto ERR;
	}

	{
		xData d1,d2;
        d2 << xData(1,"name1");
        d2 << xData(2,"name2");
        d1.Allocate(100,d2);
        xData rr = d1.GetObjectDescriptor(false);
        char* cc = rr;
		__int8 buf[8192];
		unsigned __int32 usedsize = d1.GetLinearObject(buf,8192);
        unsigned __int32 linearsize = d1.GetLinearSize();
        if (linearsize!=usedsize) goto ERR;
		xData *d3 = (xData *)buf;
        xData r = d3->GetObjectDescriptor(false);
        char* c = r;
        if (strcmp(c,cc)!=0) goto ERR;
	}

    {
		xData d1,d2;
        d2 << xData(1,"ReceiveDateTime");
        d2 << xData(39345.80061342592,"DateTime");
        d2 << xData(56.4778556823730,"Latitude");
        d2 << xData(85.0116958618164,"Longitude");
        d2 << xData(0,"Speed");
        d2 << xData(11.89999961853027,"Voltage");
        d2 << xData(0.0,"Analogue");
        d2 << xData(1,"Ignition");
        d2 << xData(0,"Protection");
        d2 << xData(0,"Alarm");
        d2 << xData(0,"Lock");
        d2 << xData(3,"Status");
        for (int i=0;i<20;i++)
            d1 << d2;

        xData rr = d1.GetObjectDescriptor(false);
        char* cc = rr;

        __int8 buf[8192];
		unsigned __int32 usedsize = d1.GetLinearObject(buf,8192);
        unsigned __int32 linearsize = d1.GetLinearSize();
        if (linearsize!=usedsize) goto ERR;
		xData *d3 = (xData *)buf;

        xData r = d3->GetObjectDescriptor(false);
        char* c = r;
    }
	{
		unsigned __int64 buf[2048];
		float buf2[2048];
		unsigned __int16 buf3[2048];
		xData d;
		d << xData(buf,2048,"ReceiveDateTime");
		d << xData(buf,2048,"DateTime");
		d << xData(buf2,2048,"Lattitude");
		d << xData(buf2,2048,"Longitude");
		d << xData(buf3,2028,"Speed");
		d << xData(buf2,2048,"Voltage");

        xData rr = d.GetObjectDescriptor(false);
        char* cc = rr;

		__int8 b[8192*20];
		unsigned __int32 usedsize = d.GetLinearObject(b,sizeof(b));

		xData *d3 = (xData *)b;

        xData r = d3->GetObjectDescriptor(false);
        char* c = r;

	}
//[DSC] DT_UNDEFINE, size=20
//   [type] DT_STRING, ansi, size=16, value=set_of_position
//   DT_UNDEFINE, size=12
//      [ReceiveDateTime] DT_DOUBLE, size=1, value=39346.09331018518
//      [DateTime] DT_DOUBLE, size=1, value=39345.80061342592
//      [Latitude] DT_DOUBLE, size=1, value=56.47785568237304
//      [Longitude] DT_DOUBLE, size=1, value=85.0116958618164
//      [Speed] DT_INT32, size=1, value=0
//      [Voltage] DT_DOUBLE, size=1, value=11.89999961853027
//      [Analogue] DT_DOUBLE, size=1, value=0
//      [Ignition] DT_INT32, size=1, value=1
//      [Protection] DT_INT32, size=1, value=0
//      [Alarm] DT_INT32, size=1, value=0
//      [Lock] DT_INT32, size=1, value=0
//      [Status] DT_INT32, size=1, value=3
//   DT_UNDEFINE, size=12
//      [ReceiveDateTime] DT_DOUBLE, size=1, value=39346.09331018518
//      [DateTime] DT_DOUBLE, size=1, value=39345.80061342592
//      [Latitude] DT_DOUBLE, size=1, value=56.47785568237304
//      [Longitude] DT_DOUBLE, size=1, value=85.0116958618164
//      [Speed] DT_INT32, size=1, value=0
//      [Voltage] DT_DOUBLE, size=1, value=11.89999961853027
//      [Analogue] DT_DOUBLE, size=1, value=0
//      [Ignition] DT_INT32, size=1, value=1
//      [Protection] DT_INT32, size=1, value=0
//      [Alarm] DT_INT32, size=1, value=0
//      [Lock] DT_INT32, size=1, value=0
//      [Status] DT_INT32, size=1, value=3

//	{
//		// Тест для неименованного объекта с числом
//		__int8 buf[1024]={0};
//		xData d1(double(123));
//		const xData *d2 = d1.GetLinearObject();
//		if (d1.GetLinearSize() != sizeof(xData)+sizeof(double)) goto ERR;
//		int a = *d2;
//		if (a!=123) goto ERR;
//		d2->free();
//	}
//	{
//		// Тест для именованного объекта в режиме указателя
//		__int8 buf[1024]={0};
//		int v=123;
//		xpData d1(v,"name");
//		const xData *d2 = d1.GetLinearObject();
//		if (d1.GetLinearSize() != sizeof(xData)+sizeof(xDataExtent)-3+strlen("name")) goto ERR;
//		int a = *d2;
//		if (a!=123 || strcmp(d1.GetName(), d2->GetName())!=0) goto ERR;
//		d2->free();
//	}
//	{
//		// Тест для простого контейнера
//		__int8 buf[1024]={0};
//		__int16 ar[100];
//		for (int i=0;i<100;i++) ar[i] = (__int16)i;
//		xData d1(ar,100,"array");
//		const xData *d2 = d1.GetLinearObject();
//		if (d1.GetLinearSize() != sizeof(xData)+sizeof(xDataExtent)-3+strlen("array")+sizeof(ar)) goto ERR;
//		if (memcmp(d1.GetThisDataPointer(),d2->GetThisDataPointer(),sizeof(ar))!=0) goto ERR;
//		d2->free();
//	}
//	// Тест комплексного контейнера
//	{
//		__int8 buf[1024]={0};
//		xData d1;
//		unsigned __int64 v=1;
//		d1 << xData("abc","name");
//		d1 << 2;
//		d1 << xpData(v);
//
//		char* v11 = (char*)(d1[0].GetReferenceDataPointer());
//		const xData *d2 = d1.GetLinearObject();
//
//		char* v1 = (char*)((*d2)[0].GetReferenceDataPointer());
//
//		int* v2 = (int*)((*d2)[1].GetReferenceDataPointer());
//		unsigned __int64* v3=(unsigned __int64*)((*d2)[2].GetReferenceDataPointer());
//		char* v4 = (*d2)[0].GetName();
//
//		if (strcmp(v1,(char*)d1[0].GetReferenceDataPointer())!=0) goto ERR;
//		if (*v2!=2 || *v3!=1) goto ERR;
//		if (!v4 || strcmp(v4,d1[0].GetName())!=0) goto ERR;
//	}
//
	return true;
ERR:
	printf("StoringTest не пройден");
	return false;
}

bool xDataTest::CompareTest()
{
    {
	    float v=1.0f;
	    xData d1(1);
	    xpData d2(v);
	    if (d1!=d2) goto ERR;
        if (xpData("3.14")!=3.14) goto ERR;
        if (xpData("text","name")!=L"text") goto ERR;
    }
    {
        xData d1;
        d1 << xData("text","name");
        const xData &d2 = d1;
        if (d2["name"]!="text") goto ERR;
    }
    {
        xData d1;
        d1 << xData("127.0.0.1","IP");
        d1 << xData("2100","PORT");
        xData d2;
        d2 << xData("2100","PORT");
        d2 << xData("127.0.0.1","IP");
        if (d1!=d2) goto ERR;
    }
//	if (d1!=1) goto ERR;
//	if (1.0f!=d2) goto ERR;
//	if (d2 < 0.5) goto ERR;
//	if (d2 <= 0.5) goto ERR;
//	if (d2 > "2") goto ERR;
//	if (d2 >= "2") goto ERR;
//
//float a =  (float)_wtof(L"3.14");
//
//	if (L"3.14"!=xData(3.14)) goto ERR;
//	if (L"3.14">xData(3.14)) goto ERR;
//
	return true;
ERR:
	printf("CompareTest не пройден");
	return false;
}

bool xDataTest::ObjectDescriptionTest()
{
	{
		double v = 3.1415926;
		xpData d(v);
		xData r = d.GetObjectDescriptor(false);
		char* c = r;
		char Res[] = "Pointer(DT_DOUBLE), size=1, value=3.1415926";
		if (strcmp(c,Res)) goto ERR;
	}
	{
		signed __int32 v = -613;
		xpData d(v, "имя");
		xData r = d.GetObjectDescriptor(false);
		char* c = r;
		char Res[] = "[имя] pointer(DT_INT32), size=1, value=-613";
		if (strcmp(c,Res)) goto ERR;
	}
	{
		xcData d;
		d << 1;
		d << 2;
		d << 3;
		d.SetName("имя");
		xData r = d.GetObjectDescriptor(false);
		char* c = r;
		char Res[] = "[имя] DT_INT32, CS, size=3\n   1, 2, 3";
		if (strcmp(c,Res)) goto ERR;
	}
	// Указатель на другой объект
	{
		xData d1(1);
		xpData d2(&d1);
		xData r = d2.GetObjectDescriptor(false);
		char* c = r;
		char Res[] = "Pointer(DT_UNDEFINE), size=1\n   DT_INT32, size=1, value=1";
		if (strcmp(c,Res)) goto ERR;
	}
	{
		float scale = 5.23f;
		xcData d1;
		d1 << 1;
		d1 << xpData(scale,"scale");
		d1.SetName("one");

		xData d2;
		d2 << -666.666;
		d2 << -33;
		d2 << "text";

		d2 << xpData(&d1);

		scale = 4;

		xData r = d2.GetObjectDescriptor(false);
		char* c = r;
		char Res[] = "DT_UNDEFINE, size=4\n   DT_DOUBLE, size=1, value=-666.666\n   DT_INT32, size=1, value=-33\n"
"   DT_STRING, ansi, size=5, value=text\n   Pointer(DT_UNDEFINE), size=1\n      [one] DT_UNDEFINE, CS, size=2\n"
"         DT_INT32, size=1, value=1\n         [scale] pointer(DT_FLOAT), size=1, value=4";
		if (strcmp(c,Res)) goto ERR;

		float a = d2["one"]["scale"];

		if (a!=4) goto ERR;
	}

	return true;
ERR:
	printf("ObjectDescriptionTest не пройден");
	return false;
}

//bool xDataTest::StoreAndRestoreTest()
//{
//	// Тест указателя на объект
//	{
//		int v1=1;
//		double v2=2;
//		char v3[5]="abcd";
//		xData v4(10,"zz");
//		xData d1;
//		d1 << xpData(v1,"v1");
//		d1 << xpData(v2,"v2");
//		d1 << xpData(v3,"v3");
//		d1 << xpData(v4);
//
//		__int8 buf[1024]={0};
//		const xData *d2 = d1.GetLinearObject();
//
//		d1[0]=3;
//		d1[1]=4;
//		d1[2]="ef";
//
//		xData r1 = d1.GetObjectDescriptor(false);
//		char* c1 = r1;
//		xData r2 = d2->GetObjectDescriptor(false);
//		char* c2 = r2;
//
//		d1 = d2;
//
//		d2->free();
//
//	}
//	return true;
//ERR:
//	printf("StoreAndRestoreTest не пройден");
//	return false;
//}
//
bool xDataTest::ContainerTest5()
{
	xData d;
	int b[3]={1,2,3};
	d.Push(b,3);		// Добавляется три элемента
	d.Push("127");
	d.Push(xData(2.71878878,"e"));
	d<<xData(b,3);			// Добавляется контейнер из 3-х элементов
	d.Push(b,3);		// Добавляется три элемента
	d.Push(xData("3.14","Pi"));
	d.Push(xpData(b,3));	// Добавляется указатель на массив из 3-х элементов
	d<<xData(d);			// Добавляется копия объекта

	xData r = d.GetObjectDescriptor(false);
	char* c = r;

	char Res[]="DT_UNDEFINE, size=12\n   DT_INT32, size=1, value=1\n   DT_INT32, size=1, value=2\n   DT_INT32, size=1, value=3\n   DT_STRING, ansi, size=4, value=127\n"
"   [e] DT_DOUBLE, size=1, value=2.71878878\n   DT_INT32, size=3\n      1, 2, 3\n   DT_INT32, size=1, value=1\n   DT_INT32, size=1, value=2\n   DT_INT32, size=1, value=3\n"
"   [Pi] DT_STRING, ansi, size=5, value=3.14\n   Pointer(DT_INT32), size=3\n      1, 2, 3\n   DT_UNDEFINE, size=11\n      DT_INT32, size=1, value=1\n"
"      DT_INT32, size=1, value=2\n      DT_INT32, size=1, value=3\n      DT_STRING, ansi, size=4, value=127\n      [e] DT_DOUBLE, size=1, value=2.71878878\n"
"      DT_INT32, size=3\n         1, 2, 3\n      DT_INT32, size=1, value=1\n      DT_INT32, size=1, value=2\n      DT_INT32, size=1, value=3\n"
"      [Pi] DT_STRING, ansi, size=5, value=3.14\n      DT_INT32, size=3\n         1, 2, 3";
		
		
	if (strcmp(c,Res)) goto ERR;

	double Pi = d[11]["Pi"];
	if (Pi!=3.14) goto ERR;

	return true;
ERR:
	printf("ContainerTest5 не пройден");
	return false;
}

//bool xDataTest::SyncronizeTest()
//{
//	// Тест синхронизации простого объекта
//	{
//		__int8 i=1;
//		xpData d1(i,"data");
//		const xData *d2 = d1.GetLinearObject();
//
//		i=0;
//
//		xData r1 = d1.GetObjectDescriptor(false);
//		char* c1 = r1;
//		xData r2 = d2->GetObjectDescriptor(false);
//		char* c2 = r2;
//
//		d1 = d2;
//
//		xData r3 = d1.GetObjectDescriptor(false);
//		char* c3 = r3;
//		xData r4 = d2->GetObjectDescriptor(false);
//		char* c4 = r4;
//
//		if (i!=1) goto ERR;
//		if (!d1.GetName()) goto ERR;
//	}
//	// Тест синхронизации простого контейнера
//	{
//		__int8 buf[100];
//		for (int i=0;i<sizeof(buf);i++) buf[i]=i;
//		xpData d1(buf, sizeof(buf), "data");
//		const xData *d2 = d1.GetLinearObject();
//		for (int i=0;i<sizeof(buf);i++) buf[i]=0;
//
//		xData r1 = d1.GetObjectDescriptor(false);
//		char* c1 = r1;
//		xData r2 = d2->GetObjectDescriptor(false);
//		char* c2 = r2;
//
//
//		d1 = d2;
//
//		for (int i=0;i<sizeof(buf);i++) 
//			if (buf[i]!=i) goto ERR;
//
//		xData r3 = d1.GetObjectDescriptor(false);
//		char* c3 = r3;
//		xData r4 = d2->GetObjectDescriptor(false);
//		char* c4 = r4;
//	}
//	// Тест синхронизации комплексного контейнера
//	{
//		__int8 buf[100];
//		for (int i=0;i<sizeof(buf);i++) buf[i]=i;
//		xData d1;
//		d1.SetName("param");
//		d1 << xpData(buf,100,"data");
//		d1 << xData(1,"randomvalue");
//		const xData *d2 = d1.GetLinearObject();
//		for (int i=0;i<sizeof(buf);i++) buf[i]=0;
//		d1["randomvalue"] = 2;
//
//		xData r1 = d1.GetObjectDescriptor(false);
//		char* c1 = r1;
//		xData r2 = d2->GetObjectDescriptor(false);
//		char* c2 = r2;
//
//		d1 = d2;
//
//		if (int(d1["randomvalue"])!=1) goto ERR;
//
//		xData r3 = d1.GetObjectDescriptor(false);
//		char* c3 = r3;
//	}
//	// Тест синхронизации комплексного контейнера с несовпадающими объектами
//	{
//		xData d1;
//		d1.SetName("abc");
//		int v1=123;
//		d1 << xData(456,"param2");
//		d1 << xpData(v1,"param1");
//
//		xData d2;
//		d2.SetName("abc");
//		d2 << xData(111,"param1");
//		d2 << xData(222,"param2");
//
//		d1 = &d2;
//
//		xData r3 = d1.GetObjectDescriptor(false);
//		char* c3 = r3;
//		xData r4 = d2.GetObjectDescriptor(false);
//		char* c4 = r4;
//
//		if (v1!=111) goto ERR;
//		if (int(d1["param2"])!=222) goto ERR;
//
//	}
//	return true;
//ERR:
//	printf("SyncronizeTest не пройден");
//	return false;
//}
//
//// Тест оператора присваивания
bool xDataTest::EqualTest()
{
//
//	// Тест присваивания объекта по таблице
//	int v1[3]={1,2,3};
//	int v2=1;
//	xData ds1;
//	xData ds2(1);
//	xData ds3(v1,3);
//	xData ds4; ds4 << ds1 << ds2 << ds3;
//	xpData ds5;
//	xpData ds6(v2);
//	xpData ds7(v1,3);
//
//	xData z1=ds1.GetObjectDescriptor(false); char *c1=z1;
//	xData z2=ds2.GetObjectDescriptor(false); char *c2=z2;
//	xData z3=ds3.GetObjectDescriptor(false); char *c3=z3;
//	xData z4=ds4.GetObjectDescriptor(false); char *c4=z4;
//	xData z5=ds5.GetObjectDescriptor(false); char *c5=z5;
//	xData z6=ds6.GetObjectDescriptor(false); char *c6=z6;
//	xData z7=ds7.GetObjectDescriptor(false); char *c7=z7;
//
//	int v3[3]={4,5,6};
//	int v4=2;
//	xData da1(3);
//	xData da2(v3,3);
//	xData da3; da3 << da1 << da2;
//	xpData da4;
//	xpData da5(v4);
//	xpData da6(v3,3);
//	xpData da7(&da1);
//
//	xData u1=da1.GetObjectDescriptor(false); char *x1=u1;
//	xData u2=da2.GetObjectDescriptor(false); char *x2=u2;
//	xData u3=da3.GetObjectDescriptor(false); char *x3=u3;
//	xData u4=da4.GetObjectDescriptor(false); char *x4=u4;
//	xData u5=da5.GetObjectDescriptor(false); char *x5=u5;
//	xData u6=da6.GetObjectDescriptor(false); char *x6=u6;
//	xData u7=da7.GetObjectDescriptor(false); char *x7=u7;
//
	{	// Тест присваивания простого объекта с данными
		int v1[3]={1,2,3};
		int v2=1;
		xData ds1;
		xData ds2(1);
		xData ds3(v1,3);
		xData ds4; ds4 << ds1 << ds2 << ds3;
		xpData ds5=1;
		xpData ds6(v2);
		xpData ds7(v1,3);

		xData da(3);

		ds1 = da;
		ds2 = da;
		ds3 = da;
		ds4 = da;
		ds5 = da;
		ds6 = da;
		ds7 = da;

		xData z1=ds1.GetObjectDescriptor(false); char *c1=z1; if (strcmp(c1,"DT_INT32, size=1, value=3")!=0) goto ERR;
		xData z2=ds2.GetObjectDescriptor(false); char *c2=z2; if (strcmp(c2,"DT_INT32, size=1, value=3")!=0) goto ERR;
		xData z3=ds3.GetObjectDescriptor(false); char *c3=z3; if (strcmp(c3,"DT_INT32, size=1, value=3")!=0) goto ERR;
		xData z4=ds4.GetObjectDescriptor(false); char *c4=z4; if (strcmp(c4,"DT_INT32, size=1, value=3")!=0) goto ERR;
		xData z5=ds5.GetObjectDescriptor(false); char *c5=z5; if (strcmp(c5,"DT_INT32, size=1, value=3")!=0) goto ERR;
		xData z6=ds6.GetObjectDescriptor(false); char *c6=z6; if (strcmp(c6,"Pointer(DT_INT32), size=1, value=3")!=0) goto ERR;
		xData z7=ds7.GetObjectDescriptor(false); char *c7=z7; 
		if (strcmp(c7,"Pointer(DT_INT32), size=3\n   1, 2, 3")!=0) goto ERR;
	}
	{	// Тест присваивания контейнера простого типа
		int v1[3]={1,2,3};
		int v2=1;
		xData ds1;
		xData ds2(1);
		xData ds3(v1,3);
		xData ds4; ds4 << ds1 << ds2 << ds3;
		xpData ds5;
		xpData ds6(v2);
		xpData ds7(v1,3);

		int v3[3]={4,5,6};
		xData da(v3,3);

		ds1 = da;
		ds2 = da;
		ds3 = da;
		ds4 = da;
		ds5 = da;
		ds6 = da;
		ds7 = da;

		xData z1=ds1.GetObjectDescriptor(false); char *c1=z1; 
        if (strcmp(c1,"DT_INT32, size=3\n   4, 5, 6")!=0) goto ERR;
		xData z2=ds2.GetObjectDescriptor(false); char *c2=z2; 
        if (strcmp(c1,"DT_INT32, size=3\n   4, 5, 6")!=0) goto ERR;
		xData z3=ds3.GetObjectDescriptor(false); char *c3=z3; 
        if (strcmp(c1,"DT_INT32, size=3\n   4, 5, 6")!=0) goto ERR;
		xData z4=ds4.GetObjectDescriptor(false); char *c4=z4; 
        if (strcmp(c1,"DT_INT32, size=3\n   4, 5, 6")!=0) goto ERR;
		xData z5=ds5.GetObjectDescriptor(false); char *c5=z5; 
        if (strcmp(c1,"DT_INT32, size=3\n   4, 5, 6")!=0) goto ERR;
		xData z6=ds6.GetObjectDescriptor(false); char *c6=z6; 
        if (strcmp(c6,"Pointer(DT_INT32), size=1, value=4")!=0) goto ERR;
		xData z7=ds7.GetObjectDescriptor(false); char *c7=z7; 
        if (strcmp(c7,"Pointer(DT_INT32), size=3\n   1, 2, 3")!=0) goto ERR;
	}

	{	// Тест присваивания контейнера объектов
		int v1[3]={1,2,3};
		int v2=1;
		xData ds1;
		xData ds2(1);
		xData ds3(v1,3);
		xData ds4; ds4 << ds1 << ds2 << ds3;
		xpData ds5;
		xpData ds6(v2);
		xpData ds7(v1,3);

		int v3[3]={4,5,6};
		int v4=2;
		xData da1(3);
		xpData da2(v3,3);
		xData da; da << da1 << da2;

		ds1 = da;
		ds2 = da;
		ds3 = da;
		ds4 = da;
		ds5 = da;
		ds6 = da;
		ds7 = da;

		xData z1=ds1.GetObjectDescriptor(false); char *c1=z1; 
		if (strcmp(c1,"DT_UNDEFINE, size=2\n   DT_INT32, size=1, value=3\n   DT_INT32, size=3\n      4, 5, 6")!=0) goto ERR;
		xData z2=ds2.GetObjectDescriptor(false); char *c2=z2; 
		if (strcmp(c2,"DT_UNDEFINE, size=2\n   DT_INT32, size=1, value=3\n   DT_INT32, size=3\n      4, 5, 6")!=0) goto ERR;
		xData z3=ds3.GetObjectDescriptor(false); char *c3=z3;
		if (strcmp(c3,"DT_UNDEFINE, size=2\n   DT_INT32, size=1, value=3\n   DT_INT32, size=3\n      4, 5, 6")!=0) goto ERR;
		xData z4=ds4.GetObjectDescriptor(false); char *c4=z4;
		if (strcmp(c4,"DT_UNDEFINE, size=2\n   DT_INT32, size=1, value=3\n   DT_INT32, size=3\n      4, 5, 6")!=0) goto ERR;
		xData z5=ds5.GetObjectDescriptor(false); char *c5=z5;
		if (strcmp(c5,"DT_UNDEFINE, size=2\n   DT_INT32, size=1, value=3\n   Pointer(DT_INT32), size=3\n      4, 5, 6")!=0) goto ERR;
		xData z6=ds6.GetObjectDescriptor(false); char *c6=z6; 
		if (strcmp(c6,"Pointer(DT_INT32), size=1, value=0")!=0) goto ERR;
		xData z7=ds7.GetObjectDescriptor(false); char *c7=z7; 
		if (strcmp(c7,"Pointer(DT_INT32), size=3\n   1, 2, 3")!=0) goto ERR;
	}

	// Тест присваивания простому объекту. Аргументом является простой объект
	{
		xData d1, d2(1,"name");
		d1 = d2;
		if (d1.IsPointer()) goto ERR;
		if (int(d1)!=1) goto ERR;
		if (!d1.CompareWithName("name")) goto ERR;
	}

	// Тест присваивания простому объекту. Аргументом является указатель
	{
		xpData d1;
		xData d2;
		float a = 3;
		d2 << 1;
		d2 << xpData(a, "name");
		d1 = d2;
		if (d1.IsPointer()) goto ERR;

		xData r1 = d1.GetObjectDescriptor(false);
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor(false);
		char* c2 = r2;

		if (!d1[1].CompareWithName("name")) goto ERR;
	}

	// Тест присваивания контейнера объектов, один из которых тоже контейнер
	{
		int p=3;
		xData d1,d2;
		d1 << xpData(p);
		d1 << 1;
		d2 << d1;
		d2 << 2;
		
		xData r1 = d1.GetObjectDescriptor(false);
		char* c1 = r1;

		xData r2 = d2.GetObjectDescriptor(false);
		char* c2 = r2;

		xpData d3(&d2);
		xData r3 = d3.GetObjectDescriptor(false);
		char* c3 = r3;
		if (strcmp(c3,"Pointer(DT_UNDEFINE), size=1\n   DT_UNDEFINE, size=2\n      DT_UNDEFINE, size=2\n         Pointer(DT_INT32), size=1, value=3\n"
                      "         DT_INT32, size=1, value=1\n      DT_INT32, size=1, value=2")!=0) goto ERR;
	}

	return true;
ERR:
	printf("EqualTest не пройден");
	return false;
}
//
//xData Func1(xData V)
//{
//	return V;
//}
//
//xData Func2(xpData V)
//{
//	return V;
//}
//
//bool xDataTest::ConstructorsTest()
//{
//xData r;
//char* c;
//
//int v=1;
//xData d1(v);
//xData d2(d1);
//v=2;
////d2=d1;
//
//		xData r2 = d2.GetObjectDescriptor(false);
//		char* c2 = r2;
//		v=3;
//
//		{
//		xData D(2);
//		xData d1(Func1(D));
//		r = d1.GetObjectDescriptor(false);c = r;
////		xpData d2(Func1(D));	// Нельзя использовать xpData для объектов, срок жизни которых заканчивается после выполнения операции
////		r = d2.GetObjectDescriptor(false);c = r;
//		xData d3(Func2(D));
//		r = d3.GetObjectDescriptor(false);c = r;
////		xpData d4(Func2(D));	// Нельзя использовать xpData для объектов, срок жизни которых заканчивается после выполнения операции
////		r = d4.GetObjectDescriptor(false);c = r;
//	}
//	{
//		int a=13;
//		xpData D(a);
//		xData d1(Func2(D));
//		r = d1.GetObjectDescriptor(false);c = r;
////		xpData d2(Func2(D));	// Нельзя использовать xpData для объектов, срок жизни которых заканчивается после выполнения операции
////		r = d2.GetObjectDescriptor(false);c = r;
//		xData d3(Func2(D));
//		r = d3.GetObjectDescriptor(false);c = r;
////		xpData d4(Func2(D));	// Нельзя использовать xpData для объектов, срок жизни которых заканчивается после выполнения операции
////		d4.Invalidate();
////		Func2(D);
////		r = d4.GetObjectDescriptor(false);c = r;
//	}
//
//	return true;
//ERR:
//	printf("ConstructorsTest");
//	return false;
//}
//

bool xDataTest::XmlTest()
{
    {
        xData Data,Error;
        ParseXmlFile(".\\test.xml",Data,Error);
    }

    {
        xmlData d("<Bauds:UI16=\" 10 20 30 \"/>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[Bauds] DT_UINT16, size=3\n   10, 20, 30";
        if (strcmp(c,z)) goto ERR;
    }

    {
        xmlData d("<Bauds:I8=\"h 10 20 30 \"/>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[Bauds] DT_INT8, size=3\n   16, 32, 48";
        if (strcmp(c,z)) goto ERR;
    }

    {
        xmlData d("<Bauds:I8>h 10 20 30</Bauds>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[Bauds] DT_INT8, size=3\n   16, 32, 48";
        if (strcmp(c,z)) goto ERR;
    }

    {
        xmlData d("< :I8>h FF 20 30  </>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="DT_INT8, size=3\n   -1, 32, 48";
        if (strcmp(c,z)) goto ERR;
    }
    {
        xmlData d("<Settings><Record Codec=\"mp3\" Channels=\"1\" Bits=\"16\" Samples=\"22050\" Bitrate=\"32000\" DefaultFolder=\"true\" Folder=\"\\Storage Card\\Music\" DateFolder=\"true\" "
            "Prefix=\"rec\" TurnOff=\"0\" VASOn=\"true\" InputOn=\"true\" Vas=\"0.500000\" Input=\"0.500000\"/>"
            "<Play Mode=\"PlaylistOnce\" TurnOff=\"0\" AssociateMP3Cmd=\"&quot;\\Storage Card"
            "\\Program files\\VitoSound\\SoundExplorer.exe&quot; %1\" AssociateWAVCmd=\"&quot;\\Storage Card\\Program files\\VitoSound\\SoundExplorer.exe&quot; %1\" AssociateM3UCmd=\"&quot;\\"
            "Storage Card\\Program files\\VitoSound\\SoundExplorer.exe&quot; %1\" AssociateOGGCmd=\"&quot;\\Storage Card\\Program files\\VitoSound\\SoundExplorer.exe&quot; %1\" AssociateWMACmd"
            "=\"&quot;\\Storage Card\\Program files\\VitoSound\\SoundExplorer.exe&quot; %1\" AssociateMP3Icon=\"\\Storage Card\\Program files\\VitoSound\\SoundExplorer.exe,-0\" AssociateWAVIcon"
            "=\"\\Storage Card\\Program files\\VitoSound\\SoundExplorer.exe,-0\" AssociateM3UIcon=\"\\Storage Card\\Program files\\VitoSound\\SoundExplorer.exe,-0\" AssociateOGGIcon=\"\\Storage"
            "Card\\Program files\\VitoSound\\SoundExplorer.exe,-0\" AssociateWMAIcon=\"\\Storage Card\\Program files\\VitoSound\\SoundExplorer.exe,-0\" Booster=\"0\" Noise=\"0\" Limiter=\"0\" "
            "Speed=\"1.000000\" PitchLock=\"true\"/><Language Language=\"Russian\"/><Buttons Button0=\"RecordStop\" Button1=\"\" Button2=\"Record\" Button3=\"\" Button4=\"\" Button5=\"\" "
            "UnmapWhenInactive=\"true\" JoyLeftAction=\"Rewind\" JoyRightAction=\"Forward\"/><FileExplorer ShowFolders=\"false\" Folder=\"\\Storage Card\\Music\\OLD\" Sort=\"NAME\"/><Skin "
            "Path=\"%app_path%\\Classic.skin\" PathLandscape=\"%app_path%\\Blue.skin\"/><Equalizer1 Frequency=\"131.000000\" Amplitude=\"4.561404\" Width=\"2.000000\" Active=\"true\"/>"
            "<Equalizer2 Frequency=\"1675.000000\" Amplitude=\"5.964912\" Width=\"2.000000\" Active=\"true\"/><Equalizer3 Frequency=\"5835.000000\" Amplitude=\"9.473684\" Width=\"2.000000\""
            "Active=\"true\"/><Schedule/></Settings>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        if (d.DataSize!=10) goto ERR;
    }
    {
        xmlData d("<name value:D=3.1415926> </name>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[name] DT_UNDEFINE, size=1\n   [value] DT_DOUBLE, size=1, value=3.1415926";
        if (strcmp(c,z)) goto ERR;
    }
    {
        xmlData d("<name:D> 3.1415926 </name>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[name] DT_DOUBLE, size=1, value=3.1415926";
        if (strcmp(c,z)) goto ERR;
    }
    {
        xmlData d("<name> какой-то текст </name>");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[name] DT_STRING, ansi, size=17, value= какой-то текст ";
        if (strcmp(c,z)) goto ERR;
    }
    {
        xmlData d("<name pi=3.14> <in as=11 sa=12/> </\"name\">");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[name] DT_UNDEFINE, size=2\n   [pi] DT_STRING, ansi, size=5, value=3.14\n   [in] DT_UNDEFINE, size=2\n"
            "      [as] DT_STRING, ansi, size=3, value=11\n      [sa] DT_STRING, ansi, size=3, value=12";
        if (strcmp(c,z)) goto ERR;
    }
    {
        xmlData d("<name pi:D=3.14 t=text></\"name\">");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[name] DT_UNDEFINE, size=2\n   [pi] DT_DOUBLE, size=1, value=3.14\n   [t] DT_STRING, ansi, size=5, value=text";
        if (strcmp(c,z)) goto ERR;
    }
    {
        xmlData d("<\"long name\" t1:F=\" 003.14\" t2=\"Пример длинного текста\" />");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[long name] DT_UNDEFINE, size=2\n   [t1] DT_FLOAT, size=1, value=3.140000104904174\n"
        "   [t2] DT_STRING, ansi, size=23, value=Пример длинного текста";
        if (strcmp(c,z)) goto ERR;
    }
    {
        xmlData d("<name t1:I16=3 t2:UI32=5 />");
        xData r = d.GetObjectDescriptor(false);
        char *c = r;
        char z[]="[name] DT_UNDEFINE, size=2\n   [t1] DT_INT16, size=1, value=3\n   [t2] DT_UINT32, size=1, value=5";
        if (strcmp(c,z)) goto ERR;
    }

    return true;
ERR:
	printf("XmlTest");
	return false;
}