#include"RTTI.h"
#include"Serialize.h"
#include"..\SpCommon\Math.h"
#include<fstream>
#include<sstream>

using namespace std;

#pragma region ColorB

class ColorB {
public:
	float r, g, b;

private:
	static Metadata _MetaData;
	static void Register(Metadata& meta);

public:
	static const Metadata* GetMetadata();
};

Metadata ColorB::_MetaData("ColorB", sizeof(ColorB), NULL, ColorB::Register);

void ColorB::Register(Metadata& meta)
{
	RTTI::RegisterCreatetor(typeid(ColorB).name(), []() { return new ColorB(); });
	meta.AddProperty(Property("", "r", EType::Float, offsetof(ColorB, r)));
	meta.AddProperty(Property("", "g", EType::Float, offsetof(ColorB, g)));
	meta.AddProperty(Property("", "b", EType::Float, offsetof(ColorB, b)));
}

const Metadata* ColorB::GetMetadata()
{
	return &_MetaData;
}

#pragma endregion

#pragma region Pixel

class Pixel {
public:
	ColorB a;
	ColorB b;

private:
	static Metadata _MetaData;
	static void Register(Metadata& meta);

public:
	static const Metadata* GetMetadata();
};

Metadata Pixel::_MetaData("Pixel", sizeof(Pixel), NULL, Pixel::Register);

void Pixel::Register(Metadata& meta)
{
	RTTI::RegisterCreatetor(typeid(Pixel).name(), []() { return new Pixel(); });
	PROP_CLS(Pixel, ColorB, a);
	PROP_CLS(Pixel, ColorB, b);
}

const Metadata* Pixel::GetMetadata()
{
	return &_MetaData;
}

#pragma endregion
#pragma region Person

class Person {
public:
	int id;
	int age;
	float height;
	bool intelligent;
	Int64 identity;
	wstring name;
	string en_name;
	ColorB color = { 0.5f, 0.4f, 0.3f };
	double coord[5] = { 1.1, 2.2, 3.3, 4.4, 5.5 };
	string flags[4] = { "cheers", "hello", "welcome", "happy" };
	wstring frieds[4] = { L"��ȫ��", L"��ȫ��", L"��ȫ��", L"��ȫ��" };
	ColorB colors[2] = { ColorB{ 1.0f, 1.0f, 1.0f }, ColorB{ 0.5f, 0.5f, 0.5f } };
	vector<ColorB> colorList = { ColorB{ 0.5f, 1.0f, 1.0f }, ColorB{ 0.2f, 0.5f, 0.5f }, ColorB{ 0.1f, 0.2f, 0.3f } };
	vector<wstring> friendList = { L"�ﱤ��", L"�ű���" };
	vector<string> flagList = { "cheers", "hello", "welcome", "happy" };
	vector<float> xyz = { 1.0f, 0.0f, 0.0f };
	Vector3 diffuse = { 9.0f, 1.5f, 0.0f };
	vector<Pixel> pixels = {
		Pixel{ ColorB{ 0.1f, 0.1f, 0.1f }, ColorB{ 0.1f, 0.1f, 0.1f } } ,
		Pixel{ ColorB{ 0.2f, 0.2f, 0.2f }, ColorB{ 0.1f, 0.1f, 0.1f } } ,
	};

	Person() : id(9), age(55), height(1.55f), name(L"���"), en_name("Ray"), intelligent(true), identity(800) { }

	virtual void clear()
	{
		id = 0;
		age = 0;
		height = 0;
		intelligent = false;
		identity = 0;
		name = L"";
		en_name = "";
		color = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 5; ++i)
			coord[i] = 0;
		for (int i = 0; i < 4; ++i)
		{
			flags[i] = "";
			frieds[i] = L"";
		}
		for (int i = 0; i < 2; ++i)
			colors[i] = { 0.0f, 0.0f, 0.0f };
		colorList.clear();
		friendList.clear();
		flagList.clear();
		xyz.clear();
		diffuse = Vector3::zero;
		pixels.clear();
	}
private:
	static Metadata _MetaData;
	static void Register(Metadata& meta);

public:
	static const Metadata* GetMetadata();
};

Metadata Person::_MetaData("Person", sizeof(Person), NULL, Person::Register);

void Person::Register(Metadata& meta)
{
	RTTI::RegisterCreatetor(typeid(Person).name(), []() { return new Person(); });
	meta.AddProperty(Property("", "id", EType::Int32, offsetof(Person, id)));
	meta.AddProperty(Property("", "age", EType::Int32, offsetof(Person, age)));
	meta.AddProperty(Property("", "height", EType::Float, offsetof(Person, height)));
	meta.AddProperty(Property("", "identity", EType::Int64, offsetof(Person, identity)));
	meta.AddProperty(Property("", "intelligent", EType::Bool, offsetof(Person, intelligent)));
	meta.AddProperty(Property("", "name", EType::WString, offsetof(Person, name)));
	PROP(Person, en_name, EType::String);
	PROP_CLS(Person, ColorB, color);
	PROP_ARR(Person, double, coord, EType::Double);
	PROP_ARR(Person, string, flags, EType::String);
	PROP_ARR(Person, wstring, frieds, EType::WString);
	PROP_ARR(Person, float, diffuse, EType::Float);
	PROP_VEC(Person, wstring, friendList, EType::WString);
	PROP_VEC(Person, string, flagList, EType::String);
	PROP_VEC(Person, float, xyz, EType::Float);
	PROP_ARR_CLS(Person, ColorB, colors);
	PROP_VEC_CLS(Person, ColorB, colorList);
	PROP_VEC_CLS(Person, Pixel, pixels);
}

const Metadata* Person::GetMetadata()
{
	return &_MetaData;
}

#pragma endregion

#pragma region Man

class Man : public Person {
public:
	float length = 1.5f;

	virtual void clear()
	{
		Person::clear();
		length = 0.0f;
	}
private:
	static Metadata _MetaData;
	static void Register(Metadata& meta);

public:
	static const Metadata* GetMetadata();
};

Metadata Man::_MetaData("Man", sizeof(Man), Person::GetMetadata(), Man::Register);

void Man::Register(Metadata& meta)
{
	RTTI::RegisterCreatetor(typeid(Man).name(), []() { return new Man(); });
	PROP(Man, length, EType::Float);
}

const Metadata* Man::GetMetadata()
{
	return &_MetaData;
}

#pragma endregion

void main()
{
	Man* p = RTTI::Instantiate<Man>("Man");

	// ���������������
	//const vector<Property>* props = p->GetMetadata()->GetProperties();
	//for each (Property prop in *props)
	//{
	//	cout << prop.GetFieldName() << endl;
	//}

	// ���л�
	string json = SerilizeHelper::Serilize(p);
	cout << json.c_str() << endl;

	ofstream os("D://man.json");
	os.write(json.c_str(), json.size());
	os.close();

	// �����л�
	//ifstream is("D://man.json");
	//ostringstream os;
	//os << is.rdbuf();

	//Man* man = new Man();
	//man->clear();
	//SerilizeHelper::Deserilize(man, os.str());

	system("pause");
}