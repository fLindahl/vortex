#pragma once
/**
	@class	IO::Stream

	Used for writing and reading from files.
	This class essentialy wrapes std::fstream and provides ability to write/read engine specific types.
	
	@copyright	See LICENSE file
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "tinyxml2.h"
#include <fstream>

template<class TYPE>
class Attr
{
public:
	Attr() {}
	~Attr() {}

	Util::String varName;
	TYPE value;
};

namespace IO
{
	class Stream
	{
	public:
		Stream();
		~Stream();

		void Open(const char* filename);
		void Close();

		void operator<<(int);
		void operator<<(const Attr<int>& attr);

	private:
		std::ofstream outFile;

	};
}