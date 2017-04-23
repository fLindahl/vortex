#pragma once
/**
	@class		IO::Console

	Vortex Console Singleton class. 
	Used for printing messages, warnings, errors but also accepting input from users.
	Only one console can exist at any time.
	
	@copright	See LICENCE file
*/
#include "core/singleton.h"
#include "core/types.h"
#include "foundation/util/string.h"
#include "foundation/util/fixedarray.h"

#define INPUTBUFSIZE 256

namespace IO
{
	/**
		These are all the types of messages that can be printed in the console
		Depending on the type of the message, they are sorted into different categories by appending a type prefix to the message
	*/
	enum LogMessageType
	{
		///Just plain white text. Adds [Message] prefix to the log message.
		MESSAGE,	
		///User Input text. Appends nothing to the message.
		INPUT,		
		///Warning text. Adds [Warning] to the message.
		WARNING,	
		///Error message. Adds [Error] to the message.
		ERROR,		
		///Exception. Adds [FATAL ERROR] to the messsage. These are only used when the application encounters an assertion and needs to abort
		EXCEPTION	
	};

class Console
{
__DeclareSingleton(Console)
public:
	struct LogEntry
	{
		///Using a string for timestamp, so that we can just show it right away
		Util::String timestamp;
		///Message type. This will be used for setting the color of the message
		LogMessageType type;
		///Message string.
		Util::String msg;
	};

	///Update function. Called from main application loop. This also draws the console if necessary
	void Update();

	///Works similarly to the _printf function, except it accepts a string instead of a char pointer. 
	///It also accepts an optional parameter called "once" which, if set to true, will only print the message once within the context it's created. This can be useful for warning messages that could possibly clog up the console really quickly.
	void Print(const Util::String& msg, LogMessageType type = MESSAGE, bool once = false);
	
	///Calls print function with a formatted string. NOTE: This always outputs as LogMessageType::MESSAGE
	void Printf(const char* msg, ...);

	///Calls print function with a formatted string. NOTE: This always outputs as LogMessageType::WARNING
	void PrintfWarning(const char* msg, ...);

	///Calls print function with a formatted string. NOTE: This always outputs as LogMessageType::ERROR
	void PrintfError(const char* msg, ...);
	
	///Shows the console as an ImGui Window. Accepts user input.
	///Note: This can eat performance if the log is abnormally large. In those cases it might be better to use the native console, as it does not require rendering...
	void Show();
	///Hides the console ImGui Window
	void Hide();

	///Brings up an native platform external window console (ex. CMD, terminal). This console does not accept input.
	///This is very useful when stepping through an application because it writes directly to the console when a
	/// print statement is encountered (GUI waits until the next frame).
	///NOTE: Opening this might halt the application for a moment if the log is big as it has to print every thing.
	void OpenNativeConsole();
	///Closes native platform console
	void CloseNativeConsole();

	///Clears the entire log and the reccent messages list
	void ClearLog();

	///Saves the console into a .log file. Argument "filename" can be used to specify a specific log name. If filename is left empty, a generic name will be generated.
	void SaveLog(Util::String filename = "") const;

	///Return a const reference to the entire log content
	const Util::Array<LogEntry>& GetLog() const;
private:

	bool nativeConsoleOpen;
	bool GUIConsoleOpen;
	
	char inputBuf[INPUTBUFSIZE];

	///Contains the entire log. This can be filtered because we append each message into it's own node in the array.
	Util::Array<LogEntry> log;

	///This Array and is used to check if a message has been printed recently.
	///recentMessages list contains a maximum n messages and a message can only exist in the list for a maximum of x seconds
	///integer is HashCode and double is time when the message will be removed
	Util::Array<Util::Pair<int, double>> recentMessages;

	const char* LogEntryTypeAsCharPtr(const LogMessageType& type) const;
};

}
