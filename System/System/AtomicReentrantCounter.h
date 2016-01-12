/**
 * @file System/System/AtomicReentrantCounter.h
 * @ingroup System
 * @brief Definition of Mutexed Counter class using reentrant mutex
 */

#ifndef __ATOMIC_REENTRANT_COUNTER_H__
#define __ATOMIC_REENTRANT_COUNTER_H__

#include <System/ConfigSystem.h>
#include <atomic>

namespace Omiscid {

/**
 * @class AtomicReentrantCounter AtomicReentrantCounter.cpp System/AtomicReentrantCounter.h
 * @brief Since atomic<T> exists, keep for backward compatibility.
 */
typedef std::atomic_int_fast32_t  AtomicReentrantCounter;

} // namespace Omiscid

#endif // __ATOMIC_REENTRANT_COUNTER_H__

