#include <ostream>

#include <jsoncc.h>

namespace Json {

// LCOV_EXCL_START
static inline std::ostream & operator<<(std::ostream & os, Error::Type type)
{
#define CASE_ERROR_TYPE(name) case name: os << # name; break
	switch (type) {
	CASE_ERROR_TYPE(Error::OK);
	CASE_ERROR_TYPE(Error::STREAM_ZERO);
	CASE_ERROR_TYPE(Error::UTF8_INVALID);
	CASE_ERROR_TYPE(Error::TOKEN_INVALID);
	CASE_ERROR_TYPE(Error::LITERAL_INVALID);
	CASE_ERROR_TYPE(Error::STRING_CTRL);
	CASE_ERROR_TYPE(Error::STRING_QUOTE);
	CASE_ERROR_TYPE(Error::ESCAPE_INVALID);
	CASE_ERROR_TYPE(Error::UESCAPE_INVALID);
	CASE_ERROR_TYPE(Error::UESCAPE_ZERO);
	CASE_ERROR_TYPE(Error::UESCAPE_SURROGATE);
	CASE_ERROR_TYPE(Error::NUMBER_INVALID);
	CASE_ERROR_TYPE(Error::NUMBER_OVERFLOW);
	CASE_ERROR_TYPE(Error::PARSER_OVERFLOW);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_DOCUMENT);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_ARRAY_START);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_ARRAY_VALUE);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_ARRAY_NEXT);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_OBJECT_START);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_OBJECT_NAME);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_OBJECT_SEP);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_OBJECT_VALUE);
	CASE_ERROR_TYPE(Error::BAD_TOKEN_OBJECT_NEXT);
	CASE_ERROR_TYPE(Error::EMPTY_NAME);
	CASE_ERROR_TYPE(Error::INTERNAL_ERROR);
	}
#undef CASE_ERROR_TYPE
	return os;
}
// LCOV_EXCL_STOP

}
