#include <System/Message.h>

#include <System/Portage.h>
#include <string.h>

#include <mutex>

using namespace Omiscid;


Message::Message(size_t size)
{
  origine = UnknownOrigine;

  if ( size > 0 )
  {
	  // Allocate size and set buffer to it
	  SetNewBufferSize(size);
	  buffer = MemoryBuffer::GetBuffer();
	  len = MemoryBuffer::GetLength();
  }
}

Message::Message(const Message& ToCopy)
{
	size_t len = ToCopy.GetLength();
	SetNewBufferSize(len);

	memcpy((void*)ToCopy.GetBuffer(), GetBuffer(), len);
	buffer = MemoryBuffer::GetBuffer();
	len = MemoryBuffer::GetLength();
	origine = ToCopy.GetOrigine();
	pid = ToCopy.GetPeerId();
	mid = ToCopy.GetMsgId();
}

const char * Message::GetBuffer() const
{
	return buffer;
}

size_t Message::GetLenght() const
{
	return len;
}

MessageOrigine Message::GetOrigine() const
{
	return origine;
}

unsigned int Message::GetPeerId() const
{
	return pid;
}

unsigned int Message::GetMsgId() const
{
	return mid;
}

  /** @brief ToString
   *
   * Generate a description of the message
   */
SimpleString Message::ToString(unsigned int MaxBufferLenToWrite /* default = 20, min value = 0 max value is 256 */) const
{
	MemoryBuffer TmpBuffer(300);

	SimpleString TmpMessage = "Message ";
	TmpMessage	 += mid;
	TmpMessage	 += " (";

	size_t len = GetLength();

	if ( len < 1024 )	// less that 1 KB
	{
		TmpMessage += len;
		TmpMessage += "bytes)";
	}
	else if ( len >= 1024 && len < (1024*1024) ) // from 1KB to 1Mo
	{
		// compute size in ko
		*((char*)TmpBuffer) = '\0';	// Empty string
		snprintf( (char*)TmpBuffer, TmpBuffer.GetLength(), "%.2f Kbytes)", ((float)len)/1024.f );
		TmpMessage += (char*)TmpBuffer;
	}
	else	// over 1 Mo
	{
		// compute size in ko
		*((char*)TmpBuffer) = '\0';	// Empty string
		snprintf( (char*)TmpBuffer, TmpBuffer.GetLength(), "%.2f Mbytes)", ((float)len)/(1024.0f*1024.0f) );
		TmpMessage += (char*)TmpBuffer;
	}

	if ( origine == FromUDP )
	{
		TmpMessage += " on UDP from ";
	}
	else
	{
		TmpMessage += " on TCP from ";
	}

#if 0
	// Was
	TmpMessage += ComTools::PeerIdAsString();
#else
	(*(char*)TmpBuffer) = '\0';
	sprintf( (char*)TmpBuffer, "%8.8x", pid );

	TmpMessage += (char*)TmpBuffer;
#endif

	if ( MaxBufferLenToWrite == 0 || MaxBufferLenToWrite > 256 )
	{
		TmpMessage += "\n";
	}
	else
	{
		SimpleString Format = ":\n%";
		Format += MaxBufferLenToWrite;
		Format += ".";
		Format += MaxBufferLenToWrite;
		Format += "s\n";
		*((char*)TmpBuffer) = '\0';	// Empty string
		snprintf( (char*)TmpBuffer, TmpBuffer.GetLength(), (const char*)Format.GetStr(), GetBuffer() );
		TmpMessage += (char*)TmpBuffer;
	}

	return TmpMessage;
}

