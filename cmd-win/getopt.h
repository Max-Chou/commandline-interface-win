#pragma
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

enum parse_opt_type {
	/* special type */
	OPTION_END,
	OPTION_MSG,
	/* options with no arguments */
	OPTION_ENABLE,
	OPTION_DISABLE,
	OPTION_CALLBACK,
	/* options with arguments */
	OPTION_STRING,
	OPTION_INT32,
	OPTION_INT64,
	OPTION_BOOL,
	OPTION_UINT32,
	OPTION_UINT64,
	OPTION_DOUBLE,
};

enum parse_opt_result {
	PARSE_OPT_HELP = -2,
	PARSE_OPT_ERROR = -1,
	PARSE_OPT_DONE = 0,
	PARSE_OPT_UNKNOWN
};

struct option {
	enum parse_opt_type type;
	int short_name;
	const TCHAR* long_name;
	void* value;
	const TCHAR* argh;
	const TCHAR* help;
};


extern int getopt(int argc, TCHAR** argv, const struct option* options);
