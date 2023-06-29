#include "getopt.h"

static int optind = 0;
static int first_nonopt = 0;
static int last_nonopt = 0;

static void exchange(TCHAR** argv)
{
	int bottom = first_nonopt;
	int middle = last_nonopt;
	int top = optind;
	TCHAR* temp;

	while (top > middle && middle > bottom)
	{
		if (top - middle > middle - bottom)
		{
			int len = middle - bottom;
			int i;

			for (i = 0; i < len; i++)
			{
				temp = argv[bottom + i];
				argv[bottom + i] = argv[top - (middle - bottom) + i];
				argv[top - (middle - bottom) + i] = temp;
			}

			top -= len;
		}
		else
		{
			int len = top - middle;
			int i;
			for (i = 0; i < len; i++)
			{
				temp = argv[bottom + i];
				argv[bottom + i] = argv[middle + i];
				argv[middle + i] = temp;
			}
			bottom += len;
		}
	}
	first_nonopt += (optind - last_nonopt);
	last_nonopt = optind;
}

static enum parse_opt_result get_value(const struct option* opt,TCHAR* optarg)
{
	const TCHAR* kTrue[] = { _T("1"), _T("t"), _T("true"), _T("y"), _T("yes") };
	const TCHAR* kFalse[] = { _T("0"), _T("f"), _T("false"), _T("n"), _T("no") };

	INT64 r = 0L;
	UINT64 ur = 0L;
	double d = 0.0;

	switch (opt->type)
	{
	case OPTION_ENABLE:
		if (optarg != NULL)
			return PARSE_OPT_ERROR;

		*(BOOL*)opt->value = TRUE;
		return PARSE_OPT_DONE;

	case OPTION_DISABLE:
		if (optarg != NULL)
			return PARSE_OPT_ERROR;

		*(BOOL*)opt->value = FALSE;
		return PARSE_OPT_DONE;

	case OPTION_BOOL:
		for (size_t i = 0; i < sizeof(kTrue) / sizeof(*kTrue); ++i)
		{
			if (!_tcscmp(optarg, kTrue[i]))
			{
				*(BOOL*)opt->value = TRUE;
				return PARSE_OPT_DONE;
			}
		}

		for (size_t i = 0; i < sizeof(kFalse) / sizeof(*kFalse); ++i)
		{
			if (!_tcscmp(optarg, kFalse[i]))
			{
				*(BOOL*)opt->value = TRUE;
				return PARSE_OPT_DONE;
			}
		}
		return PARSE_OPT_ERROR;

	case OPTION_INT32:
		r = _tcstoi64(optarg, NULL, 0);
		if ((INT32)r != r)
			return PARSE_OPT_ERROR;
		*(INT32*)opt->value = (INT32)r;
		return PARSE_OPT_DONE;

	case OPTION_INT64:
		r = _tcstoi64(optarg, NULL, 0);
		*(INT64*)opt->value = (INT64)r;
		return PARSE_OPT_DONE;

	case OPTION_UINT32:
		ur = _tcstoui64(optarg, NULL, 0);
		if ((UINT32)ur != ur)
			return PARSE_OPT_ERROR;
		*(UINT32*)opt->value = (UINT32)ur;
		return PARSE_OPT_DONE;

	case OPTION_UINT64:
		ur = _tcstoui64(optarg, NULL, 0);
		*(UINT64*)opt->value = (UINT64)ur;
		return PARSE_OPT_DONE;

	case OPTION_DOUBLE:
		d = _tcstod(optarg, NULL);
		*(double*)opt->value = d;
		return PARSE_OPT_DONE;

	case OPTION_STRING:
		if (optarg == NULL)
			return PARSE_OPT_ERROR;
		*(const TCHAR**)opt->value = (const TCHAR*)optarg;
		return PARSE_OPT_DONE;

	default:
		return PARSE_OPT_ERROR;
	}

}

static enum parse_opt_result parse_short_opt(int argc,TCHAR* arg, const struct option* options)
{
	for (; options->type != OPTION_END; options++)
	{
		if (options->short_name == arg[0])
		{
			const TCHAR* optarg = ((*(arg + 1)) == _T('=')) ? arg + 2 : NULL;
			return get_value(options, optarg);
		}
	}
	return PARSE_OPT_UNKNOWN;
}

static enum parse_opt_result parse_long_opt(int argc,TCHAR* arg, const struct option* options)
{
	TCHAR* nameend;
	for (nameend = (TCHAR*)arg; *nameend && *nameend != _T('='); nameend++)
		;

	for (; options->type != OPTION_END; options++)
	{
		if (options->long_name && !_tcsncmp(options->long_name, arg, nameend - arg))
		{
			if ((unsigned int)(nameend - arg) == (unsigned int)_tcslen(options->long_name))
			{
				const TCHAR* optarg = (*nameend) ? nameend + 1 : NULL;
				return get_value(options, optarg);
			}
		}
	}
	return PARSE_OPT_UNKNOWN;
}

#define USAGE_OPTS_WIDTH		24
#define USAGE_GAP				2
static enum parse_opt_result usage_with_options(const struct option* opts)
{
	FILE* outfile = stderr;

	for (; opts->type != OPTION_END; opts++)
	{
		size_t pos;
		int pad;

		if (opts->type == OPTION_MSG)
		{
			_ftprintf(outfile, _T("%s\n"), opts->help);
			continue;
		}

		pos = _ftprintf(outfile, _T("    "));
		if (opts->short_name)
			pos += _ftprintf(outfile, _T("-%c"), opts->short_name);

		if (opts->long_name && opts->short_name)
			pos += _ftprintf(outfile, _T(", "));

		if (opts->long_name)
			pos += _ftprintf(outfile, _T("--%s"), opts->long_name);

		if (opts->argh)
			pos += _ftprintf(outfile, _T("[=<%s>]"), opts->argh);

		if (pos <= USAGE_OPTS_WIDTH)
			pad = USAGE_OPTS_WIDTH - pos;
		else
		{
			_fputtc(_T('\n'), outfile);
			pad = USAGE_OPTS_WIDTH;
		}
		_ftprintf(outfile, _T("%*s%s\n"), pad + USAGE_GAP, _T(""), opts->help);
	}
	_fputtc(_T('\n'), outfile);

	return PARSE_OPT_HELP;
}



int getopt(int argc,TCHAR** argv, const struct option* options)
{
	while (optind <= argc)
	{
		// move the non-option arguments to back 
		if (last_nonopt > optind)
			last_nonopt = optind;

		if (first_nonopt > optind)
			first_nonopt = optind;

		if (first_nonopt != last_nonopt && last_nonopt != optind)
			exchange(argv);
		else if (last_nonopt != optind)
			first_nonopt = optind;

		while (optind < argc && (argv[optind][0] != _T('-') || argv[optind][1] == _T('\0')))
			optind++;
		last_nonopt = optind;

		// return if there is no more command line options
		if (optind == argc)
		{
			if (first_nonopt != last_nonopt)
				optind = first_nonopt;
			return optind;
		}

		// find the options
		TCHAR* arg = argv[optind];

		// short options
		if (arg[1] != _T('-'))
		{
			if (arg[1] == _T('h'))
				return usage_with_options(options);

			switch (parse_short_opt(argc, arg + 1, options))
			{
			case PARSE_OPT_ERROR:
				return PARSE_OPT_ERROR;
			case PARSE_OPT_UNKNOWN:
				return usage_with_options(options);
			case PARSE_OPT_DONE:
				break;
			default:
				break;
			}
			optind++;
			continue;
		}

		if (!_tcscmp(arg + 2, _T("help")))
			return usage_with_options(options);

		switch (parse_long_opt(argc, arg + 2, options))
		{
		case PARSE_OPT_ERROR:
			return PARSE_OPT_ERROR;
		case PARSE_OPT_UNKNOWN:
			return usage_with_options(options);
		case PARSE_OPT_DONE:
			break;
		default:
			break;
		}
		optind++;
	}
	return PARSE_OPT_ERROR;
}