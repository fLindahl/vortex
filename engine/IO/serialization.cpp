#include "config.h"
#include "serialization.h"


namespace IO
{
    Stream::Stream()
	{
	}

	Stream::~Stream()
	{
	}

	void Stream::Open(const char * filename)
	{
		this->outFile.open(filename);
	}

	void Stream::Close()
	{
		this->outFile.close();
	}

	void Stream::operator<<(int i)
	{
		outFile << i;
	}

    void Stream::operator<<(const Attr<int>& attr)
	{
		_printf("Attribute Value %i", attr.value);
	}
}