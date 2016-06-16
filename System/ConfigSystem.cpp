

#include <System/ConfigSystem.h>
#include <System/Mutex.h>
#include <System/Portage.h>
#include <System/Socket.h>

using namespace Omiscid;

// Initialise All stuff needed by the System Layer

// static Objects to prevent multiple access
// We do not want to include Mutex in ConfigSystem.h
Mutex OmiscidSystemLayerInitClassInitMutex;					/*!< A Mutex object to prevent multiple parallel modifications of this layer. */
unsigned int OmiscidSystemLayerInitClassInstanceCount = 0;	/*!< Instance counter */

// Function to do mandatory initialisation

// Init Socket
static void	OmiscidSocketInit()
{
#ifdef OMISCID_ON_WINDOWS
	// Start Windows Socket subsystem
	WORD wVersionRequested;
	WSADATA wsaData;

 	wVersionRequested = MAKEWORD( 2, 2 );
	int err = WSAStartup( wVersionRequested, &wsaData );
#endif
}

/** @brief constructor.
 */
OmiscidSystemLayerInitClass::OmiscidSystemLayerInitClass()
{
	// Enter locker
	OmiscidSystemLayerInitClassInitMutex.Lock();	// Add SL_ as comment in order to prevent false alarm in code checker on locks

	OmiscidSystemLayerInitClassInstanceCount++;
	if ( OmiscidSystemLayerInitClassInstanceCount == 1 )
	{
		// First instance, do init for Layer System
		OmiscidTrace( "Init System layer\n" );

		// Init random for main thread
		RandomInit();

		// Init Socket
		OmiscidSocketInit();

		// Init Memory Tacking subsystem
		// TrackMemoryLeaksInit();
	}

	// Leave locker
	OmiscidSystemLayerInitClassInitMutex.Unlock();	// Add SL_ as comment in order to prevent false alarm in code checker on locks
}

/** @brief Virtual destructor, always.
 */
OmiscidSystemLayerInitClass::~OmiscidSystemLayerInitClass()
{
	// Enter locker
	OmiscidSystemLayerInitClassInitMutex.Lock();	// Add SL_ as comment in order to prevent false alarm in code checker on locks

	OmiscidSystemLayerInitClassInstanceCount--;
	if ( OmiscidSystemLayerInitClassInstanceCount == 0 )
	{
		// Last instance, do reset for Layer System
		OmiscidTrace( "Free System layer\n" );

		// Nothing to do for the moment
	}

	// Leave locker
	OmiscidSystemLayerInitClassInitMutex.Unlock();	// Add SL_ as comment in order to prevent false alarm in code checker on locks
}

// External function to initialise this layer
void OmiscidSystemLayerInit()
{
	// Just create a static object
	static OmiscidSystemLayerInitClass OmiscidSystemLayerInitObject;
}

// For this layer and only this layer, we can create a static
// object for initialisation. We use also OmiscidSystemLayerInit()
// for uniformity
static OmiscidSystemLayerInitClass OmiscidSystemLayerInitObject;	/*!< A static object to initialise automatically this layer. */