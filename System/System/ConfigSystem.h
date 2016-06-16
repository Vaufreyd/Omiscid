/**
 * @defgroup System Layer 0 : Omiscid multiplateform System layer.
 *
 */

/**
 * @file System/Config.h
 * @ingroup System
 * @brief this file is the first file included in the System layer. it defines all mandatory includes.
 * Inludes any header file from the System layer will first include this file. Most thinks about DEBUG
 * and specific definitions for different operating systems.
 */

#ifndef __CONFIG_SYSTEM_H__
#define __CONFIG_SYSTEM_H__

#if defined _WIN32 || defined WIN32 || defined _WIN64 || defined WIN64
   /**
	* @def WIN32
	* @ingroup System
	* @brief Define the WIN32 symbol
	*
	* In order to be sure to have the correct WIN32 symbol.
	*/
	#define OMISCID_ON_WINDOWS	// OMISCID_ON_WINDOWS is more general for us
#endif


#ifdef OMISCID_ON_WINDOWS
	// To prevent the Yield definition from the winbase.h
	#undef Yield

	// To prevent warning about printf, strcpy in Visual Studio 2005, etc...
	#ifndef _CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE
	#endif

	#pragma warning(disable : 4996) // deprecated API
	#pragma warning(disable : 4100) // formal parameter not used, mostly in virtual fonction
	#pragma warning(disable : 4512) // Could not generate automatically operator= for a class
	// #pragma warning(disable : 4127)	// to remove carning about constant expression when compiling...

	// If *you* are using MFC and co...
	#ifdef USE_AFX
		#define  WINVER 0x0501
		#include "StdAfx.h"
		// To prevent problems using iostream later...
		#ifndef _IOSTREAM_
			#error "You must include '<iostream>' in your 'StdAfx.h' file."
		#endif
	#else
		#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
		#include <windows.h>
	#endif

	#include <WS2tcpip.h>
	#include <WSPiApi.h>

	#include <Winsock2.h>
	#include <process.h>		// Process action

   /**	@def FUNCTION_CALL_TYPE
	*	@ingroup System
	*	@brief DNS-SD callbacks type (WIN32 specific).
	*
	*	As WIN32 plateform supports many function call types, we must use the right
	*	one for DNS-SD callbacks. As defined in DNS-SD documentation, under WIN32 plateforms
	*	the callback type is __stdcall.	On other plateform, this define is empty.
	*/
	#define FUNCTION_CALL_TYPE __stdcall

	/** @def DEBUG
	 *	@ingroup System
	 *	@brief Define the gcc like debug symbol (WIN32 only).
	 *
	 *	In order to have portable DEBUG support, we need to define a common
	 *	debug symbol. We choose to use the debug symbol used by gcc : DEBUG.
	 */
	#if defined _DEBUG && ! defined DEBUG
		#define DEBUG	// let Visual Studio be gcc -g compliant
	#endif

	/*! @def strcasecmp
	 *	@ingroup System
	 *	@brief Define the WIN32 symbol
	 *
	 *	For multiplateform source code.
	 */

	/*! @def strncasecmp
	 *	@ingroup System
	 *	@brief Define the WIN32 symbol
	 *
	 *	For multiplateform source code.
	 */

	/*! @def _snprintf
	 *	@ingroup System
	 *	@brief Define the WIN32 symbol
	 *
	 *	For snprintf source code.
	 */
	#define strcasecmp stricmp
	#define strncasecmp strnicmp
	#define snprintf _snprintf

	/*! @def SOCKET
	 *	@ingroup System
	 *	@brief Define the SOCKET type as on WIN32 plateform.
	 *
	 *	For multiplateform source code.
	 */
	/*! @def SOCKET_ERROR
	 *	@ingroup System
	 *	@brief Define the socket error symbol as on WIN32 plateform.
	 *
	 *	For multiplateform source code.
	 */

#else
	#include <sys/types.h>
	#include <sys/time.h>

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>

   /*!	@def nullptr
	*	@ingroup System
	*	@brief define nullptr as NULL for non C++11 compiler
	*/
	#if __cplusplus < 201103L
		#ifndef nullptr
			#define nullptr NULL
		#endif
	#endif


   /*!	@def FUNCTION_CALL_TYPE
	*	@ingroup System
	*	@brief DNS-SD callbacks type (WIN32 specific).
	*
	*	As WIN32 plateform supports many function call types, we must use the right
	*	one for DNS-SD callbacks. As defined in DNS-SD documentation, under WIN32 plateforms
	*	the callback type is __stdcall.	On other plateform, this define is empty.
	*/
	#define FUNCTION_CALL_TYPE

	/*! @def SOCKET
	 *	@ingroup System
	 *	@brief Define the SOCKET type as on WIN32 plateform.
	 *
	 *	For multiplateform source code.
	 */
	/*! @def SOCKET_ERROR
	 *	@ingroup System
	 *	@brief Define the socket error symbol as on WIN32 plateform.
	 *
	 *	For multiplateform source code.
	 */
	typedef int SOCKET;
	#define SOCKET_ERROR (-1)

	/*! @def DEBUG
	 *	@ingroup System
	 *	@brief Define the gcc like debug symbol (WIN32 only).
	 *
	 *	In order to have portable DEBUG support, we need to define a common
	 *	debug symbol. We choose to use the debug symbol used by gcc : DEBUG.
	 */
	#if defined DEBUG	// This section is made only to appear in the Doxygen documentation
		#undef DEBUG
		#define DEBUG
	#endif

	/*! @def OMISCID_ON_WINDOWS
	 *	@ingroup System
	 *	@brief Define the OMISCID_ON_WINDOWS symbol
	 *
	 *	In order to be sure to have the correct WIN32 symbol.
	 */


	/*! @def strcasecmp
	 *	@ingroup System
	 *	@brief Define the WIN32 symbol
	 *
	 *	For multiplateform source code.
	 */

	/*! @def strncasecmp
	 *	@ingroup System
	 *	@brief Define the WIN32 symbol
	 *
	 *	For multiplateform source code.
	 */

	/*! @def _snprintf
	 *	@ingroup System
	 *	@brief Define the WIN32 symbol
	 *
	 *	For snprintf source code.
	 */

	#include <unistd.h>
	#include <pthread.h>
#endif

// mandatory included files. Some are included here in order to
// let us define a new/delete operator
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <exception>

// Check if DEBUG and NDEBUG are set simultaneously
#ifdef DEBUG
	#if defined NDEBUG || defined _NDEBUG
		#error "You must not define DEBUG and NDEBUG simultaneously"
	#endif
#endif

#ifdef TRACKING_MEMORY_LEAKS

	// include redefinition of new/delete operator
	#include <System/TrackingMemoryLeaks.h>

	#define OMISCID_TLM (__LINE__,__FILE__)

#else

	#define OMISCID_TLM

#endif

namespace Omiscid {


// Trace management
#ifdef OMISCID_TRACE_ENABLE
	#define OMISCID_TRACE_IS_ENABLED 1
#else
	#define OMISCID_TRACE_IS_ENABLED 0
#endif

/** @fct OmiscidError
	*  @brief used to warn messages
	*/
inline void OmiscidError(const char * format, ... )
{
	va_list args;
	va_start( args, format );
	vfprintf( stderr, format, args );
	va_end( args );
}

inline void OmiscidTrace(const char * format, ... )
{
	va_list args;
	va_start( args, format );
#ifdef DEBUG
	vfprintf( stderr, format, args );
#else
	vprintf( format, args );
#endif
	va_end( args );
}

#define OmiscidError  !(OMISCID_TRACE_IS_ENABLED) ? (void)0 : OmiscidError
#define OmiscidTrace  !(OMISCID_TRACE_IS_ENABLED) ? (void)0 : OmiscidTrace

// Message for developping purpose
#ifdef DEBUG
	#define DevOmiscidTrace OmiscidError
#else
	inline void DevOmiscidTrace( const char * format, ... ) {}
#endif

// Define an OmiscidMessage depend on DEBUG flag but always doing a trace
#ifdef DEBUG
	#define OmiscidMessage OmiscidError
#else
	#define OmiscidMessage OmiscidTrace
#endif

#ifdef OMISCID_ON_WINDOWS

// For Time function port under Windows
struct timezone {
	   int  tz_minuteswest; /* minutes W of Greenwich */
	   int  tz_dsttime;	 /* type of dst correction */
};

#endif	// ifdef OMISCID_ON_WINDOWS

/**
 * @class OmiscidSystemLayerInitClass ConsigSystem.cpp System/ConsigSystem.h
 * @brief Initialisation Object for the Omiscid Layer class
	      It can be instanciated several time without any problem
		  even if it should not be instanciated by Omiscid user (but could be done).
 *
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 */
class OmiscidSystemLayerInitClass
{
public:
	/** @brief constructor.
	 */
	OmiscidSystemLayerInitClass();

	/** @brief Virtual destructor, always.
	 */
	~OmiscidSystemLayerInitClass();

};

/** @brief External function to initialise this System layer
 */
extern void OmiscidSystemLayerInit();

/** @enum MessageOrigine
 *  @brief Define origine of messages : TDP, TCP or ShareMemory
 */
enum MessageOrigine { UnknownOrigine, FromUDP, FromTCP, FromSharedMemory };

} // namespace Omiscid

#endif	// __CONFIG_SYSTEM_H__

