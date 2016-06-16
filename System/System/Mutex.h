/**
 * @file System/Mutex.h
 * @ingroup System
 * @brief Definition of Mutex class
 */

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <System/ConfigSystem.h>
#include <System/LockManagement.h>


#include <mutex>

namespace Omiscid {

/**
 * @class Mutex Mutex.cpp System/Mutex.h
 * @brief Mutex Implementation
 *
 * Mutex enables to lock some code section to avoid multiple access by several threads.
 * The mutex are not reentrant.
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 * @author Sebastien Pesnel
 */
class Mutex : public LockableObject// , public std::mutex
{
protected:
	std::mutex InternalMutex;

public:
	/** @brief Constructor */
	Mutex() {}

	/** @brief Destructor */
	virtual ~Mutex() {}

	/**
	 * @brief Lock the mutex.
	 *
	 * Wait if the mutex is already locked, until it is unlocked, and then locks the mutex
	 * @return false if an error occured
	 */
	bool Lock(int wait_us = 0);

	/**
	 * @brief Lock the mutex. Deprecated, use Mutex#Lock instead.
	 *
	 * Wait if the mutex is already locked, until it is unlocked, and then locks the mutex
	 * @return false if an error occured
	 */
	inline bool EnterMutex(int wait_us = 0) { return Lock(wait_us); };

	/**
	 * @brief Unlock the mutex
	 *
	 * Enables other clients to use the critical section protected by this mutex.
	 */
	inline bool Unlock() { InternalMutex.unlock(); return true; }

	/**
	 * @brief Unlock the mutex. Deprecated, use Mutex#Unlock instead.
	 *
	 * Enables other clients to use the critical section protected by this mutex.
	 */
	inline bool LeaveMutex() { return Unlock(); };

private:

#ifdef DEBUG_MUTEX_OWNER
	unsigned int OwnerId;
	unsigned int PreviousOwnerId;
#endif

};

} // namespace Omiscid

#endif // __MUTEX_H__

