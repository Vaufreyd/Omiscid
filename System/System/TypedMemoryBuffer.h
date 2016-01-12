/**
 * @file System/TemporaryMemoryBuffer.h
 * @ingroup System
 * @brief interface for TemporaryMemoryBuffer and MemoryBufferException classes.
 */

#ifndef __TYPED_MEMORY_BUFFER_H__
#define __TYPED_MEMORY_BUFFER_H__

#include <System/ConfigSystem.h>
#include <System/MemoryBuffer.h>

namespace Omiscid {

template <typename TYPE>
class TypedMemoryBuffer : protected MemoryBuffer
{
public:
	TypedMemoryBuffer()
	{
		NumberOfElements = 0;
	}

	TypedMemoryBuffer( size_t NumberOfElementsInBuffer ) : MemoryBuffer( sizeof(TYPE)*NumberOfElementsInBuffer )
	{
		NumberOfElements = NumberOfElementsInBuffer;
	}

	virtual ~TypedMemoryBuffer()
	{
	}

	void SetNewNumberOfElementsInBuffer( size_t NumberOfElementsInBuffer, bool KeepData = false )
	{
		SetNewBufferSize( NumberOfElementsInBuffer*sizeof(TYPE), KeepData );
		NumberOfElements = NumberOfElementsInBuffer;
	}

	size_t GetNumberOfElements() const
	{
		return NumberOfElements;
	}

	operator TYPE*()
	{
		return (TYPE*)(get());
	}
	
	operator const TYPE*() const
	{
		return (const TYPE*)(get());
	}

	TYPE& operator[](size_t i)
	{
		return GetReferenceOnElement(i);
	}

	TYPE& operator[](size_t i) const
	{
		return GetReferenceOnElement(i);
	}

protected:
	TYPE& GetReferenceOnElement( size_t i )
	{
		if ( i >= NumberOfElements )
		{
			throw std::exception();
		}
		TYPE * pArr = (TYPE*)get();
		return pArr[i];
	}

	size_t NumberOfElements;
};

} // namespace Omiscid

#endif // __TYPED_MEMORY_BUFFER_H__

