/**
 * @file ReentrantMutex.h
 * @ingroup System
 * @brief Definition of ReentrantMutex class
 */

#ifndef __REENTRANT_MUTEX_H__
#define __REENTRANT_MUTEX_H__

#include <System/ConfigSystem.h>
#include <System/LockManagement.h>

#include <mutex>

namespace Omiscid {

/**
 * @class ReentrantMutex ReentrantMutex.h System/ReentrantMutex.h
 * @brief ReentrantMutex Implementation
 *
 * ReentrantMutex enables to lock some code section to avoid multiple access by several threads.
 * These mutex are reentrant.
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 */

class ReentrantMutex : public LockableObject 
{
protected:
	std::recursive_mutex InternalMutex;

public:
	/** @brief Constructor */
	ReentrantMutex() {}

	/** @brief Destructor */
	virtual ~ReentrantMutex() {}

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
	 * @brief Unlock the mutex. Deprecated, use ReentrantMutex#Unlock instead
	 *
	 * Enables other clients to use the critical section protected by this mutex.
	 */
	inline bool LeaveMutex() { return Unlock(); };
};

} // namespace Omiscid

#endif // __REENTRANT_MUTEX_H__

