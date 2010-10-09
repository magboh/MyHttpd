/*
 * bytebuffer.cpp
 *
 *  Created on: Oct 9, 2010
 *      Author: magnus
 */
#include <string.h>

#include "bytebuffer.h"

ByteBuffer::ByteBuffer(size_t size)
{
	mBuffer = new unsigned char[size];
	mSize = size;
}

ByteBuffer::~ByteBuffer()
{
	delete mBuffer;
	mBuffer = 0;
	mSize=0;

}

size_t ByteBuffer::GetSpaceLeft()  const
{
	return mSize - mPosition ;
}

bool ByteBuffer::Add(const char *data, size_t size)
{
	return Add((unsigned char *)data, size);
}

bool ByteBuffer::Add(unsigned char *data, size_t size)
{
	size_t spaceLeft = GetSpaceLeft();
	bool bOk=true;

	if (spaceLeft>=size)
	{
		memcpy(mBuffer+mPosition,data,size);
		mPosition+=size;
	}
	else
	{
		bOk=false;
	}

	return bOk;
}

size_t ByteBuffer::GetSize()  const
{
	return mSize;
}

void ByteBuffer::Clear()
{
	memset(mBuffer,0x00,mSize);
}

unsigned char* ByteBuffer::GetBuffer() const
{
	return mBuffer;
}
