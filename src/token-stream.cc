/*
   Copyright (c) 2015, Andreas Fett. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.
*/

#include <errno.h>
#include <inttypes.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <stdexcept>

#include "token-stream.h"
#include "utf8stream.h"

namespace {

bool is_ws(int c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

}

namespace jsonp {

// LCOV_EXCL_START
std::ostream & operator<<(std::ostream & os, Token::Type type)
{
#define CASE_TOKEN_TYPE(name) case name: os << # name; break
	switch (type) {
	CASE_TOKEN_TYPE(Token::INVALID);
	CASE_TOKEN_TYPE(Token::BEGIN_ARRAY);
	CASE_TOKEN_TYPE(Token::END_ARRAY);
	CASE_TOKEN_TYPE(Token::BEGIN_OBJECT);
	CASE_TOKEN_TYPE(Token::END_OBJECT);
	CASE_TOKEN_TYPE(Token::NAME_SEPARATOR);
	CASE_TOKEN_TYPE(Token::VALUE_SEPARATOR);
	CASE_TOKEN_TYPE(Token::TRUE_LITERAL);
	CASE_TOKEN_TYPE(Token::FALSE_LITERAL);
	CASE_TOKEN_TYPE(Token::NULL_LITERAL);
	CASE_TOKEN_TYPE(Token::STRING);
	CASE_TOKEN_TYPE(Token::NUMBER);
	}
#undef CASE_TOKEN_TYPE
	return os;
}
// LCOV_EXCL_STOP

TokenStream::TokenStream(Utf8Stream & stream)
:
	stream_(stream)
{ }

void TokenStream::scan()
{
	token.reset();

	int c;
	do {
		c = stream_.getc();
	} while (is_ws(c));

	if (c == int(Utf8Stream::SEOF)) {
		return;
	}

	if (stream_.state() == Utf8Stream::SBAD) {
		return;
	}

	try {
		(this->*select_scanner(c))();
	} catch (std::runtime_error const& e) {
		throw;
	}
}

TokenStream::scanner TokenStream::select_scanner(int c)
{
	scanner res(0);

	switch (c) {
	case '[': case '{': case ']':
	case '}': case ':': case ',':
		res = &TokenStream::scan_structural;
		break;
	case 't':
		res = &TokenStream::scan_true;
		break;
	case 'n':
		res = &TokenStream::scan_null;
		break;
	case 'f':
		res = &TokenStream::scan_false;
		break;
	case '"':
		res = &TokenStream::scan_string;
		break;
	case '-':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		c = '0';
		stream_.ungetc();
		res = &TokenStream::scan_number;
		break;
	default:
		c = 0;
		res = &TokenStream::invalid_token;
		break;
	}

	token.type = Token::Type(c);
	return res;
}

void TokenStream::invalid_token() { throw std::runtime_error("invalid token"); }
void TokenStream::scan_structural() { /* NOOP */ }
void TokenStream::scan_true() { scan_literal("true"); }
void TokenStream::scan_false() { scan_literal("false"); }
void TokenStream::scan_null() { scan_literal("null"); }

void TokenStream::scan_literal(const char *literal)
{
	for (const char *p(&literal[1]); *p; p++) {
		if (stream_.getc() != *p) {
			throw std::runtime_error(literal);
		}
	}
}

void TokenStream::scan_string()
{
	// TODO
}

struct auto_locale {
	auto_locale(const char *name)
	:
		locale_(newlocale(LC_ALL_MASK, name, 0)),
		saved_(uselocale(locale_))
	{ }

	~auto_locale()
	{
		uselocale(saved_);
		freelocale(locale_);
	}

	locale_t locale_;
	locale_t saved_;
};

bool make_int(const char *str, int64_t *res)
{
	errno = 0;
	char *endp(0);
	*res = strtoll(str, &endp, 10);
	if (*endp != '\0') {
		return false;
	}
	if (errno != 0) {
		return false;
	}
	return true;
}

bool make_float(const char *str, long double *res)
{
	errno = 0;
	char *endp(0);
	auto_locale lc("C");
	*res = strtold(str, &endp);
	if (*endp != '\0') {
		return false;
	}
	if (errno != 0) {
		return false;
	}
	return true;
}

enum NumberState {
	SSTART = 0,
	SMINUS,
	SINT_ZERO,
	SINT_DIGIT,
	SINT_DIGIT19,
	SDEC_POINT,
	SFRAC_DIGIT,
	SE,
	SE_PLUS,
	SE_MINUS,
	SE_DIGIT,
	SDONE,
	SERROR,
};

NumberState number_state(int c, NumberState state)
{
#define DIGIT19 "123456789"
#define DIGIT "0" DIGIT19
	struct {
		const char *match;
		NumberState state;
	} transitions[][4] = {
	/* SSTART       */ {{"-", SMINUS},   {"0", SINT_ZERO},  {DIGIT19, SINT_DIGIT19}, {0, SERROR}},
	/* SMINUS       */ {                 {"0", SINT_ZERO},  {DIGIT19, SINT_DIGIT19}, {0, SERROR}},
	/* SINT_ZERO    */ {{"eE", SE},      {".", SDEC_POINT},                          {0, SDONE} },
	/* SINT_DIGIT   */ {{"eE", SE},      {".", SDEC_POINT}, {DIGIT, SINT_DIGIT},     {0, SDONE} },
	/* SINT_DIGIT19 */ {{"eE", SE},      {".", SDEC_POINT}, {DIGIT, SINT_DIGIT},     {0, SDONE} },
	/* SDEC_POINT   */ {{"eE", SE},                         {DIGIT, SFRAC_DIGIT},    {0, SERROR}},
	/* SFRAC_DIGIT  */ {{"eE", SE},                         {DIGIT, SFRAC_DIGIT},    {0, SDONE} },
	/* SE           */ {{"-", SE_MINUS}, {"+", SE_PLUS},    {DIGIT, SE_DIGIT},       {0, SERROR}},
	/* SE_PLUS      */ {                                    {DIGIT, SE_DIGIT},       {0, SERROR}},
	/* SE_MINUS     */ {                                    {DIGIT, SE_DIGIT},       {0, SERROR}},
	/* SE_DIGIT     */ {                                    {DIGIT, SE_DIGIT},       {0, SDONE} },
	};

	for (size_t t(0); true; ++t) {
		const char *match(transitions[state][t].match);
		if (!match || strchr(match, c)) {
			return transitions[state][t].state;
		}
	}

	return SERROR;
}

Token::NumberType validate_number(Utf8Stream & stream, char *buf, size_t size)
{
	NumberState state(SSTART);
	Token::NumberType res(Token::INT);
	size_t i(0);
	for (;;) {
		int c(stream.getc());
		state = number_state(c, state);

		switch (state) {
		case SSTART:
			break;
		case SDEC_POINT:
		case SE:
			res = Token::FLOAT;
			// fallthrough
		case SMINUS:
		case SINT_ZERO:
		case SINT_DIGIT:
		case SINT_DIGIT19:
		case SFRAC_DIGIT:
		case SE_MINUS:
		case SE_PLUS:
		case SE_DIGIT:
			buf[i] = c;
			if (++i == size) {
				buf[i - 1] = '\0';
				return Token::NONE;
			}
			break;
		case SERROR:
			res = Token::NONE;
			// fallthrough
		case SDONE:
			buf[i] = '\0';
			stream.ungetc();
			return res;
		}
	}

	return Token::NONE;
}

void TokenStream::scan_number()
{
	char buf[1024];
	token.number_type = validate_number(stream_, buf, sizeof(buf));
	switch (token.number_type) {
	case Token::INT:
		if (!make_int(buf, &token.int_value)) {
			token.type = Token::INVALID;
			token.number_type = Token::NONE;
			token.int_value = 0;
			return;
		}
		break;
	case Token::FLOAT:
		if (!make_float(buf, &token.float_value)) {
			token.type = Token::INVALID;
			token.number_type = Token::NONE;
			token.float_value = 0;
			return;
		}
		break;
	case Token::NONE:
		token.type = Token::INVALID;
		break;
	}
}

}
