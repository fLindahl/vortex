//------------------------------------------------------------------------------
//  @file		debug.cc
//  @copyright	See licence file
//------------------------------------------------------------------------------
#include "config.h"
#include "core/types.h"
#include "foundation/util/string.h"

//------------------------------------------------------------------------------
/**
This function is called by n_assert() when the assertion fails.
*/
void
_exception(const char* exp, const char* file, int line)
{
	//if (IO::Console::HasInstance())
	//{
	//	n_error("*** NEBULA ASSERTION ***\nexpression: %s\nfile: %s\nline: %d\n", exp, file, line);
	//}
	//else
	{
		Util::String msg;
		msg.Format("*** VORTEX ASSERTION ***\nexpression: %s\nfile: %s\nline: %d\n", exp, file, line);
		//Core::SysFunc::Error(msg.AsCharPtr());
	}
}

//------------------------------------------------------------------------------
/**
This function is called by n_assert2() when the assertion fails.
*/
void
_exception2(const char* exp, const char* msg, const char* file, int line)
{
	//if (IO::Console::HasInstance())
	//{
	//	n_error("*** NEBULA ASSERTION ***\nprogrammer says: %s\nexpression: %s\nfile: %s\nline: %d\n", msg, exp, file, line);
	//}
	//else
	{
		Util::String fmt;
		fmt.Format("*** VORTEX ASSERTION ***\nmessage: %s\nexpression: %s\nfile: %s\nline: %d\n", msg, exp, file, line);
		//Core::SysFunc::Error(fmt.AsCharPtr());
	}
}


//------------------------------------------------------------------------------
/**
*/
void
_exception_fmt(const char *	exp, const char *fmt, const char *file, int line, ...)
{
	Util::String msg;
	va_list argList;
	va_start(argList, line);
	msg.FormatArgList(fmt, argList);
	va_end(argList);
	Util::String format = Util::String::Sprintf("*** VORTEX ASSERTION ***\nmessage: %s\nfile : %s\nline : %d\nexpression : %s\n", msg.AsCharPtr(), file, line, exp);
	//if (IO::Console::HasInstance())
	//{
	//	n_error(format.AsCharPtr());
	//}
	//else
	{
		//Core::SysFunc::Error(format.AsCharPtr());
	}
}

//------------------------------------------------------------------------------
/**
	This function is called when a serious situation is encountered which
	requires abortion of the application.
*/
void __cdecl
_error(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	//if (IO::Console::HasInstance())
	//{
	//	IO::Console::Instance()->Error(msg, argList);
	//}
	//else
	{
		Util::String str;
		str.FormatArgList(msg, argList);
		//Core::SysFunc::Error(str.AsCharPtr());
	}
	va_end(argList);
}

//------------------------------------------------------------------------------
/**
This function is called when a warning should be issued which doesn't
require abortion of the application.
*/
void __cdecl
_warning(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	//if (IO::Console::HasInstance())
	//{
	//	IO::Console::Instance()->Warning(msg, argList);
	//}
	//else
	{
		Util::String str;
		str.FormatArgList(msg, argList);
		//Core::SysFunc::MessageBox(str.AsCharPtr());
	}
	va_end(argList);
}

//------------------------------------------------------------------------------
/**
This function is called when a message should be displayed to the
user which requires a confirmation (usually displayed as a MessageBox).
*/
void __cdecl
_confirm(const char* msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	//if (IO::Console::HasInstance())
	//{
	//	IO::Console::Instance()->Confirm(msg, argList);
	//}
	//else
	{
		Util::String str;
		str.FormatArgList(msg, argList);
		//Core::SysFunc::MessageBox(str.AsCharPtr());
	}
	va_end(argList);
}

//------------------------------------------------------------------------------
/**
Nebula's printf replacement. Will redirect the text to the console
and/or logfile.

- 27-Nov-98   floh    created
*/
void __cdecl
_printf(const char *msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	//IO::Console::Instance()->Print(msg, argList);
	va_end(argList);
}

//------------------------------------------------------------------------------
/**
Put process to sleep.

- 21-Dec-98   floh    created
*/
void
_sleep(double sec)
{
	//Core::SysFunc::Sleep(sec);
}

//------------------------------------------------------------------------------
/**
*/
void
_break()
{
#ifndef __WIN32__
	n_error("Break not implemented\n");
#else
	_CrtDbgBreak();
#endif
}
