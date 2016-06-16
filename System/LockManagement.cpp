// LockableObject.cpp: implementation of the  class.
//
//////////////////////////////////////////////////////////////////////

#include <System/LockManagement.h>
// #include <System/Thread.h>

using namespace Omiscid;

	/** @brief Constructor
	 *
	 */
SmartLocker::SmartLocker( LockableObject& LockableObjectToManage, bool LockAtInit /* = true */ )
	: ManagedLoackableObject(LockableObjectToManage)
{
	// Initiate the LockCount
	LockCount = 0;

	// Here, we are ready to work
	if ( LockAtInit == true )
	{
		Lock();
	}
}

	/** @brief Constructor
	 *
	 */
SmartLocker::SmartLocker( const LockableObject& LockableObjectToManage, bool LockAtInit /* = true */ )
	: ManagedLoackableObject((LockableObject&)LockableObjectToManage)
{
	// Initiate the LockCount
	LockCount = 0;

	// Here, we are ready to work
	if ( LockAtInit == true )
	{
		Lock();
	}
}


	/** @brief Desonstructor
	 *
	 */
SmartLocker::~SmartLocker()
{
	if ( LockCount > 0 )
	{
		// OmiscidError( "SmartLocker::SmartLocker~: warning object not unlock before exiting the scope\n" );
		while( LockCount > 0 )
		{
			ManagedLoackableObject.Unlock();
			LockCount--;
		}
	}
}

	/** @brief Function used to lock the LockableObject object
	 *
	 */
bool SmartLocker::Lock(int wait_us /* = 0 */)
{
	bool res = ManagedLoackableObject.Lock(wait_us);
	if ( res == true )
	{
		LockCount++;
	}
	return true;
}

	/** @brief Function used to unlock the LockableObject object
	 *
	 */
bool SmartLocker::Unlock()
{
	if ( LockCount == 0 )
	{
		OmiscidError( "SmartLocker::Unlock : the object is not lock." );
		return false;
	}
	if ( ManagedLoackableObject.Unlock() == true )
	{
		LockCount--;
		return true;
	}
	return false;
}

