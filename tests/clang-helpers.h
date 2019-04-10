#ifndef clang_helpers_h_
#define clang_helpers_h_

#ifdef __clang__
#define DONT_WARN_ABOUT_SELFASSIGNMENT \
	do { \
	_Pragma("clang diagnostic push"); \
	_Pragma("clang diagnostic ignored \"-Wself-assign-overloaded\""); \
	} while(0)
#define WARN_ABOUT_SELFASSIGNMENT \
	do { \
	_Pragma("clang diagnostic pop"); \
	} while(0)
#else
#define DONT_WARN_ABOUT_SELFASSIGNMENT do {} while(0)
#define WARN_ABOUT_SELFASSIGNMENT do {} while(0)
#endif

#endif
