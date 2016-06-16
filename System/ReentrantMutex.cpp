#include <System/ReentrantMutex.h>
#include <System/ElapsedTime.h>
#include <System/Thread.h>

using namespace Omiscid;

/**
	* @brief Lock the mutex.
	*
	* Wait if the mutex is already locked, until it is unlocked, and then locks the mutex
	* @return false if an error occured
	*/
bool ReentrantMutex::Lock(int wait_us /* = 0*/ )
{	
	if ( wait_us == 0 )
	{
		InternalMutex.lock();
	}
	else
	{
		Omiscid::PerfElapsedTime Time;
		while( InternalMutex.try_lock() == false )
		{
			if ( Time.Get() >= wait_us )
			{
				return false;
			}
			Omiscid::Thread::Usleep(100);
		}
	}
	return true;
}
