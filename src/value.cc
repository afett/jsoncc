/*
   Copyright (c) 2015 - 2018 Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
 */
#include <jsoncc.h>
#include <cstring>
#include <cassert>

namespace Json {

static True TrueValue;
static False FalseValue;
static Null NullValue;

Value::Value()
:
	tag_(TAG_INVALID)
{ }

Value::Value(Value const& o)
:
	tag_(TAG_INVALID)
{
	clone(o);
}

Value::Value(Value&& o)
:
	tag_(TAG_INVALID)
{
	*this = std::move(o);
}

Value & Value::operator=(Value const& o)
{
	if (&o != this) {
		clear();
		clone(o);
	}
	return *this;
}

Value & Value::operator=(Value&& o)
{
	if (&o == this) {
		return *this;
	}

	clear();
	tag_ = o.tag_;

	switch (tag_) {
	case TAG_INVALID:
	case TAG_TRUE:
	case TAG_FALSE:
	case TAG_NULL:
		break;
	case TAG_NUMBER:
		assert(o.number_);
		number_ = std::move(o.number_);
		break;
	case TAG_STRING:
		assert(o.string_);
		string_ = std::move(o.string_);
		break;
	case TAG_OBJECT:
		assert(o.object_);
		object_ = std::move(o.object_);
		break;
	case TAG_ARRAY:
		assert(o.array_);
		array_ = std::move(o.array_);
		break;
	}

	o.tag_ = TAG_INVALID;
	return *this;
}

Value::~Value()
{ }

void Value::clone(Value const& o)
{
	assert(&o != this);

	tag_ = o.tag_;

	switch (tag_) {
	case TAG_INVALID:
	case TAG_TRUE:
	case TAG_FALSE:
	case TAG_NULL:
		break;
	case TAG_NUMBER:
		assert(o.number_);
		number_.reset(new Number(*o.number_));
		break;
	case TAG_STRING:
		assert(o.string_);
		string_.reset(new String(*o.string_));
		break;
	case TAG_OBJECT:
		assert(o.object_);
		object_.reset(new Object(*o.object_));
		break;
	case TAG_ARRAY:
		assert(o.array_);
		array_.reset(new Array(*o.array_));
		break;
	}
}

void Value::clear()
{
	number_.reset();
	string_.reset();
	object_.reset();
	object_.reset();
	tag_ = TAG_INVALID;
}

Value::Value(Null const&)
:
	tag_(TAG_NULL)
{ }

Value::Value(True const&)
:
	tag_(TAG_TRUE)
{ }

Value::Value(False const&)
:
	tag_(TAG_FALSE)
{ }

Value::Value(Number const& number)
:
	tag_(TAG_NUMBER)
{
	number_.reset(new Number(number));
}

Value::Value(String const& string)
:
	tag_(TAG_STRING)
{
	string_.reset(new String(string));
}

Value::Value(Object const& object)
:
	tag_(TAG_OBJECT)
{
	object_.reset(new Object(object));
}

Value::Value(Array const& array)
:
	tag_(TAG_ARRAY)
{
	array_.reset(new Array(array));
}

void Value::set(Null const&)
{
	clear();
	tag_ = TAG_NULL;
}

void Value::set(True const&)
{
	clear();
	tag_ = TAG_TRUE;
}

void Value::set(False const&)
{
	clear();
	tag_ = TAG_FALSE;
}

void Value::set(Number const& number)
{
	clear();
	tag_ = TAG_NUMBER;
	number_.reset(new Number(number));
}

void Value::set(String const& string)
{
	clear();
	tag_ = TAG_STRING;
	string_.reset(new String(string));
}

void Value::set(Object const& object)
{
	clear();
	tag_ = TAG_OBJECT;
	object_.reset(new Object(object));
}

void Value::set(Array const& array)
{
	clear();
	tag_ = TAG_ARRAY;
	array_.reset(new Array(array));
}

void Value::build(std::unique_ptr<Number> number)
{
	clear();
	tag_ = TAG_NUMBER;
	number_ = std::move(number);
}

void Value::build(std::unique_ptr<String> string)
{
	clear();
	tag_ = TAG_STRING;
	string_ = std::move(string);
}

void Value::build(std::unique_ptr<Object> object)
{
	clear();
	tag_ = TAG_OBJECT;
	object_ = std::move(object);
}

void Value::build(std::unique_ptr<Array> array)
{
	clear();
	tag_ = TAG_ARRAY;
	array_ = std::move(array);
}

Value::Tag Value::tag() const
{
	return tag_;
}

Null const& Value::null() const
{
	assert(tag_ == TAG_NULL);
	return NullValue;
}

True const& Value::true_value() const
{
	assert(tag_ == TAG_TRUE);
	return TrueValue;
}

False const& Value::false_value() const
{
	assert(tag_ == TAG_FALSE);
	return FalseValue;
}

Number const& Value::number() const
{
	assert(tag_ == TAG_NUMBER);
	assert(number_);
	return *number_;
}

String const& Value::string() const
{
	assert(tag_ == TAG_STRING);
	assert(string_);
	return *string_;
}

Array const& Value::array() const
{
	assert(tag_ == TAG_ARRAY);
	assert(array_);
	return *array_;
}

Object const& Value::object() const
{
	assert(tag_ == TAG_OBJECT);
	assert(object_);
	return *object_;
}

void ValueFactory<bool>::build(bool const& value, Value & res)
{
	if (value) {
		res.set(True());
	} else {
		res.set(False());
	}
}

void ValueFactory<uint8_t>::build(uint8_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<int8_t>::build(int8_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<uint16_t>::build(uint16_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<int16_t>::build(int16_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<uint32_t>::build(uint32_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<int32_t>::build(int32_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<uint64_t>::build(uint64_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<int64_t>::build(int64_t const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<float>::build(float const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<double>::build(double const& value, Value & res)
{
	res.make<Number>(value);
}

void ValueFactory<long double>::build(long double const& value, Value & res)
{
	res.make<Number>(value);
}

}
