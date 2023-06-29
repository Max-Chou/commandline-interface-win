#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#include "getopt.h"

static BOOL enable = FALSE;
static TCHAR* name = _T("");
static double price = 0;
static INT32 number = 0;

static struct option options[] = {
	{OPTION_MSG, 0, NULL, NULL, NULL, _T("Usage: cmd-win [<option>] ...")},
	{OPTION_MSG, 0, NULL, NULL, NULL, _T("Options")},
	{OPTION_ENABLE, _T('e'), _T("enable"), &enable, NULL, _T("Enable Feature")},
	{OPTION_STRING, _T('n'), _T("name"), &name, NULL, _T("Filename")},
	{OPTION_INT32, 0, _T("number"), &number, NULL, _T("Number")},
	{OPTION_DOUBLE, _T('p'), _T("price"), &price, NULL, _T("Price")},
	{OPTION_END}
};


int _tmain(int argc, TCHAR** argv)
{
	int optind = getopt(argc, argv, options);

	if (optind >= 0)
	{
		if (enable)
		{
			_tprintf(_T("Enable!\n"));
		}
		else
		{
			_tprintf(_T("Disable!\n"));
		}

		_tprintf(_T("Filename is %s\n"), name);
		_tprintf(_T("Number is %d\n"), number);
		_tprintf(_T("Price is %lf\n"), price);

	}
	else
	{
		return optind;
	}

	return 0;
}