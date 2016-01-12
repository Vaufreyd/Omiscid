/**
 * @file System/MemoryBuffer.h
 * @ingroup System
 * @brief interface for TemporaryMemoryBuffer and MemoryBufferException classes.
 */

#ifndef __MEMORY_BUFFER_H__
#define __MEMORY_BUFFER_H__

#include <System/ConfigSystem.h>
#include <System/SimpleException.h>

#include <memory>

namespace Omiscid {

class MemoryBuffer : public std::unique_ptr<char>
{
public:
	MemoryBuffer();

	MemoryBuffer( size_t SizeOfBuffer );

	virtual ~MemoryBuffer() {}

	void SetNewBufferSize( size_t SizeOfBuffer, bool KeepData = false );

	const char* GetBuffer() const
	{
		return (const char*)get();
	}

	size_t GetLength() const
	{
		return LogicalBufferSize;
	}

	template<typename CAST_TYPE>
	operator CAST_TYPE*()
	{
		return (CAST_TYPE*)(get());
	}
	
	template<typename CAST_TYPE>
	operator const CAST_TYPE*() const
	{
		return (const CAST_TYPE*)(get());
	}

private:
	size_t LogicalBufferSize;
	size_t RealBufferSize;
};




} // namespace Omiscid

#endif // __MEMORY_BUFFER_H__

