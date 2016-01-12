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

template <typename TYPE> class SimpleList;

/**
 * @class ReentrantMutex ReentrantMutex.h System/ReentrantMutex.h
 * @brief ReentrantMutex Implementation
 *
 * ReentrantMutex enables to lock some code section to avoid multiple access by several threads.
 * These mutex are reentrant.
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 */

class ReentrantMutex : public LockableObject, public std::recursive_mutex
{
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
	inline bool Lock() { return try_lock(); }

	/**
	 * @brief Lock the mutex. Deprecated, use ReentrantMutex#Lock instead
	 *
	 * Wait if the mutex is already locked, until it is unlocked, and then locks the mutex
	 * @return false if an error occured
	 */
	inline bool EnterMutex() { return Lock(); };

	/**
	 * @brief Unlock the mutex. Deprecated, use ReentrantMutex#Unlock instead
	 *
	 * Enables other clients to use the critical section protected by this mutex.
	 */
	inline bool Unlock() { unlock(); return true; }

	/**
	 * @brief Unlock the mutex. Deprecated, use ReentrantMutex#Unlock instead
	 *
	 * Enables other clients to use the critical section protected by this mutex.
	 */
	inline bool LeaveMutex() { return Unlock(); };
};

} // namespace Omiscid

#endif // __REENTRANT_MUTEX_H__

