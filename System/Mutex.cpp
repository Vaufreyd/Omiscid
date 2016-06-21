#include <System/Mutex.h>
#include <System/ElapsedTime.h>
#include <System/Thread.h>

using namespace Omiscid;

/**
	* @brief Lock the mutex.
	*
	* Wait if the mutex is already locked, until it is unlocked, and then locks the mutex
	* @return false if an error occured
	*/
bool Mutex::Lock(int wait_us /*= 0*/)
{	
	try
	{
		if ( wait_us == 0 )
		{
			while( InternalMutex.try_lock() == false )
			{
				Omiscid::Thread::Usleep(100);
			}
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
	}
	catch ( std::exception & )
	{
		return false;
	}

	return true;
}

