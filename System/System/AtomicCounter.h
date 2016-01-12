/**
 * @file System/System/AtomicCounter.h
 * @ingroup System
 * @brief Definition of AtomicCounter class
 */

#ifndef __ATOMIC_COUNTER_H__
#define __ATOMIC_COUNTER_H__

#include <System/ConfigSystem.h>
#include <atomic>

namespace Omiscid {

/**
 * @class AtomicCounter AtomicCounter.cpp System/AtomicCounter.h
 * @brief Since atomic<T> exists, keep for backward compatibility.
 *
 */
typedef std::atomic_int_fast32_t AtomicCounter;


} // namespace Omiscid

#endif // __ATOMIC_COUNTER_H__

