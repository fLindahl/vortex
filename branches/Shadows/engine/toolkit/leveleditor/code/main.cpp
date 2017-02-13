//------------------------------------------------------------------------------
// main.cc
// (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"

#define _LEVELEDITOR

int
main(int argc, const char** argv)
{
	LevelEditor::Application app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
}