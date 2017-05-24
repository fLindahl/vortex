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

	private:
		std::ofstream outFile;

	};

	Stream::Stream()
	{
	}

	Stream::~Stream()
	{
	}

	inline void Stream::Open(const char * filename)
	{
		this->outFile.open(filename);
	}

	inline void Stream::Close()
	{
		this->outFile.close();
	}

	inline void Stream::operator<<(int i)
	{
		outFile << i;
	}
}