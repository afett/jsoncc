#include <cppunit/extensions/HelperMacros.h>
#include <jsoncc.h>

#include <jsoncc-cppunit.h>

// LCOV_EXCL_START
std::ostream & operator<<(std::ostream & os, Json::Value::Tag tag)
{
	switch (tag) {
	case Json::Value::TAG_INVALID: os << "INVALID"; break;
	case Json::Value::TAG_NULL:    os << "NULL";    break;
	case Json::Value::TAG_TRUE:    os << "TRUE";    break;
	case Json::Value::TAG_FALSE:   os << "FALSE";   break;
	case Json::Value::TAG_NUMBER:  os << "NUMBER";  break;
	case Json::Value::TAG_STRING:  os << "STRING";  break;
	case Json::Value::TAG_OBJECT:  os << "OBJECT";  break;
	case Json::Value::TAG_ARRAY:   os << "ARRAY";   break;
	}

	return os;
}
// LCOV_EXCL_STOP

namespace unittests {
namespace value {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void test_invalid_type();
	void test_null_type();
	void test_true_type();
	void test_false_type();
	void test_number_type();
	void test_numeric_types();
	void test_string_type();
	void test_object_type();
	void test_array_type();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(test_invalid_type);
	CPPUNIT_TEST(test_null_type);
	CPPUNIT_TEST(test_true_type);
	CPPUNIT_TEST(test_false_type);
	CPPUNIT_TEST(test_number_type);
	CPPUNIT_TEST(test_numeric_types);
	CPPUNIT_TEST(test_string_type);
	CPPUNIT_TEST(test_object_type);
	CPPUNIT_TEST(test_array_type);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::test_invalid_type()
{
	Json::Value t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);

	CPPUNIT_ASSERT(!t1);
}

void test::test_null_type()
{
	Json::Value t0((Json::Null()));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t0.tag());

	Json::Value t1;
	t1.set(Json::Null());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Null());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NULL, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_true_type()
{
	Json::Value t0((Json::True()));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t0.tag());

	Json::Value t1;
	t1.set(Json::True());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::True());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_TRUE, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_false_type()
{
	Json::Value t0((Json::False()));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t0.tag());

	Json::Value t1;
	t1.set(Json::False());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::False());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_FALSE, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_number_type()
{
	Json::Value t0(Json::Number(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t0.tag());

	Json::Value t1;
	t1.set(Json::Number(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Number(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_numeric_types()
{
	Json::Value uint8(uint8_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, uint8.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, uint8.number().type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), uint8.number().uint_value());

	Json::Value int8(int8_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, int8.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, int8.number().type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), int8.number().int_value());

	Json::Value uint16(uint16_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, uint16.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, uint16.number().type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), uint16.number().uint_value());

	Json::Value int16(int16_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, int16.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, int16.number().type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), int16.number().int_value());

	Json::Value uint32(uint32_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, uint32.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, uint32.number().type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), uint32.number().uint_value());

	Json::Value int32(int32_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, int32.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, int32.number().type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), int32.number().int_value());

	Json::Value uint64(uint64_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, uint64.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_UINT, uint64.number().type());
	CPPUNIT_ASSERT_EQUAL(uint64_t(5), uint64.number().uint_value());

	Json::Value int64(int64_t(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, int64.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_INT, int64.number().type());
	CPPUNIT_ASSERT_EQUAL(int64_t(5), int64.number().int_value());

	Json::Value fp_float(float(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, fp_float.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_FP, fp_float.number().type());
	CPPUNIT_ASSERT_EQUAL((long double)(5), fp_float.number().fp_value());

	Json::Value fp_double(double(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, fp_double.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_FP, fp_double.number().type());
	CPPUNIT_ASSERT_EQUAL((long double)(5), fp_double.number().fp_value());

	Json::Value fp_long_double((long double)(5));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_NUMBER, fp_long_double.tag());
	CPPUNIT_ASSERT_EQUAL(Json::Number::TYPE_FP, fp_long_double.number().type());
	CPPUNIT_ASSERT_EQUAL((long double)(5), fp_long_double.number().fp_value());
}

void test::test_string_type()
{
	Json::Value t0(Json::String("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t0.tag());

	Json::Value t1;
	t1.set(Json::String("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::String("foo"));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_STRING, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_object_type()
{
	Json::Value t0((Json::Object()));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t0.tag());

	Json::Value t1;
	t1.set(Json::Object());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Object());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_OBJECT, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

void test::test_array_type()
{
	Json::Value t0((Json::Array()));
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t0.tag());

	Json::Value t1;
	t1.set(Json::Array());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t1.tag());

	t1 = Json::Value();
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_INVALID, t1.tag());

	t1.set(Json::Array());
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t1.tag());

	Json::Value t2(t1);
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t2.tag());

	Json::Value t3;
	t3 = t1;
	CPPUNIT_ASSERT_EQUAL(Json::Value::TAG_ARRAY, t3.tag());

	CPPUNIT_ASSERT_EQUAL(t1, t3);
}

}}
