class A
{public:
void init();
void f();
};

class R
{
public:
	void* Owner;
	inline R* SetOwner(void* owner){Owner=owner;return this;};
	virtual void OnFire(){};
};

void A::f()
{
	int a=1;
}

void A::init()
	{
		char a[1024];
		strcpy(a,__FUNCSIG__);
		char b[1024];
		strcpy(b,__FUNCDNAME__);
		char c[1024];
		strcpy(c,__FUNCTION__);



		class AA : 
			 public R {
		public:
				 void OnFire()
				 {
					 return ((A*)Owner)->f();
				 };
			} aa; 
		aa.SetOwner(this);
		aa.OnFire();

};



bool ReflectorTest()
{

	A a;
	a.init();

	return true;
ERR:
	printf("ReflectorTest не пройден");
	return false;
}

bool InitTest()
{
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
	// Тест инициализации константы
	{
		xData d1(1);
		xpData d2(1);	// Должно произойти копирование данных, как в предыдущей строке
		if (int(d1)!=1) goto ERR;
		if (int(d2)!=1) goto ERR;
	}
	// Тест инициализации переменной
	{
		float p=1.0;
		xData d1(p);
		xpData d2(p);
		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor();
		char* c2 = r2;
		if (d1.IsPointer()) goto ERR;
		if (!d2.IsPointer()) goto ERR;
	}
	// Тест инициализации простого объекта
	{
		xData d1(3);
		xData d2(d1);
		xpData d3(d1);
		xData r1 = d2.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d3.GetObjectDescriptor();
		char* c2 = r2;
		if (d2.IsPointer() || d2!=3) goto ERR;
		if (!d3.IsPointer() || d3!=3) goto ERR;
	}
	{
		xData d1;
		float p=3.14;
		float e=2.81;
		d1 << xpData(p,"pi");
		d1 << xpData(e,"e");
		xData d2(d1);
		xpData d3(d1);

		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor();
		char* c2 = r2;
		xData r3 = d3.GetObjectDescriptor();
		char* c3 = r3;
	}

	// Тест инициализации объекта-указателя
	{
		float p=3.14;
		xpData d1(p);
		xData d2(d1);
		xpData d3(d1);
		xData r1 = d2.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d3.GetObjectDescriptor();
		char* c2 = r2;
		if (d2.IsPointer() || d3!=3.14) goto ERR;
		if (!d3.IsPointer() || d3!=3.14) goto ERR;
	}
	// Тест инициализации простого объекта-контейнера
	{
		float p=3.14;
		xpData d1(p);
		xData d2(d1);
		xpData d3(d1);
		if (d2.IsPointer() || d3!=3.14) goto ERR;
		if (!d3.IsPointer() || d3!=3.14) goto ERR;
	}

	return true;
ERR:
	printf("InitTest не пройден");
	return false;
}

// Тест исключения DE_PointerIsConst

bool ExceptionTest1()
{
	try{xpeData d1(100);goto ERR;}
	catch (xDataException* e){	
		if (e->GetNumber()!=DE_PointerIsConst) 
		{//delete e;
		goto ERR;}	
//		delete e;	
	}
	return true;
ERR:
	printf("ExceptionTest1 не пройден");
	return false;
}

// Тест простого контейнера
bool ContainerTest1()
{
	int Buf[10]={1,2,3,4,5,6,7,8,9,10};

	xData d1;
	d1.Push(Buf,10);

	double v1 = d1[0];
	d1[0]=10;
	double v2 = d1[0];
	d1[9]=1;
	double v3 = d1[9];
	if (v1!=1 || v2!=10 || v3!=1)
	{
		printf("ContainerTest1 не пройден");
		return false;
	}
	return true;
}

// Тест комплексного контейнера
bool ContainerTest2()
{
	{
		xData d1;
		d1 << double(3);
		d1 << unsigned __int8(7);
		int a = d1[1];
		if (a!=7) goto ERR;
	}
	{
		xData d1;
		d1 << 3;
		if (!d1.IsContainer()) goto ERR;	// Объект должен быть контейнером
	}
	{
		xData d1;
		d1 << 3;
		d1 << xData(4);
		// Объект должен быть комплексным контейнером
		if (!d1.IsContainer() || d1.GetThisDataType()!=DT_UNDEFINE) goto ERR;	
	}
	{
		xData d1;
		d1 << 3;
		d1 << float(4);
		// Объект должен быть контейнером комплексного типа 
		if (!d1.IsContainer() || d1.GetThisDataType()!=DT_UNDEFINE) goto ERR;	
	}
	{
		xData d1;
		d1 << xcData(3,"name");
		// Объект должен стать комплексным контейнером из одного элемента
		if (!d1.IsContainer() || d1.GetThisDataType()!=DT_UNDEFINE) goto ERR;	
		if (!d1[0].GetName() || strcmp(d1[0].GetName(),"name")!=0) goto ERR;
		d1 << 4;
		// Объект должен стать комплексным контейнером и неименованным
		if (!d1.IsContainer() || d1.GetThisDataType()!=DT_UNDEFINE) goto ERR;	
		if (d1.GetName()!=0) goto ERR;
	}
	{
		xData d1;
		d1 << 3;
		d1 << xData(4,"name");
		// Объект должен стать контейнером комплексного типа
		// (так как был добавлен именованый объект)
		if (!d1.IsContainer() || d1.GetThisDataType()!=DT_UNDEFINE) goto ERR;	
	}

	return true;
ERR:
	printf("ContainerTest2 не пройден");
	return false;
}

// Тест объекта-указателя. 
bool PointerTest1()
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

bool StringTest1()
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
		char* t = (char*)d.GetReferenceDataPointer();
		for (int i=0;i<10;i++)
			d[i] = i+65;
		if (memcmp(t,"ABCDEFGHIJ",10)!=0) goto ERR;
	}
	// Тест адресации символов в UNICODE строке
	{
		xData d(L"0123456789");
		wchar_t* t = (wchar_t*)d.GetReferenceDataPointer();
		for (int i=0;i<10;i++)
			d[i] = L'A'+i;
		if (memcmp(t,L"ABCDEFGHIJ",20)!=0) goto ERR;
	}
	return true;
ERR:
	printf("StringTest1");
	return false;
}

// Тест контейнера указателей. 
bool ContainerTest3()
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

bool ContainerTest4()
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
		xData d2;
		xpData d1;
		int v1=1;
		float v2=2;
		d1 << xpData(v1);
		d1 << xpData(v2);
		d2 = d1;

		v1=3;v2=4;

		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor();
		char* c2 = r2;

		if (int(d2[0])!=3) goto ERR;
		if (int(d1[0])!=3) goto ERR;


	}


	return true;
ERR:
	printf("ContainerTest4 не пройден");
	return false;
}

bool NameTest1()
{
	{
		xData d1(1);				// Не именованный
		xData d2(2,"Именованый");	// Именованный
		d1=d2;	// Должно скопироваться не только содержимое, но и имя
		if (!d1.IsExtended()) goto ERR;
		if (strcmp((char*)d1.GetName(), (char*)d2.GetName())!=0) goto ERR;
		if (int(d1)!=int(d2)) goto ERR;
	}
	{
		xData d1(1,"Тоже именованный");	// Именованный
		xData d2(2,"Именованый");		// Именованный
		d1=d2;	// Должно скопироваться не только содержимое, но и имя
		if (!d1.IsExtended()) goto ERR;
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
		xpData d1(v1);				// Не именованный
		xpData d2(v2,"Именованый");	// Именованный
		d1=d2;	// Должно скопироваться не только содержимое, но и имя
		if (int(d1)!=int(d2)) goto ERR;
		if (!d1.IsExtended()) goto ERR;
		if (strcmp((char*)d1.GetName(), (char*)d2.GetName())!=0) goto ERR;
	}
	{
		// При добавлении именованного объекта должен создаться комплексный контейнер
		xData d1;
		d1 << xData("abc","name");
		if (!d1.IsContainer()) goto ERR;
		if (strcmp(d1[0].GetName(),"name")!=0) goto ERR;
	}

	return true;
ERR:
	printf("NameTest1 не пройден");
	return false;
}

// Тест "спасения" объекта
bool StoringTest()
{
	{
		// Тест для именованного объекта со строкой
		__int8 buf[1024]={0};
		xData d1(L"100", "name");
		const xData *d2 = d1.GetLinearObject();
		if (d1.GetLinearSize() != sizeof(xData)+sizeof(xDataExtent)-3+strlen("name")+8) goto ERR;
		int a = *d2;
		char* p = d2->GetName();
		if (a!=100 || strcmp(d1.GetName(), d2->GetName())!=0) goto ERR;
		d2->free();
	}
	{
		// Тест для неименованного объекта с числом
		__int8 buf[1024]={0};
		xData d1(double(123));
		const xData *d2 = d1.GetLinearObject();
		if (d1.GetLinearSize() != sizeof(xData)+sizeof(double)) goto ERR;
		int a = *d2;
		if (a!=123) goto ERR;
		d2->free();
	}
	{
		// Тест для именованного объекта в режиме указателя
		__int8 buf[1024]={0};
		int v=123;
		xpData d1(v,"name");
		const xData *d2 = d1.GetLinearObject();
		if (d1.GetLinearSize() != sizeof(xData)+sizeof(xDataExtent)-3+strlen("name")) goto ERR;
		int a = *d2;
		if (a!=123 || strcmp(d1.GetName(), d2->GetName())!=0) goto ERR;
		d2->free();
	}
	{
		// Тест для простого контейнера
		__int8 buf[1024]={0};
		__int16 ar[100];
		for (int i=0;i<100;i++) ar[i] = (__int16)i;
		xData d1(ar,100,"array");
		const xData *d2 = d1.GetLinearObject();
		if (d1.GetLinearSize() != sizeof(xData)+sizeof(xDataExtent)-3+strlen("array")+sizeof(ar)) goto ERR;
		if (memcmp(d1.GetThisDataPointer(),d2->GetThisDataPointer(),sizeof(ar))!=0) goto ERR;
		d2->free();
	}
	// Тест указателя на объект
	{
		xData d1(float(3),"name");
		xpData d2(d1);
		d1=4;
		__int8 buf[1024]={0};
		char* z = d1.GetName();
		const xData *d3 = d2.GetLinearObject();

		short v=*d3;
		char* z2 = d3->GetName();
		if (!d3->GetName() || !d1.GetName()) goto ERR;
		if (strcmp(d3->GetName(), d1.GetName())!=0) goto ERR;
		if (v!=4) goto ERR;
		d3->free();
	}
	// Тест комплексного контейнера
	{
		__int8 buf[1024]={0};
		xData d1;
		unsigned __int64 v=1;
		d1 << xData("abc","name");
		d1 << 2;
		d1 << xpData(v);

		char* v11 = (char*)(d1[0].GetReferenceDataPointer());
		const xData *d2 = d1.GetLinearObject();

		char* v1 = (char*)((*d2)[0].GetReferenceDataPointer());

		int* v2 = (int*)((*d2)[1].GetReferenceDataPointer());
		unsigned __int64* v3=(unsigned __int64*)((*d2)[2].GetReferenceDataPointer());
		char* v4 = (*d2)[0].GetName();

		if (strcmp(v1,(char*)d1[0].GetReferenceDataPointer())!=0) goto ERR;
		if (*v2!=2 || *v3!=1) goto ERR;
		if (!v4 || strcmp(v4,d1[0].GetName())!=0) goto ERR;
	}

	return true;
ERR:
	printf("StoringTest не пройден");
	return false;
}

bool CompareTest()
{
	float v=1.0f;
	xpData d1(v);
	xData d2(1);
	if (d1!=d2) goto ERR;
	if (d1!=1) goto ERR;
	if (1.0f!=d2) goto ERR;
	if (d2 < 0.5) goto ERR;
	if (d2 <= 0.5) goto ERR;
	if (d2 > "2") goto ERR;
	if (d2 >= "2") goto ERR;
	if (L"3.14"!=xData(3.14)) goto ERR;
	if (L"3.14">xData(3.14)) goto ERR;

	return true;
ERR:
	printf("CompareTest не пройден");
	return false;
}

bool ObjectDescriptionTest()
{
	{
		double v = 3.1415926;
		xpData d(v);
		xData r = d.GetObjectDescriptor();
		char* c = r;
		char Res[] = "Pointer(DT_DOUBLE) size=0 value=3.1415926";
		if (r!=Res) goto ERR;
	}
	{
		signed __int32 v = -613;
		xpData d(v, "имя");
		xData r = d.GetObjectDescriptor();
		char* c = r;
		char Res[] = "[имя] pointer(DT_INT32) ext size=0 value=-613";
		if (r!=Res) goto ERR;
	}
	{
		xcData d;
		d << 1;
		d << 2;
		d << 3;
		d.SetName("имя");
		xData r = d.GetObjectDescriptor();
		char* c = r;
		char Res[] = "[имя] container(DT_INT32) ext&cs size=3\n   1, 2, 3";
		if (r!=Res) goto ERR;
	}
	// Указатель на другой объект
	{
		xData d1(1);
		xpData d2(d1);
		xData r = d2.GetObjectDescriptor();
		char* c = r;
		char Res[] = "Pointer(DT_UNDEFINE) size=0\n   DT_INT32 size=0 value=1";
		if (r!=Res) goto ERR;
	}
	{
		float scale = 5.23;
		xcData d1;
		d1 << 1;
		d1 << xpData(scale,"scale");
		d1.SetName("one");

		xData d2;
		d2 << -666.666;
		d2 << -33;
		d2 << "text";

		d2 << xpData(d1);

		xData r = d1.GetObjectDescriptor();
		char* c = r;
		char Res[] = "[one] container(DT_UNDEFINE) ext&cs size=2\n"
			"   [0]DT_INT32 size=0 value=1\n"
			"   [1][scale] pointer(DT_FLOAT) ext size=0 value=5.230000019073486";
		if (r!=Res) goto ERR;

		scale = 4;

		float a = d2["one"]["scale"];

		if (a!=4) goto ERR;
	}

	return true;
ERR:
	printf("ObjectDescriptionTest не пройден");
	return false;
}

bool StoreAndRestoreTest()
{
	// Тест указателя на объект
	{
		int v1=1;
		double v2=2;
		char v3[5]="abcd";
		xData v4(10,"zz");
		xData d1;
		d1 << xpData(v1,"v1");
		d1 << xpData(v2,"v2");
		d1 << xpData(v3,"v3");
		d1 << xpData(v4);

		__int8 buf[1024]={0};
		const xData *d2 = d1.GetLinearObject();

		d1[0]=3;
		d1[1]=4;
		d1[2]="ef";

		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2->GetObjectDescriptor();
		char* c2 = r2;

		d1 = d2;

		d2->free();

	}
	return true;
ERR:
	printf("StoreAndRestoreTest не пройден");
	return false;
}

bool ConvertionTest()
{
	// Тест преобразования одного набора простых чисел в другой
	{
		int ar[100];
		for (int i=0;i<100;i++) ar[i]=i;
		xData d1(ar, 100);
		d1.ConvertToType(DT_DOUBLE);
		if (d1.GetThisDataType() != DT_DOUBLE) goto ERR;
		for (int i=0;i<100;i++)
//			if (d1[i]!=i)
				goto ERR;
	}

	//xData d1;
	//int v = 1;
	//d1 << 1 << 2 << xpData(v);

	//d1.ConvertToType(DT_UINT8);


	return true;
ERR:
	printf("StoringTest не пройден");
	return false;
}

bool ContainerTest5()
{
	xData d;
	int b[3]={1,2,3};
	d.Push(b,3);		// Добавляется три элемента
	d.Push("127");
	d.Push(xData(2.71878878,"e"));
	d<<xData(b,3);			// Добавляется контейнер из 3-х элементов
	d<<&xpData(b,3);		// Добавляется три элемента
	d.Push(xData("3.14","Pi"));
	d.Push(xpData(b,3));	// Добавляется указатель на массив из 3-х элементов
	d<<xData(d);			// Добавляется копия объекта

	xData r = d.GetObjectDescriptor();
	char* c = r;

	char Res[]="Container(DT_UNDEFINE) size=12\n   [0]DT_INT32 size=0 value=1\n   [1]DT_INT32 size=0 value=2\n   [2]DT_INT32 size=0 value=3\n"
"   [3]DT_ANSISTRING size=4 value=127\n   [4][e]DT_DOUBLE ext size=0 value=2.71878878\n   [5]Container(DT_INT32) size=3\n      1, 2, 3\n"
"   [6]DT_INT32 size=0 value=1\n   [7]DT_INT32 size=0 value=2\n   [8]DT_INT32 size=0 value=3\n   [9][Pi]DT_ANSISTRING ext size=5 value=3.14\n"
"   [10]Pointer to array(DT_INT32) size=3\n      1, 2, 3\n   [11]Container(DT_UNDEFINE) size=11\n      [0]DT_INT32 size=0 value=1\n      [1]DT_INT32 size=0 value=2\n"
"      [2]DT_INT32 size=0 value=3\n      [3]DT_ANSISTRING size=4 value=127\n      [4][e]DT_DOUBLE ext size=0 value=2.71878878\n      [5]Container(DT_INT32) size=3\n"
"         1, 2, 3\n      [6]DT_INT32 size=0 value=1\n      [7]DT_INT32 size=0 value=2\n      [8]DT_INT32 size=0 value=3\n      [9][Pi]DT_ANSISTRING ext size=5 value=3.14\n"
"      [10]Pointer to array(DT_INT32) size=3\n         1, 2, 3";		
		
		
	if (r!=Res) goto ERR;

	double Pi = d[11]["Pi"];
	if (Pi!=3.14) goto ERR;

	return true;
ERR:
	printf("ContainerTest5 не пройден");
	return false;
}

bool SyncronizeTest()
{
	// Тест синхронизации простого объекта
	{
		__int8 i=1;
		xpData d1(i,"data");
		const xData *d2 = d1.GetLinearObject();

		i=0;

		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2->GetObjectDescriptor();
		char* c2 = r2;

		d1 = d2;

		xData r3 = d1.GetObjectDescriptor();
		char* c3 = r3;
		xData r4 = d2->GetObjectDescriptor();
		char* c4 = r4;

		if (i!=1) goto ERR;
		if (!d1.GetName()) goto ERR;
	}
	// Тест синхронизации простого контейнера
	{
		__int8 buf[100];
		for (int i=0;i<sizeof(buf);i++) buf[i]=i;
		xpData d1(buf, sizeof(buf), "data");
		const xData *d2 = d1.GetLinearObject();
		for (int i=0;i<sizeof(buf);i++) buf[i]=0;

		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2->GetObjectDescriptor();
		char* c2 = r2;


		d1 = d2;

		for (int i=0;i<sizeof(buf);i++) 
			if (buf[i]!=i) goto ERR;

		xData r3 = d1.GetObjectDescriptor();
		char* c3 = r3;
		xData r4 = d2->GetObjectDescriptor();
		char* c4 = r4;
	}
	// Тест синхронизации комплексного контейнера
	{
		__int8 buf[100];
		for (int i=0;i<sizeof(buf);i++) buf[i]=i;
		xData d1;
		d1.SetName("param");
		d1 << xpData(buf,100,"data");
		d1 << xData(1,"randomvalue");
		const xData *d2 = d1.GetLinearObject();
		for (int i=0;i<sizeof(buf);i++) buf[i]=0;
		d1["randomvalue"] = 2;

		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2->GetObjectDescriptor();
		char* c2 = r2;

		d1 = d2;

		if (int(d1["randomvalue"])!=1) goto ERR;

		xData r3 = d1.GetObjectDescriptor();
		char* c3 = r3;
		xData r4 = d2->GetObjectDescriptor();
		char* c4 = r4;
	}
	// Тест синхронизации комплексного контейнера с несовпадающими объектами
	{
		xData d1;
		d1.SetName("abc");
		int v1=123;
		d1 << xData(456,"param2");
		d1 << xpData(v1,"param1");
		xData d2;
		d2.SetName("abc");
		d2 << xData(111,"param1");
		d2 << xData(222,"param2");

		d1 = &d2;

		if (v1!=111) goto ERR;
		if (int(d1["param2"])!=222) goto ERR;

		xData r3 = d1.GetObjectDescriptor();
		char* c3 = r3;
		xData r4 = d2.GetObjectDescriptor();
		char* c4 = r4;
	}
	return true;
ERR:
	printf("SyncronizeTest не пройден");
	return false;
}

// Тест оператора присваивания
bool EqualTest()
{
		// Тест присваивания простому объекту. Аргументом является простой объект
	{
		xData d1, d2(1,"name");
		d1 = d2;
		if (d1.IsPointer()) goto ERR;
		if (d1.IsContainer()) goto ERR;
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

		xData r1 = d1.GetObjectDescriptor();
		char* c1 = r1;
		xData r2 = d2.GetObjectDescriptor();
		char* c2 = r2;

		if (!d1.IsContainer()) goto ERR;
		if (!d1[1].CompareWithName("name")) goto ERR;
	}
	// Тест присваивания контейнера объектов, один из которых тоже контейнер
	{
		int p=3;
		xData d1,d2;
		xpData d3;
		d1 << xpData(p);
		d1 << 1;
		d2 << d1;
		d2 << 2;
		
		xData d4(d2);
		xData r1 = d3.GetObjectDescriptor();
		char* c1 = r1;
	}

	return true;
ERR:
	printf("EqualTest не пройден");
	return false;
}

