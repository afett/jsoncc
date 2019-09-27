#include <cppunit/extensions/HelperMacros.h>

#include <jsoncc-cppunit.h>
#include <algorithm>

namespace Json {

bool is_equal(Number const& l, Number const& r, std::stringstream & reason)
{
	if (&l == &r) {
		return true;
	}

	if (l.type() != r.type()) {
		reason << "Number Type: " << l.type() << " != " << r.type();
		return false;
	}

	switch (l.type()) {
	case Number::TYPE_INVALID:
		return true;
	case Number::TYPE_UINT:
		if (l.uint_value() == r.uint_value()) {
			return true;
		} else {
			reason << "Number: " << l.uint_value() << " != " << r.uint_value();
			break;
		}
	case Number::TYPE_INT:
		if (l.int_value() == r.int_value()) {
			return true;
		} else {
			reason << "Number: " << l.int_value() << " != " << r.int_value();
			break;
		}

	case Number::TYPE_FP:
		if (l.fp_value() == r.fp_value()) {
			return true;
		} else {
			reason << "Number: " << l.fp_value() << " != " << r.fp_value();
			break;
		}
	}

	return false;
}

bool is_equal(String const& l, String const& r, std::stringstream & reason)
{
	if (&l == &r) {
		return true;
	}
	if (l.value() == r.value()) {
		return true;
	}

	reason << "String: '" << l.value() << "' != '" << r.value() << "'";
	return false;
}

bool is_equal(Value const& l, Value const& r, std::stringstream & reason);

bool is_equal(Array const& l, Array const& r, std::stringstream & reason)
{
	if (&l == &r) {
		return true;
	}

	if (l.size() != r.size()) {
		reason << "Array Size: " << l.size() << " != " << r.size();
		return false;
	}

	auto re = std::begin(r);
	for (auto const& le : l) {
		if (!is_equal(le, *re++, reason)) {
			reason << " in Array Element: ";
			return false;
		}
	}

	return true;
}

bool is_equal(Member const& l, Member const& r, std::stringstream & reason)
{
	if (&l == &r) {
		return true;
	}

	if (!is_equal(l.key(), r.key(), reason)) {
		reason << " in Member: ";
		return false;
	}

	if (!is_equal(l.value(), r.value(), reason)) {
		reason << " in Member: " << l.key();
		return false;
	}

	return true;
}

bool is_equal(Object const& l, Object const& r, std::stringstream & reason)
{
	if (&l == &r) {
		return true;
	}

	if (l.size() != r.size()) {
		reason << "Object Size: " << l.size() << " != " << r.size();
		return false;
	}

	auto rm = std::begin(r);
	for (auto const& lm : l) {
		if (!is_equal(lm, *rm++, reason)) {
			reason << " in Object";
			return false;
		}
	}

	return true;
}

bool is_equal(Value const& l, Value const& r, std::stringstream & reason)
{
	if (&l == &r) {
		return true;
	}

	if (l.tag() != r.tag()) {
		reason << "Value Tag: " << l.tag() << " != " << r.tag();
		return false;
	}

	switch (l.tag()) {
	case Value::TAG_INVALID:
	case Value::TAG_TRUE:
	case Value::TAG_FALSE:
	case Value::TAG_NULL:
		return true;
	case Value::TAG_NUMBER:
		if (is_equal(l.number(), r.number(), reason)) {
			return true;
		}
		break;
	case Value::TAG_STRING:
		if (is_equal(l.string(), r.string(), reason)) {
			return true;
		}
		break;
	case Value::TAG_OBJECT:
		if (is_equal(l.object(), r.object(), reason)) {
			return true;
		}
		break;
	case Value::TAG_ARRAY:
		if (is_equal(l.array(), r.array(), reason)) {
			return true;
		}
		break;
	}

	reason << " in Value";
	return false;
}

}

namespace unittests {
namespace cppunit {

class test : public CppUnit::TestCase {
public:
	test();
	void setUp();
	void tearDown();

private:
	void simple_test();

	CPPUNIT_TEST_SUITE(test);
	CPPUNIT_TEST(simple_test);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);

test::test()
{ }

void test::setUp()
{ }

void test::tearDown()
{ }

void test::simple_test()
{
	auto ref = Json::Object{
		{"foo", 5},
		{"bar", "peng"},
		{"baz", std::vector<std::string>{}},
	};

	auto res = Json::Object{
		{"foo", 5},
		{"bar", ""},
		{"baz", std::vector<std::string>{}},
	};

	std::stringstream reason;
	if (!is_equal(res, ref, reason)) {
		CPPUNIT_ASSERT_EQUAL(std::string(), reason.str());
		std::cerr << ref;
	}
}

}}
