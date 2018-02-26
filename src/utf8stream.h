/*
   Copyright (c) 2015, 2016, 2018 Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#ifndef JSON_UTF8_STREAM_H
#define JSON_UTF8_STREAM_H

#include <jsoncc.h>
#include "utf8.h"

namespace Json {

class Utf8Stream {
public:
	enum State {
		SGOOD = 0,
		SEOF = -1,
		SBAD = -2,
	};

	Utf8Stream(const char *buf, size_t len)
	:
		buf_(buf),
		len_(len),
		pos_(0),
		bad_(false),
		eof_(false),
		utf8_(),
		valid_(0)
	{ }

	State state() const
	{
		if (bad_) {
			return SBAD;
		}

		if (eof_) {
			return SEOF;
		}

		return SGOOD;
	}

	int getc()
	{
		if (bad_) {
			return SBAD;
		}

		if (pos_ == len_) {
			eof_ = true;
			return SEOF;
		}

		uint8_t c(buf_[pos_]);
		if (c == '\0') {
			bad_ = true;
			throw Error(Error::STREAM_ZERO, location());
		}

		if (valid_ <= pos_) {
			if ((bad_ = !utf8_.validate(c))) {
				throw Error(Error::UTF8_INVALID, location());
			}
			++valid_;
		}

		++pos_;
		return c;
	}

	void ungetc()
	{
		if (pos_ != 0 && !bad_ && !eof_) {
			--pos_;
		}
	}

	Location location() const
	{
		return Location(pos_);
	}

	void bad()
	{
		bad_ = true;
	}

private:
	const char *buf_;
	size_t len_;
	size_t pos_;
	bool bad_;
	bool eof_;
	utf8validator utf8_;
	size_t valid_;
};

}
#endif
