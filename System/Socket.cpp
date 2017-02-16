#include <System/Socket.h>
#include <System/Portage.h>
#include <System/MemoryBuffer.h>

#include <sys/types.h>

#ifdef OMISCID_ON_WINDOWS
#define MSG_NOSIGNAL 0
#define socklen_t int

#pragma warning(disable : 4800) // To prevent warning about performance warning
#pragma warning(disable : 4127) // Disable warning when using FD_SET

#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/select.h>
#include <netdb.h>

#include <errno.h>

#endif

#include <string.h>

#ifdef __APPLE__
// MSG_NOSIGNAL is not available under Darwin
#define MSG_NOSIGNAL 0
#endif

#include <System/SocketException.h>


#define BACKLOG 5

using namespace Omiscid;


// Static members of the Socket class
Socket::DynamicNameSolvingType Socket::GetDynamicNameSolving()
{
	static DynamicNameSolvingType DynamicNameSolving = OMISCIDNS_UNSET;

	if ( DynamicNameSolving == OMISCIDNS_UNSET )
	{
		char * Option = getenv( "OMISCIDNS_USE_MDNS_NAME_SOLVING" );
		if ( Option == NULL )
		{
			DynamicNameSolving = OMISCIDNS_USE_DNS_ONLY;
			OmiscidTrace( "Use standard DNS for name solving (faster on non ad-hoc networks - default).\n" );
		}
		else
		{
			DynamicNameSolving = OMISCIDNS_USE_MDNS_NAME_SOLVING;
			OmiscidTrace( "OMISCIDNS_USE_MDNS_NAME_SOLVING found. Use mDNS for name solving (useful on ad-hoc networks).\n" );
		}
	}

	return DynamicNameSolving;
}

Socket::Socket()
: socketType(SOCKET_KIND_UNDEFINED), descriptor((SOCKET)SOCKET_ERROR)
{}

Socket::Socket(SocketKind type)
: socketType(type), descriptor((SOCKET)SOCKET_ERROR)
{
	if((descriptor = socket(AF_INET, type, 0)) == SOCKET_ERROR)
	{
		throw SocketException("socket", Errno());
	}
}

Socket::~Socket()
{
	Close();
}

int Socket::Errno()
{
#ifdef OMISCID_ON_WINDOWS
	return WSAGetLastError();
#else
	return errno;
#endif
}

void Socket::SetDescriptor(SOCKET descr)
{
	descriptor = descr;
	int t;
	int size = sizeof(int);

	if ( getsockopt(descriptor, SOL_SOCKET, SO_TYPE , (char*)&t, (socklen_t*)&size) == SOCKET_ERROR )
	{
		throw SocketException("getsockopt", Errno());
	}

	socketType = (SocketKind)t;
}

void Socket::Bind(const SimpleString addr, int port)
{
	struct sockaddr_in my_addr;

	if ( FillAddrIn( &my_addr, addr, port ) == false )
	{
		throw SocketException("Socket::Bind FillAddrIn");
	}

	if( bind(descriptor, (struct sockaddr*)&my_addr, sizeof(struct sockaddr) ) == SOCKET_ERROR)
	{
		throw SocketException("Socket::Bind bind");
	}

}

void Socket::Listen()
{
	if( listen(descriptor, BACKLOG) == SOCKET_ERROR)
	{
		throw SocketException("listen", Errno());
	}
}

/**
 * @brief Check if socket descriptor is valid
 */
bool Socket::IsActive() const
{
	return (descriptor != SOCKET_ERROR);
}

Socket* Socket::Accept()
{
	// Variables to use the select function on our descriptor
	timeval timeout;
	fd_set fds;
	int nReady;
	int MaxDesc = 0;	// Maximal descriptor for the select function

	// REVIEW : a revoir...
	struct sockaddr_in the_addr;
	int size = sizeof(struct sockaddr_in);
	SOCKET new_fd;

	// ok, we will wait for an accept attempt for 100 ms
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	FD_ZERO(&fds);
	FD_SET( descriptor, &fds );

#ifndef OMISCID_ON_WINDOWS
	MaxDesc = descriptor + 1;
#else
	// On OMISCID_ON_WINDOWS plateform, the value is unused and remain 0
#endif

	// Ok, wait for the connection...
	nReady = select(MaxDesc, &fds, NULL, NULL, &timeout);

	// If our socket can accept a connection...
	if ( nReady > 0 )
	{
		new_fd = accept(descriptor, (struct sockaddr*)&the_addr, (socklen_t*)&size);
		Socket* s = new OMISCID_TLM Socket();
		if ( s )
		{
			s->SetDescriptor(new_fd);
			return s;
		}
		// else we will return NULL at the end of the function
	}

	// ok, nothing to accept, go up to see if there is a pending request...
	return NULL;
}

const SimpleString Socket::GetConnectedHost()
{
	struct hostent *he;
	struct sockaddr_in addr;
	socklen_t namelen = sizeof(struct sockaddr_in);

	if ( descriptor < 0 || socketType != TCP ) // All kind of errors
	{
		return "";
	}

	if ( ConnectedHost.GetLength() == 0 )
	{
		// Try to get the peername
		if ( getpeername( descriptor, (sockaddr*)&addr, &namelen ) == SOCKET_ERROR )
		{
			return "";
		}

		// Ok, let's get the name of the connected host

		he = gethostbyaddr((char *) &addr.sin_addr, 4, AF_INET);
		if ( he == NULL )
		{
			return "";
		}

		ConnectedHost = he->h_name;
	}

	return ConnectedHost;
}

const struct sockaddr_in * Socket::GetAddrDest()
{
	return (const struct sockaddr_in *)&dest;
}

bool Socket::FillAddrIn(struct sockaddr_in * pAdd, const SimpleString name, int port)
{
	if ( pAdd == NULL || port < 0 )
		return false;

	pAdd->sin_family = AF_INET;
#ifdef OMISCID_ON_WINDOWS
	pAdd->sin_port = htons((u_short)port);
#else
	pAdd->sin_port = htons((uint16_t)port);
#endif
	if ( name.GetLength() == 0 )	// name == ""
	{
		// Brodcast
		pAdd->sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		// new version using getaddrinfo
		// TO TEST
		struct addrinfo *pai = NULL;
		// pAdd->sin_addr = 0;

		// Get short name
		bool ModificationDone = false;
		Omiscid::SimpleString ShortName = RemoveLocalDomain( name, ModificationDone );

		if ( getaddrinfo(ShortName.GetStr(), NULL, NULL, &pai) == 0 )
		{
			for( struct addrinfo *p = pai; p != NULL; p = p->ai_next )
			{
				if (p->ai_family == AF_INET)
				{
					// int si = sizeof(struct sockaddr_in);
					pAdd->sin_addr = ((struct sockaddr_in*)p->ai_addr)->sin_addr;
					memset(&(pAdd->sin_zero), 0, 8);
					freeaddrinfo(pai);
					return true;
				}
			}
		}

		if ( pai != NULL )
		{
			freeaddrinfo(pai);
			pai = NULL;
		}

		// Here we did not get network info
		if ( ModificationDone == true )
		{
			// Ask with full name
			if ( getaddrinfo(name.GetStr(), NULL, NULL, &pai) == 0)
			{
				for( struct addrinfo *p = pai; p != NULL; p = p->ai_next )
				{
					if (p->ai_family == AF_INET)
					{
						pAdd->sin_addr = ((struct sockaddr_in*)p->ai_addr)->sin_addr;
						memset(&(pAdd->sin_zero), 0, 8);
						freeaddrinfo(pai);
						return true;
					}
				}
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Socket::Connect(const SimpleString addr, int port, int ConnectionTimeout /* = DEFAULT_CONNECTION_TIMEOUT */ )
{
	if ( socketType == TCP )
	{
		struct sockaddr_in the_addr;
		FillAddrIn(&the_addr, addr, port);
		
#ifndef OMISCID_ON_WINDOWS
		// TO CHECK : strange behavior on Linux test... Back to blocking connect on non Windows plateform
		if( connect(descriptor, (struct sockaddr*)&the_addr, sizeof(struct sockaddr)) == SOCKET_ERROR )
		{
#ifdef DEBUG
			SimpleString Mesg;
			Mesg = "connect to ";
			Mesg += addr;
			Mesg += ":";
			Mesg += port;
			throw SocketException( Mesg, Errno() );
#else
			throw SocketException("connect", Errno());
#endif
		}

		return true;
#endif

		// Put the socket in non blocking mode
		if ( SetBlockingMode(false) == true )
		{
			// Do it non blocking...
			int res = connect(descriptor, (struct sockaddr*)&the_addr, sizeof(struct sockaddr));
			if ( res == SOCKET_ERROR )
			{
#ifdef OMISCID_ON_WINDOWS
				if ( Errno() == WSAEWOULDBLOCK )
#else
				if ( Errno() == EWOULDBLOCK )
#endif
				{
					// Call Select to wait for completion
					// Done, change it to non full blocking mode....
					fd_set socketfds;

					timeval timeout;

					FD_ZERO(&socketfds);
					FD_SET(descriptor, &socketfds);

					// First check for event (like disconnection...)
					timeout.tv_sec = ConnectionTimeout/1000;			// seconds
					timeout.tv_usec = (ConnectionTimeout%1000)*1000;	// millisecond to microseconds

					// Ask if some event are waiting on this socket
					if (select((int)descriptor + 1, NULL, &socketfds, NULL, &timeout) > 0)
					{
						// an event occurs, thus we are connected
						// back to blocking mode
						SetBlockingMode( true );

						return true;
					}
				}

				// True error
				Close();

#ifdef DEBUG
				SimpleString Mesg;
				Mesg = "connect to ";
				Mesg += addr;
				Mesg += ":";
				Mesg += port;
				throw SocketException(Mesg, Errno());
#else
				throw SocketException("connect", Errno());
#endif
			}

			// here we are *not* connected
			return false;
		}
		else
		{
			// Do it as usual, without connection timeout
			if ( connect(descriptor, (struct sockaddr*)&the_addr, sizeof(struct sockaddr)) == SOCKET_ERROR )
			{
#ifdef DEBUG
				SimpleString Mesg;
				Mesg = "connect to ";
				Mesg += addr;
				Mesg += ":";
				Mesg += port;
				throw SocketException(Mesg, Errno());
#else
				throw SocketException("connect", Errno());
#endif
			}

			return true;
		}
	}
	else /* UDP */
	{
		// Simply fill destination address
		FillAddrIn(&dest, addr, port);
		return true;
	}

	// We should never be here
	return false;
}

void Socket::Close()
{
	if ( descriptor == SOCKET_ERROR )
	{
		return;
	}
#ifdef OMISCID_ON_WINDOWS
	::shutdown(descriptor, SD_BOTH);
	closesocket(descriptor);
#else
	shutdown(descriptor, SHUT_RDWR);
	close(descriptor);
#endif
	descriptor = (SOCKET)SOCKET_ERROR;
}

int Socket::Recv(size_t len, unsigned char* buf, struct sockaddr_in* pfrom)
{
	int res = 0;
	if(socketType == TCP)
	{
		if((res = recv(descriptor, (char*)buf, (CONVERT_SIZE_FOR_RECVSEND)len, 0)) == SOCKET_ERROR)
		{
			res = Errno();
			throw SocketException("recv_sock_stream", res );
		}
	}
	else
	{
		struct sockaddr_in from;
		struct sockaddr_in* fromptr = (pfrom)? pfrom : &from;
		int from_len = sizeof(struct sockaddr);
		if((res = recvfrom(descriptor, (char*)buf, (CONVERT_SIZE_FOR_RECVSEND)len, 0,
			(struct sockaddr*)fromptr, (socklen_t*)&from_len)) == SOCKET_ERROR)
		{
			throw SocketException("recv_sock_dgram", Errno());
		}
	}
	return res;
}

int Socket::Send(size_t len, const char* buf)
{
	const int socket_send_flag = MSG_NOSIGNAL;
	int res = 0;		// as gcc on gentoo seems not understand that this will be initialised

	if ( len <= 0 || buf == (const char*)NULL )
	{
		return SOCKET_ERROR;
	}

	if ( socketType == TCP )
	{
		if ( len > (TCP_BUFFER_SIZE-1) )
		{
			int TotalLen = 0;

			// Send all parts of the message in maximum TCP_BUFFER_SIZE frame
			// Let's send data... For linux, we can not send it as a single call to
			// send, so let's do it the other way... Naggle will do the rest...
			while( TotalLen < len )
			{
				if ( (len-TotalLen) >= (TCP_BUFFER_SIZE-1) )	// 60 Ko
				{
					res = send(descriptor, (char*)(buf+TotalLen), (TCP_BUFFER_SIZE-1), socket_send_flag );
				}
				else
				{
					res = send(descriptor, (char*)(buf+TotalLen), (CONVERT_SIZE_FOR_RECVSEND)(len-TotalLen), socket_send_flag );
				}

				if ( res == SOCKET_ERROR )
				{
					throw SocketException("send_sock_stream", Errno());
				}
				TotalLen += res;
			}

			return TotalLen;
		}
		else
		{
			if ( (res = send(descriptor, buf, (CONVERT_SIZE_FOR_RECVSEND)len, socket_send_flag)) == SOCKET_ERROR )
			{
				throw SocketException("send_sock_stream", Errno());
			}
		}
	}
	else
	{
		if((res = sendto(descriptor, buf, (CONVERT_SIZE_FOR_RECVSEND)len, socket_send_flag,
			(struct sockaddr*)&dest, sizeof(struct sockaddr))) == SOCKET_ERROR)
		{
			throw SocketException("send_sock_dgram", Errno());
		}
	}
	return res;
}

int Socket::SendTo(size_t len, const char* buf, struct sockaddr_in* adest)
{
	const int socket_send_flag = MSG_NOSIGNAL;
	int res;

	if((res = sendto(descriptor, buf, (CONVERT_SIZE_FOR_RECVSEND)len, socket_send_flag,
		(struct sockaddr*)adest, sizeof(struct sockaddr))) == SOCKET_ERROR)
	{
		throw SocketException("send_sock_dgram", Errno());
	}
	return res;
}

bool Socket::Select()
{
	// Done, change it to non full blocking mode....
	fd_set socketfds;

	timeval timeout;

	FD_ZERO(&socketfds);
	FD_SET(descriptor, &socketfds);

	// First check for event (like disconnection...)
	timeout.tv_sec = 0;
	timeout.tv_usec = 10;	// 10 micro seconds

	// Ask if some event are waiting on this socket
	if ( select((int)descriptor+1, NULL, NULL, &socketfds, &timeout) > 0 )
	{
		// Ok, something happens
		throw SocketException("select", Errno());
	}

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;	// 100 ms

	FD_ZERO(&socketfds);
	FD_SET(descriptor, &socketfds);

	if( select((int)descriptor+1, &socketfds, NULL, NULL, &timeout) == SOCKET_ERROR )
	{
		// A problem occurs
#ifdef OMISCID_ON_WINDOWS
		throw SocketException("select",  Errno());
#else
		int err = Errno();
		if ( err != EINTR )
		{
			// This error is sent when there is a pending signal (not necessary
			// related to this socket) in order to get out of low level system
			// and let the appropriate process of the signal, so it is not an
			// error
			throw SocketException("select", err);
		}
#endif
	}
	return FD_ISSET(descriptor, &socketfds);
}

/**
* @return true blocking mode was set
* @param BlockingMode [in] Will put the socket in blocking mode if BlockingMode == true. Non blocking mode if false.
*/
bool Socket::SetBlockingMode( bool BlockingMode )
{
#ifdef OMISCID_ON_WINDOWS
	// Set mode to 0 if blocking mode is asked
	unsigned long iMode = BlockingMode ? 0 : 1;
	// Call ioctlsocket to set (non)blocking mode
	int res = ioctlsocket(descriptor, FIONBIO, &iMode);
	// Return true if ok
	return (res == NO_ERROR);
#else 
	// Retrieving current flags
	int CurrentFlags;
	if ( (CurrentFlags = fcntl(descriptor, F_GETFL, 0)) < 0 )
	{
		// error!
		return false;
	}
	// Compute flags with(out) blocking mode
	CurrentFlags = BlockingMode ? (CurrentFlags&~O_NONBLOCK) : (CurrentFlags|O_NONBLOCK);
	// return true if we manage to apply flags
	return (fcntl(descriptor, F_SETFL, CurrentFlags) == 0);
#endif
}

unsigned short Socket::GetPortNb()
{
	struct sockaddr_in name;
	socklen_t size = sizeof(struct sockaddr_in);
	memset(&name, 0, size);
	if( getsockname(descriptor, (struct sockaddr*)&name, &size) != 0 )
		throw SocketException("getsockname", Errno());

	return ntohs(name.sin_port);
}


SimpleString Socket::GetHostName()
{
	MemoryBuffer HostName(1024);

	if ( ::gethostname( HostName, 1023) )
	{
		throw SocketException("GetHostName", Errno());
	}
	return SimpleString( (const char*)HostName );
}

  /**
   * Return the IP Adress of the socket
   */
SimpleString Socket::GetLocalIPAdress()
{
	struct sockaddr_in LocalAddress;
	socklen_t LenOfLocalAddress = (socklen_t)sizeof(LocalAddress);

	if ( descriptor == 0 || getsockname(descriptor, (struct sockaddr*)&LocalAddress, &LenOfLocalAddress ) != 0 )
	{
		return "";
	}

	return SimpleString(inet_ntoa(LocalAddress.sin_addr));
}

const SimpleString Socket::RemoveLocalDomain( const SimpleString Name, bool& Modified )
{
	size_t Pos;
	SimpleString TmpS(Name);

	// Say that we do not modify it
	Modified = false;

	// seek .local.
	Pos = TmpS.Find( ".local." );
	if ( Pos != -1 && (Pos+7) == (int)TmpS.GetLength() )
	{
		// Ok we found "xxx.local."
		Modified = true;
		return TmpS.SubString( 0, Pos );
	}

	// seek .local.
	Pos = TmpS.Find( ".local" );
	if ( Pos != -1 && (Pos+6) == (int)TmpS.GetLength() )
	{
		// Ok we found "xxx.local"
		Modified = true;
		return TmpS.SubString( 0, Pos );
	}

	return TmpS;
}

const SimpleString Socket::RemoveDnsSdDaemonNumber( const SimpleString Name, bool& Modified )
{
	size_t PosDeb;			// Position of '-\d+' in the string

	// Say that we do not modify it
	Modified = false;

	bool PreviousSequenceOfNumber = false;	// give information about the fact we have numbers before
	int SizeOfNum = 0;						// number of characters in the sequence '-\d+'
	// Start from end
	for( PosDeb = Name.GetLength()-1; PosDeb >= 2; PosDeb-- )
	{
		if ( Name[PosDeb] == '-' )
		{
			if ( PreviousSequenceOfNumber == true )
			{
				// Ok, we've got '-\d+', do the modification
				Modified = true;
				return Name.SubString(0,PosDeb) + Name.SubString(PosDeb+1+SizeOfNum,Name.GetLength() );
			}
			// ok, the next previous is '-', so not a number
			PreviousSequenceOfNumber = false;
			SizeOfNum = 0;
		}
		if ( Name[PosDeb] >= '0' && Name[PosDeb] <= '9' )
		{
			// Ok, we've got a number
			PreviousSequenceOfNumber = true;
			SizeOfNum++;
			continue;
		}
		// ok, this is not a number
		PreviousSequenceOfNumber = false;
		SizeOfNum = 0;
	}

	return Name;
}

hostent* Socket::GetHostByName( const SimpleString name )
{
	struct hostent *he;
	SimpleString SearchName = name;
	bool IsModified;

	if ( GetDynamicNameSolving() == OMISCIDNS_USE_MDNS_NAME_SOLVING )
	{
		// Try to solve name !
		if((he = ::gethostbyname(SearchName.GetStr())) != (struct hostent*)NULL )
		{
			return he;
		}

		// Here try to cope with multiple DnsSd deamon running on the
		// same computer (like Avahi *AND* mDNS).
		SearchName = RemoveDnsSdDaemonNumber( SearchName, IsModified );
		if ( IsModified == false )
		{
			return (struct hostent*)NULL;
		}

		if((he = ::gethostbyname(SearchName.GetStr())) == (struct hostent*)NULL )
		{
			SimpleString Mesg = "GetHostByName : ";
			Mesg += name;
			Mesg += " not found";
			throw SocketException(Mesg, Errno());
		}
		return he;
	}

	// remove try to remove ".local." at end
	SearchName = RemoveLocalDomain( SearchName, IsModified );
	if((he = ::gethostbyname((char*)SearchName.GetStr())) != (struct hostent*)NULL )
	{
		// ok
		return he;
	}

	// try to remove number in case of several DnsSdDaemon
	SearchName = RemoveDnsSdDaemonNumber( SearchName, IsModified );
	if ( IsModified == false )
	{
		SimpleString Mesg = "GetHostByName : ";
		Mesg += name;
		throw SocketException(Mesg, Errno());
	}

	if((he = ::gethostbyname((char*)SearchName.GetStr())) == (struct hostent*)NULL )
	{
		SimpleString Mesg = "GetHostByName : ";
		Mesg += name;
		throw SocketException(Mesg, Errno());
	}

	return he;
}

bool Socket::SetTcpNoDelay(bool Set)
{
	if ( descriptor == SOCKET_ERROR || socketType != TCP )
	{
		return false;
	}
	int OptVal;
	if ( Set == true )
	{
		OptVal = 1;
	}
	else
	{
		OptVal = 0;
	}

#ifdef OMISCID_ON_WINDOWS
	if ( setsockopt(descriptor, IPPROTO_TCP, TCP_NODELAY, (char*)&OptVal, sizeof(OptVal)) < 0 )
	{
		OmiscidError("setsockopt: could not set TCP nodelay\n");
		return false;
	}
	return true;
#else
	struct protoent *p;
	p = getprotobyname("tcp");
	if ( p && setsockopt(descriptor, p->p_proto, TCP_NODELAY, &OptVal, sizeof(OptVal)) < 0)
	{
		OmiscidError("setsockopt: could not set TCP nodelay\n");
		return false;
	}
	return true;
#endif
}

