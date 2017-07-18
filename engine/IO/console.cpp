/**
	@class		IO::Console
	@copyright	See LICENCE file
*/
#include "config.h"
#include "console.h"
#include "imgui.h"
#include "callbacks.h"
#include "core/sysfunc.h"
//#include <iomanip>
#include <ctime>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define RECENTMESSAGESCAPACITY 24
#define MAXLIFETIME 60.0

namespace IO
{

Console::Console() :
		log(),
		recentMessages(),
		GUIConsoleOpen(false),
		nativeConsoleOpen(false)
{
	//Empty
}

void Console::Update()
{		
	static bool ScrollToBottom = true;

	if (this->GUIConsoleOpen)
	{

		ImGui::Begin("Console", &this->GUIConsoleOpen, 0);
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			static ImGuiTextFilter filter;
			filter.Draw("Filter", 180);
			ImGui::PopStyleVar();
			ImGui::Separator();

			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Save Log..."))
					this->SaveLog();
				if (ImGui::Selectable("Clear"))
					ClearLog();
				ImGui::EndPopup();
			}

			// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
			// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
			// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
			// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
			//     ImGuiListClipper clipper(Items.Size);
			//     while (clipper.Step())
			//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			// However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
			// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
			// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
			// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			for (int i = 0; i < log.Size(); i++)
			{
				const char* item = log[i].msg.AsCharPtr();
				const char* timestamp = log[i].timestamp.AsCharPtr();

				//Filter on both time, prefix and entry
				if (!filter.PassFilter(item) && !filter.PassFilter(timestamp) && !filter.PassFilter(this->LogEntryTypeAsCharPtr(log[i].type)))
					continue;

				ImColor col;
				switch (log[i].type)
				{
				case IO::MESSAGE:
					col = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				case IO::INPUT:
					col = ImColor(0.4f, 1.0f, 0.4f, 1.0f);
					break;
				case IO::WARNING:
					col = ImColor(1.0f, 1.0f, 0.4f, 1.0f);
					break;
				case IO::ERROR:
					col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
					break;
				case IO::EXCEPTION:
					col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				default:
					break;
				}

				static const ImColor timeColor = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

				//Start by printing timestamp.
				ImGui::PushStyleColor(ImGuiCol_Text, timeColor);
				ImGui::TextUnformatted(timestamp);
				ImGui::PopStyleColor();

				ImGui::PushStyleColor(ImGuiCol_Text, col);
				ImGui::SameLine();
				//Print message prefix
				ImGui::TextUnformatted(this->LogEntryTypeAsCharPtr(log[i].type));
				ImGui::SameLine();
				//Print log entry
				ImGui::TextWrapped(item);
				ImGui::PopStyleColor();
			}

			if (ScrollToBottom)
				ImGui::SetScrollHere();

			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			// Command-line / Input ----------------------------------------------------

			auto callback = [](ImGuiTextEditCallbackData* data) { return 1; };

			if (ImGui::InputText("|", inputBuf, INPUTBUFSIZE * sizeof(char), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, callback, (void*)this))
			{
				char* input_end = inputBuf + strlen(inputBuf);
				while (input_end > inputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
				if (inputBuf[0])
				{
					this->Print(inputBuf, IO::LogMessageType::INPUT);

					//TODO: ExecCommand Functionality
					//ExecCommand(inputBuf);
				}
				strcpy(inputBuf, "");
			}

			//keeping auto focus on the input box
			if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			ImGui::SameLine();
			ImGui::PushItemWidth(-140);
			if (ImGui::SmallButton("Auto Scroll"))
				ScrollToBottom = (ScrollToBottom ? false : true);

			ImGui::PopItemWidth();
			ImGui::Separator();
		}
		ImGui::End();
	}
}

void Console::Print(const Util::String& msg, LogMessageType type, bool once)
{
	//Get Time and Date
	Console::LogEntry logEntry;
	logEntry.type = type;
	logEntry.timestamp = Core::SysFunc::GetTimeFormatted() + " ";

	logEntry.msg.Append(msg.AsCharPtr());

	if (!once)
	{
		//Just append to the log
		this->log.Append(logEntry);
		if (this->nativeConsoleOpen)
		{
			std::cout << logEntry.timestamp.AsCharPtr() << this->LogEntryTypeAsCharPtr(type) << logEntry.msg.AsCharPtr() << std::endl;
		}
	}
	else
	{
		// We need to check if the message has been printed recently
		int hashcode = msg.HashCode();
		for (int i = 0; i < this->recentMessages.Size(); ++i)
		{
			Util::Pair<int, double>& pair = this->recentMessages[i];

			if (pair.Key() == hashcode)
			{
				//Remove from hashtable then print the message if the message in the list is too old
				if (pair.Value() <= glfwGetTime())
				{
					this->recentMessages.EraseIndex(i);
					//We need to print this message again because the existing one is too old.
					break;
				}
				else
				{
					//No need to print the message
					return;
				}
			}
		}
		
		
		//If the hashtable is full, we need to reserve a new spot for our message.
		this->log.Append(logEntry);
		if (this->nativeConsoleOpen)
		{
			std::cout << logEntry.timestamp.AsCharPtr() << this->LogEntryTypeAsCharPtr(type) << logEntry.msg.AsCharPtr() << std::endl;
		}

		if (this->recentMessages.Size() == RECENTMESSAGESCAPACITY)
		{
			//Find the oldest message and delete it
			this->recentMessages.EraseIndex(0);
		}

		this->recentMessages.Append(Util::Pair<int, double>(hashcode, glfwGetTime() + MAXLIFETIME));
	}
}

void Console::Printf(const char * msg, ...)
{
	va_list argList;
	va_start(argList, msg);
	
	Util::String str;
	str.FormatArgList(msg, argList);
	
	this->Print(str, IO::MESSAGE, true);
	
	va_end(argList);
}

void Console::PrintfWarning(const char * msg, ...)
{
	va_list argList;
	va_start(argList, msg);

	Util::String str;
	str.FormatArgList(msg, argList);

	this->Print(str, IO::WARNING);

	va_end(argList);
}

void Console::PrintfError(const char * msg, ...)
{
	va_list argList;
	va_start(argList, msg);

	Util::String str;
	str.FormatArgList(msg, argList);

	this->Print(str, IO::ERROR);

	va_end(argList);
}

void Console::Show()
{
	this->GUIConsoleOpen = true;
}

void Console::Hide()
{
	this->GUIConsoleOpen = false;
}

void Console::OpenNativeConsole()
{
	this->nativeConsoleOpen = true;
}

void Console::CloseNativeConsole()
{
	this->nativeConsoleOpen = false;
}

void Console::ClearLog()
{
	this->log.Clear();
	this->recentMessages.Clear();
}

void Console::SaveLog(Util::String filename) const
{
	if(filename.IsEmpty())
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		filename = "console_";
		filename.AppendInt(tm.tm_year - 100);
		filename.Append("-");
		filename.AppendInt(tm.tm_mon);
		filename.Append("-");
		filename.AppendInt(tm.tm_mday);
		filename.Append("_");
		filename.AppendInt(tm.tm_hour);
		filename.Append("-");
		filename.AppendInt(tm.tm_min);
		filename.Append("-");
		filename.AppendInt(tm.tm_sec);
		filename.Append(".log");
	}
	else
	{
		if(!filename.CheckFileExtension("log"))
		{
			_warning("Log file-extension is not \".log\"!");
			_printf("Changing log file-extension to \".log\"...");
			filename.ChangeFileExtension("log");		
		}
	}
	
	std::ofstream file;
	file.open(filename.AsCharPtr());
	
	for (size_t i = 0; i < this->log.Size(); i++)
	{
		file << this->log[i].timestamp.AsCharPtr() << this->LogEntryTypeAsCharPtr(this->log[i].type) << this->log[i].msg.AsCharPtr() << std::endl;
	}	
	file.close();

	_printf("Created log %s", filename.AsCharPtr());
	
	return;
}

const Util::Array<Console::LogEntry> & Console::GetLog() const
{
	return this->log;
}

const char* Console::LogEntryTypeAsCharPtr(const LogMessageType& type) const
{
	//static const prefixes that are appended to messages. Doing this saves a ton of memory.
	static const Util::String prefix_message =   "[Message]: ";
	static const Util::String prefix_input =     ">> ";
	static const Util::String prefix_warning =   "[Warning]: ";
	static const Util::String prefix_error =     "[ Error ]: ";
	static const Util::String prefix_exception = "[FATAL ERROR]: ";

	switch (type)
	{
	case IO::MESSAGE:
		return prefix_message.AsCharPtr();
	case IO::INPUT:
		return prefix_input.AsCharPtr();
	case IO::WARNING:
		return prefix_warning.AsCharPtr();
	case IO::ERROR:
		return prefix_error.AsCharPtr();
	case IO::EXCEPTION:
		return prefix_exception.AsCharPtr();
	default:
		return nullptr;
	}
}

}
