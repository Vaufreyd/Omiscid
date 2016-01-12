/**
* @file System/MemoryBuffer.cpp
* @ingroup System
* @brief Implementation of MemoryBuffer and MemoryBufferException classes.
*/

#include <System/MemoryBuffer.h>

#include <string.h>

using namespace Omiscid;

// class for Allocate/Unallocation memory buffer
MemoryBuffer::MemoryBuffer()
{
	RealBufferSize = LogicalBufferSize = 0;
}

// class for Allocate/Unallocation memory buffer
MemoryBuffer::MemoryBuffer( size_t SizeOfBuffer )
{
	RealBufferSize = LogicalBufferSize = 0;
	
	reset( new OMISCID_TLM char[SizeOfBuffer+1] );	// +1 to prevent buffer overflow using snprintf function for instance
	
	// if no exception were thrown, set buffer size
	RealBufferSize = LogicalBufferSize = SizeOfBuffer;
}

void MemoryBuffer::SetNewBufferSize( size_t NewSizeOfBuffer, bool KeepData /* = false */ )
{
	if ( NewSizeOfBuffer > LogicalBufferSize && KeepData == true )
	{
		throw std::exception();
	}

	// If buffer is not large enougth, get a new buffer
	if ( NewSizeOfBuffer > RealBufferSize )
	{
		// If we do not want to keep data or if there is no data to keep
		if ( KeepData == false || LogicalBufferSize == 0 )
		{
			// simply destroy previous buffer and replace it by new one
			reset( new OMISCID_TLM char[NewSizeOfBuffer+1] ); // +1 to prevent going out of buffer
			
			// here, without exception, new size is set else previous size and buffer are kept
			LogicalBufferSize = NewSizeOfBuffer;
		}
		else
		{
			// Allocate bigger buffer
			char * TmpBuf = new OMISCID_TLM char[NewSizeOfBuffer+1];
			
			// Copy data
			memcpy( TmpBuf, get(), LogicalBufferSize );

			// replace Current buffer by the new one
			reset( TmpBuf );
		}

		// Set the new real size of the buffer because new buffer is larger
		RealBufferSize = NewSizeOfBuffer;
	}

	// In all case, the new size if Set
	LogicalBufferSize = NewSizeOfBuffer;
}
