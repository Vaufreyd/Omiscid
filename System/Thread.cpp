/**
* @file System/Thread.cpp
* @ingroup System
* @brief Implementation of ThreadMessage and Thread classes
*/

#include <System/Thread.h>
#include <System/Portage.h>
#include <System/ElapsedTime.h>


#include <errno.h>

using namespace Omiscid;

	/** @brief Constructor
	 *
	 */
ThreadMessage::ThreadMessage()
{
	Code   = 0;
	Param1 = (void*)NULL;
	Param2 = (void*)NULL;
}

	/** @brief Constructor
	 *
	 */
ThreadMessage::ThreadMessage(int cCode, void * cParam1, void* cParam2 /* = (void*)NULL */ )
{
	Code   = cCode;
	Param1 = cParam1;
	Param2 = cParam2;
}

/** @brief Constructor
	 *
	 */
ThreadMessage::~ThreadMessage()
{
}

const ThreadMessage& ThreadMessage::operator=(const ThreadMessage& ToCopy)
{
	Code   = ToCopy.Code;
	Param1 = ToCopy.Param1;
	Param2 = ToCopy.Param2;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef DEBUG_THREAD
Thread::Thread(const SimpleString Name /* = SimpleString::EmptyString */)
#else
Thread::Thread()
#endif
{
#ifdef DEBUG_THREAD
	ThreadName = Name;
#endif
	ThreadIsRunning = false;
	StopWasAsked = false;
}

Thread::~Thread()
{
	if ( IsRunning() == true )
	{
		StopThread();
	}
}

bool Thread::StartThread(unsigned int WaitForStartCompletionInMicroSeconds /* = DEFAULT_START_THREAD_TIMEOUT (0) */)
{
	// do nothing if already running
	if ( IsRunning() == true )
	{
		return false;
	}

	ThreadIsRunning = false;
	StopWasAsked = false;

#ifdef DEBUG_THREAD
	OmiscidTrace( "StartThread : %s\n", ThreadName.GetStr() );
#endif

	MyThread.reset( new std::thread( Thread::CallRun, (void*)this ) );

	// let's it have its own life
	MyThread->detach();

	if ( WaitForStartCompletionInMicroSeconds == 0 )
	{
		PerfElapsedTime WaitedTime;
		while( IsRunning() == false )
		{
			if ( WaitedTime.Get() > WaitForStartCompletionInMicroSeconds )
			{
				return false;
			}
			Thread::Usleep(100);
		}
	}

	return true;
}

bool Thread::StopThread(int wait_ms /* = DEFAULT_THREAD_DESTRUCTOR_TIMEOUT */, bool UNUSED /* = true */  )
{
	bool ThreadStopInTime = true;

	if ( IsRunning() == true )
	{
#ifdef DEBUG_THREAD
		OmiscidTrace( "Ask %s to stop.\n", ThreadName.GetStr() );
#endif

		// Notify thread !
		StopWasAsked = true;			// Notification for pseudo active waiting
		StopWasAskedEvent.Signal();		// other notification

		// If I am myself asking to stop, flags are set, go back in the stack to terminate cleanly
		if ( GetId() == GetCallerThreadId() )
		{
			// Ok, I will stop later so, maybe not in time
			return false;
		}

		// No more unlimited wait (as c++11 do not provide way to kill thread directly
		if ( wait_ms == 0 )
		{
			// default wait to DEFAULT_MAX_THREAD_DESTRUCTOR_TIMEOUT ms.
			wait_ms = DEFAULT_MAX_THREAD_DESTRUCTOR_TIMEOUT;
		}

		ThreadStopInTime = IsEnded.Wait(wait_ms);
		if ( ThreadStopInTime == false )
		{
			// Timeout !!!
#ifdef DEBUG_THREAD
			OmiscidError( "Thread::StopThread: %s do not stop before timeout (%d).\n",  ThreadName.GetStr(), wait_ms );
#else 
			OmiscidError( "Thread::StopThread: Thread do not stop before timeout (%d).\n",  wait_ms );
#endif
		}
		else
		{
#ifdef DEBUG_THREAD
			OmiscidTrace( "Thread %s stopped.\n", ThreadName.GetStr() );
#endif
		}

	}

	// Delete it by resetting the unique ptr
	MyThread.reset();

	return ThreadStopInTime;
}

/** @brief Join the thread
 *
 */
void Thread::Join(int wait_ms /* = DEFAULT_JOIN_TIMEOUT */ )
{
	// If I am myself asking to stop, flags are set, go back in the stack to terminate cleanly
	if ( GetId() == GetCallerThreadId() )
	{
		// Ok, I will stop later so, maybe not in time
		// throw SimpleException("Thread can not join itself");
		return;
	}

	// As we detech the thread, we can not call std::thread::join
	if ( IsRunning() == true )
	{
		IsEnded.Wait(wait_ms);
	}
}

unsigned int FUNCTION_CALL_TYPE Thread::CallRun(void* ptr)
{
	Thread* t = (Thread*)ptr;

	// init thread random generator
	RandomInit();

	// Reset stat event
	t->IsEnded.Reset();

#ifdef DEBUG_THREAD
	OmiscidTrace( "%s started\n", t->ThreadName.GetStr() );
#endif

	// Do my job
	t->ThreadIsRunning = true;
	t->Run();
	t->ThreadIsRunning = false;

	// signal, my job is over
	t->IsEnded.Signal();

	return 0;
}

void Thread::Sleep(int nb_milliseconds)
{
	std::this_thread::sleep_for( std::chrono::milliseconds(nb_milliseconds) );
}

/** @brief the calling thread sleep
	* @param nb_microseconds [in] number of microseconds to sleep
	*/
void Thread::Usleep(int nb_microseconds)
{
	std::this_thread::sleep_for( std::chrono::microseconds(nb_microseconds) );
}

bool Thread::IsRunning() const
{
	return ThreadIsRunning;
}

bool Thread::StopPending() const
{
	return StopWasAsked;
}

void Thread::WaitForStop()
{
	StopWasAskedEvent.Wait(0);
}

/** @brief return an Id for the Thread
 *
 */
std::thread::id Thread::GetId()
{
	if ( MyThread.get() != nullptr )
	{
		return MyThread->get_id();
	}
	return std::thread::id();
}

/** @brief return an Id for the calling Thread
 *
 */
std::thread::id Thread::GetCallerThreadId()
{
	return std::this_thread::get_id();
}

	/** @brief Send message to a Thread
	 *
	 * Send a message to a thread
	 */
void Thread::SendMessage( int Code, void * Param1, void* Param2 /* = (void*)NULL */ )
{
	ThreadMessage MsgToSend( Code, Param1, Param2 );

	SmartLocker SL_MsgQueue(MsgQueue);

	MsgQueue.AddTail(MsgToSend);
}

/** @brief Send message to a Thread
	*
	* Send a message to a thread
	*/
void Thread::SendMessage( const ThreadMessage& MsgToSend )
{
	SmartLocker SL_MsgQueue(MsgQueue);

	MsgQueue.AddTail(MsgToSend);
}

	/** @brief Retrieve message for a thread with timeout
	 *
	 * Retrieve message for a thread
	 */
bool Thread::WaitAndGetMessage( ThreadMessage& MsgToGet, unsigned int DelayMax /* = (unsigned int)DEFAULT_MESSAGE_TIMEOUT */ )
{
	bool Done;

	SmartLocker SL_MsgQueue(MsgQueue, false);

	if ( DelayMax == 0 )
	{
		// INFINITE wait
		Done = false;
		while( true ) // was for(;;) but new version of MS compiler do no like it
		{
			// Is there a message ?
			SL_MsgQueue.Lock();
			if ( MsgQueue.GetNumberOfElements() != 0 )
			{
				MsgToGet = MsgQueue.ExtractFirst();
				Done = true;
			}
			SL_MsgQueue.Unlock();

			if ( Done == true )
			{
				return true;
			}

			// Wait
			Sleep( 10 );
		}
		// never here
		return true;
	}
	else
	{
		ElapsedTime CountWaitedTime;
		Done = false;
		while( true ) // was for(;;) but new version of MS compiler do no like it
		{
			// Is there a message ?
			SL_MsgQueue.Lock();
			if ( MsgQueue.GetNumberOfElements() != 0 )
			{
				MsgToGet = MsgQueue.ExtractFirst();
				Done = true;
			}
			SL_MsgQueue.Unlock();

			if ( Done == true )
			{
				return true;
			}

			if ( Done == true || CountWaitedTime.Get() >= DelayMax )
			{
				return Done;
			}

			Sleep( 10 );
		}
		// never here
		return true;
	}

	return false;
}

	/** @brief Get a message for a thread
	 *
	 * Get message for a thread
	 */
bool Thread::GetMessage( ThreadMessage& MsgToSend )
{
	SmartLocker SL_MsgQueue(MsgQueue);

	if ( MsgQueue.GetNumberOfElements() == 0 )
	{
		// No message
		return false;
	}

	// get first message
	MsgToSend = MsgQueue.ExtractFirst();
	return true;
}

