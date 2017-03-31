# Debugging and Useful Features

## Console

Vortex has an built in console that can be brought forth with the `\``-key or by using the `debug menu bar` and going under `View` -> `Show` -> `Console`.

In C++ code you can use the following functions to make use of the console:

`_printf(const char*, ...)` - This is essentially a replacement of the printf function. It writes directly to the console and log file.

`IO::Console::Print(const Util::String& msg, LogMessageType type = IO::MESSAGE, bool once = false)` - Works similarly to the _printf function, except it accepts a string instead of a char pointer.
You can specify message type which will make it possible to filter by type.
It also accepts an optional parameter called "once" which, if set to true, will only print the message once within the context it's created. This can be useful for warning messages that could possibly clog up the console really quickly.

`IO:Console::SaveLog(Util::String filename = "")` - Saves the console into a .log file. Argument "filename" can be used to specify a specific log name. If filename is left empty, a generic name will be generated.

`IO::Console::Show()` - Brings up the console as an ImGui window.

`IO::Console::Hide()` - Hides the ImGui window console.

`IO::Console::ShowNativeConsole()` - Brings up an native platform external window console (ex. CMD, terminal). This console does not accept input.
Note: This can be painfully slow to open if the log is already large, as we need to print everything to the console in one sweep.

`IO::Console::CloseNativeConsole()` - Closes the native platform console

`IO::Console::GetLog()` - Returns the entire log as a list of messages.

## Debugging

There a a couple of useful functions for debugging your code.
Most of these functions are declared in `core/debug.h`.

`_printf(const char*, ...)` - Prints a message to the console

`_error(const char*, ...)` - Prints error message provided to the console. This does not abort the application.

`_warning(const char*, ...)` - Prints warning message provided to the console. This does not exit the application.
 
`_confirm(const char*, ...)` - Brings up a message box with the message provided. This uses an in-game window and does not in anyway pause, nor exit, the application.

`_sleep(double)` - Puts the process to sleep. This assures sleep functionality for all platforms.

`_break()` - Places a breakpoint in the application.

`_assert(bool)` - Replaces the assert function. This aborts the application and prints information to the console+log and also shows it in a pop-up window.

`_assert2(bool, const char*)` - Same as above, but appends a message from the programmer to the user to the error message.

`_assertfmt(bool, const char*, ...)` - Same as above, but with a formatted message.