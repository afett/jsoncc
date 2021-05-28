/*
   Copyright (c) 2015, 2016, 2018, 2021 Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <jsoncc.h>

namespace Json {

Array::Array()
:
	elements_()
{ }

Array make_array(std::initializer_list<Value> l)
{
	return Array(std::begin(l), std::end(l));
}

Array::Array(Array const& o)
:
	elements_(o.elements_)
{ }

Array::Array(Array && o)
:
	elements_(std::move(o.elements_))
{ }

Array & Array::operator=(Array const& o)
{
	if (&o != this) {
		elements_ = o.elements_;
	}
	return *this;
}

Array & Array::operator=(Array && o)
{
	if (&o != this) {
		elements_ = std::move(o.elements_);
	}
	return *this;
}

Array & Array::operator<<(Value const& element)
{
	elements_.push_back(element);
	return *this;
}

Array & Array::operator<<(Value && element)
{
	elements_.push_back(std::move(element));
	return *this;
}

size_t Array::size() const
{
	return elements_.size();
}

std::vector<Value> Array::elements() const
{
	return elements_;
}

std::vector<Value>::const_iterator Array::begin() const
{
	return elements_.begin();
}

std::vector<Value>::const_iterator Array::end() const
{
	return elements_.end();
}

}
