#include <cppunit/extensions/HelperMacros.h>

namespace {

struct foo_object {
	int a;
	bool b;
	std::string c;
};

struct bar_object {
	int a;
	bool b;
	std::string c;
};

struct baz_object {
	int a;
	bool b;
	std::string c;
};

struct bar_array {

	int a;
	bool b;
	std::string c;
};

struct baz_array {

	int a;
	bool b;
	std::string c;
};

struct bar { };

enum Baz {
	BAZ_1,
	BAZ_2,
	BAZ_3,
};

std::ostream & operator<<(std::ostream & os, ::bar const&)
{
	return os << "Bar Object";
}

std::ostream & operator<<(std::ostream & os, ::foo_object const&)
{
	return os << "Foo Object";
}

std::ostream & operator<<(std::ostream & os, ::Baz baz)
{
	switch (baz) {
	case BAZ_1: os << "BAZ_1"; break;
	case BAZ_2: os << "BAZ_2"; break;
	case BAZ_3: os << "BAZ_3"; break;
	}

	return os;
}

}

// we have to place it here to keep clang++ happy.
// It will warn about unused std::ostream & operator<<(...)
// for the above otherwise...
#include <jsoncc.h>

namespace Json {

template<> struct ValueFactory< ::foo_object> {
	static void build(foo_object const& f, Value & res)
	{
		// construct Object in place
		res.make_object({
			{"a", f.a},
			{"b", f.b},
			{"c", f.c}});
	}
};

template<> struct ValueFactory< ::bar_object> {
	static void build(bar_object const& f, Value & res)
	{
		auto o = Object{
			{"a", f.a},
			{"b", f.b},
			{"c", f.c}};
		// move members to new Object
		CPPUNIT_ASSERT_EQUAL(size_t(3), o.size());
		res.make<Object>(std::move(o));
		CPPUNIT_ASSERT_EQUAL(size_t(0), o.size());
	}
};

template<> struct ValueFactory< ::baz_object> {
	static void build(baz_object const& f, Value & res)
	{
		auto o = Object{
			{"a", f.a},
			{"b", f.b},
			{"c", f.c}};
		// full copy
		res.set(o);
	}
};

template<> struct ValueFactory< ::bar_array> {
	static void build(bar_array const& f, Value & res)
	{
		auto a = Array{f.a, f.b, f.c};
		CPPUNIT_ASSERT_EQUAL(size_t(3), a.size());
		res.make<Array>(std::move(a));
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.size());
	}
};

template<> struct ValueFactory< ::baz_array> {
	static void build(baz_array const& f, Value & res)
	{
		auto a = Array{f.a, f.b, f.c};
		res.set(a);
	}
};

}

namespace unittests {
namespace custom_type {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_custom_type();
	void test_custom_type_move();
	void test_custom_type_copy();
	void test_custom_type_vector();
	void test_streamable_object();
	void test_streamable_enum();
	void test_object_to_array_move();
	void test_object_to_array_copy();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_custom_type);
	CPPUNIT_TEST(test_custom_type_move);
	CPPUNIT_TEST(test_custom_type_copy);
	CPPUNIT_TEST(test_custom_type_vector);
	CPPUNIT_TEST(test_streamable_object);
	CPPUNIT_TEST(test_streamable_enum);
	CPPUNIT_TEST(test_object_to_array_move);
	CPPUNIT_TEST(test_object_to_array_copy);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_custom_type()
{
	struct foo_object f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::stringstream ss;
	ss << f;
	CPPUNIT_ASSERT_EQUAL(std::string("Foo Object"), ss.str());
	ss.str("");

	ss << Json::Value(f);
	std::string expected(
		"{\n"
		"	\"a\": 42,\n"
		"	\"b\": false,\n"
		"	\"c\": \"Foo\"\n"
		"}"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_custom_type_move()
{
	struct bar_object f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::stringstream ss;
	ss << Json::Value(f);
	std::string expected(
		"{\n"
		"	\"a\": 42,\n"
		"	\"b\": false,\n"
		"	\"c\": \"Foo\"\n"
		"}"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_custom_type_copy()
{
	struct baz_object f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::stringstream ss;
	ss << Json::Value(f);
	std::string expected(
		"{\n"
		"	\"a\": 42,\n"
		"	\"b\": false,\n"
		"	\"c\": \"Foo\"\n"
		"}"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_custom_type_vector()
{
	foo_object f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::vector<foo_object> v{f, f};

	std::stringstream ss;
	ss << Json::Value(v);

	std::string expected(
		"[\n"
		"	{\n"
		"		\"a\": 42,\n"
		"		\"b\": false,\n"
		"		\"c\": \"Foo\"\n"
		"	},\n"
		"	{\n"
		"		\"a\": 42,\n"
		"		\"b\": false,\n"
		"		\"c\": \"Foo\"\n"
		"	}\n"
		"]"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_streamable_object()
{
	::bar b;

	std::stringstream ss;
	ss << Json::Value(b);

	std::string expected("\"Bar Object\"");
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_streamable_enum()
{
	Json::Array a;
	a << ::BAZ_1;
	a << ::BAZ_2;
	a << ::BAZ_3;

	std::stringstream ss;
	ss << a;

	std::string expected(
		"[\n"
		"	\"BAZ_1\",\n"
		"	\"BAZ_2\",\n"
		"	\"BAZ_3\"\n"
		"]"
	);
	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_object_to_array_move()
{
	struct bar_array f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::stringstream ss;
	ss << Json::Value(f);
	std::string expected(
		"[\n"
		"	42,\n"
		"	false,\n"
		"	\"Foo\"\n"
		"]"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

void test::test_object_to_array_copy()
{
	struct baz_array f;
	f.a = 42;
	f.b = false;
	f.c = "Foo";

	std::stringstream ss;
	ss << Json::Value(f);
	std::string expected(
		"[\n"
		"	42,\n"
		"	false,\n"
		"	\"Foo\"\n"
		"]"
	);

	CPPUNIT_ASSERT_EQUAL(expected, ss.str());
}

}}
