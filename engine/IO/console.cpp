#pragma once
/**
	@class		IO::Console
	@copyright	See LICENCE file
*/
#include "config.h"
#include "console.h"
#include "imgui.h"
#include "callbacks.h"
#include "core/sysfunc.h"

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
	if (this->GUIConsoleOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiSetCond_FirstUseEver);

		static bool ScrollToBottom = true;

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
				const char* item = log[i].AsCharPtr();
				if (!filter.PassFilter(item))
					continue;

				//TODO: A better implementation may store a type per-item. For now just parse the text.
				ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
				if (strstr(item, "[Error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
				else if (strstr(item, "[Warning]")) col = ImColor(1.0f, 1.0f, 0.4f, 1.0f);
				else if (strstr(item, "[FATAL ERROR]")) col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
				else if (strstr(item, ">>")) col = ImColor(0.4f, 1.0f, 0.4f, 1.0f);

				ImGui::PushStyleColor(ImGuiCol_Text, col);

				//TODO: Wrapping should resume with a tab, so that continuing on a new line does not align with the time stamp, but with the message itself.
				//		This would make it a lot more readable.
				//		ex.
				/*       _______________________________________________________________________
						|18:23:43 [Message]: This is a long message that will wrap around to the |
						|					 next line. This makes the message a lot more readab-|
						|					 le than if this was not the case.					 |
						|18:24:01 [Message]: This is another message.							 |
						|________________________________________________________________________|
				*/
				ImGui::TextWrapped(item);
				ImGui::PopStyleColor();
			}

			if (ScrollToBottom)
				ImGui::SetScrollHere();

			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			if (ImGui::SmallButton("Clear Log"))
				ClearLog();

			ImGui::SameLine();
			if (ImGui::SmallButton("Auto Scroll"))
				ScrollToBottom = (ScrollToBottom ? false : true);

			ImGui::Separator();

			// Command-line
			//TODO: Implement me!!!

			//if (ImGui::InputText(">>", inputBuf, INPUTBUFSIZE * sizeof(char), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, TEXTEDITCALLBACKFUNCTION, (void*)this))
			//{
			//	char* input_end =inputBuf + strlen(inputBuf);
			//	while (input_end > inputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
			//	if (inputBuf[0])
			//		//TODO: ExecCommand Functionality
			//		//ExecCommand(inputBuf);
			//	strcpy(inputBuf, "");
			//}
			//
			//// Demonstrate keeping auto focus on the input box
			//if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
			//	ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		} 
		ImGui::End();
	}
}

void Console::Print(const Util::String& msg, LogMessageType type, bool once)
{
	//Get Time and Date
	Util::String logEntry = Core::SysFunc::GetTimeFormatted() + " ";

	switch (type)
	{
	case IO::MESSAGE:
		logEntry.Append("[Message]: ");
		break;
	case IO::INPUT:
		logEntry.Append(">> ");
		break;
	case IO::WARNING:
		logEntry.Append("[Warning]: ");
		break;
	case IO::ERROR:
		logEntry.Append("[Error]: ");
		break;
	case IO::EXCEPTION:
		logEntry.Append("[FATAL ERROR]: ");
		break;
	default:
		break;
	}

	logEntry.Append(msg.AsCharPtr());

	if (!once)
	{
		//Just append to the log
		this->log.Append(logEntry);
		if (this->nativeConsoleOpen)
		{
			std::cout << logEntry.AsCharPtr() << std::endl;
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
			std::cout << logEntry.AsCharPtr() << std::endl;
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
	//TODO: Implement me!
}

const Util::Array<Util::String> & Console::GetLog() const
{
	return this->log;
}

}
