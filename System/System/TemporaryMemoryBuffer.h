/**
 * @file System/TemporaryMemoryBuffer.h
 * @ingroup System
 * @brief interface for TemporaryMemoryBuffer (for backward compatibility).
 */

#ifndef __TEMPORARY_MEMORY_BUFFER_H__
#define __TEMPORARY_MEMORY_BUFFER_H__

#include <System/ConfigSystem.h>
#include <System/MemoryBuffer.h>

namespace Omiscid {

class TemporaryMemoryBuffer : public MemoryBuffer
{
public:
	TemporaryMemoryBuffer() : MemoryBuffer() {};
	TemporaryMemoryBuffer( size_t SizeOfBuffer ) : MemoryBuffer(SizeOfBuffer) {};

	virtual ~TemporaryMemoryBuffer() {};
};

} // namespace Omiscid

#endif // __TEMPORARY_MEMORY_BUFFER_H__
