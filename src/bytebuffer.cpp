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
	mBuffer = new char[size];
	mSize = size;
	mPosition=0;
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
	return Add((char *)data, size);
}

bool ByteBuffer::Add(char *data, size_t size)
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
size_t ByteBuffer::GetUsage() const
{
	return mPosition;
}
void ByteBuffer::Clear()
{
	memset(mBuffer,0x00,mSize);
	mPosition = 0 ;
}

const char* ByteBuffer::GetBuffer() const
{
	return mBuffer;
}

void ByteBuffer::Remove(int size)
{
	if (size >= mPosition)
		Clear();
	else
	{
		memcpy(mBuffer,mBuffer+size,mPosition-size);
		mPosition -= size;
	}

}
