/*
 * bytebuffer.h
 *
 *  Created on: Oct 9, 2010
 *      Author: magnus
 */

#ifndef BYTEBUFFER_H_
#define BYTEBUFFER_H_
#include <unistd.h>

class ByteBuffer
{
public:
	explicit ByteBuffer(size_t sizeGrow);
	virtual ~ByteBuffer();
	void Clear();
	bool Add(unsigned char* data, size_t size);
	bool Add(const char *data, size_t size);

	size_t GetSpaceLeft() const;
	size_t GetSize() const;
	unsigned char* GetBuffer()  const;
private:
	unsigned char* mBuffer;
	size_t mSize;
	unsigned int mPosition;
};

#endif /* BYTEBUFFER_H_ */
