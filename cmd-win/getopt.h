#pragma
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

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

enum parse_cmd_type {
	/* special type */
	CMD_END,
	CMD_MSG,
	/* normal command */
	CMD_NOR,
};

enum parse_cmd_result {
	PARSE_CMD_HELP = -2,
	PARSE_CMD_ERROR = -1,
	PARSE_CMD_DONE = 0,
	PARSE_CMD_UNKNOWN
};


struct command {
	enum parse_cmd_type type;
	const TCHAR* cmd;
	const TCHAR* argh;
	const TCHAR* help;
	int (*fn)(int, TCHAR**);
};

extern int getopt(int argc, TCHAR** argv, const struct option* options);
extern int getcmd(int argc, TCHAR** argv, const struct command* cmds);
extern int usage_with_commands(const struct command* cmds);
