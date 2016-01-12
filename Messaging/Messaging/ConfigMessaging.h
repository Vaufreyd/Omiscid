/**
 * @defgroup Messaging : Omiscid serialization
 *
 */

/**
 * @file Messaging/ConfigMessaging.h
 * @ingroup Messaging
 * @brief this file is the first header file for the structured message support
 */

#include <System/ConfigSystem.h>

// Desable Warning C4290: Visual C++ does not implement checked exceptions,
// C4290 is just informing you that other exceptions may still be throw from
// these functions
#ifdef OMISCID_ON_WINDOWS
#pragma warning(disable: 4290)
// Warning about multiple copy constructors (C4521)
#pragma warning(disable: 4521)
#endif
