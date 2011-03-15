/***************************************************************************
 *      MyHTTPd
 *
 *      Tue, 15 Mar 2011 22:16:12 +0100
 *      Copyright 2011 Magnus Bohman
 *      magnus.bohman@gmail.com
 ***************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, US.
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
	memset(mBuffer,0x00,mSize-1);
	mPosition = 0 ;
}

const char* ByteBuffer::GetBuffer() const
{
	return mBuffer;
}

void ByteBuffer::Remove(unsigned int size)
{
	if (size >= mPosition)
		Clear();
	else
	{
		memcpy(mBuffer,mBuffer+size,mPosition-size);
		mPosition -= size;
	}

}
