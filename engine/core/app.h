#pragma once
//------------------------------------------------------------------------------
/**
	Base class for applications, override this class
	and use it as an entry point for your game.
	
	@copyright	See LICENSE file
*/
//------------------------------------------------------------------------------
#include <GL/glew.h>
namespace Core
{
class App
{
public:
	/// constructor
	App();
	/// destructor
	~App();

	/// open application
	virtual bool Open();
	/// perform per-frame loop
	virtual void Run();
	/// stop application
	virtual void Close();
	/// kill application
	virtual void Exit();

protected:
	bool isOpen;
};
} // namespace App