#include <System/Portage.h>
// #include <System/Thread.h>
#include <System/MemoryBuffer.h>

#ifdef OMISCID_ON_WINDOWS

#include <sys/timeb.h>
#include <math.h>

#endif

using namespace Omiscid;

#ifdef OMISCID_ON_WINDOWS

int Omiscid::random()
{
	int result;

#if RAND_MAX <= 32767
	int i;
	short * where = (short*)&result;
	for( i = 0; i < (sizeof(result)/sizeof(short)); i++ )
	{
		where[i] = (short)rand();
	}
#else
	result = rand();
#endif
	return result;
}

int Omiscid::gettimeofday(struct timeval * tv,struct timezone * tz )
{
	struct _timeb tb;
	_ftime(&tb);

	tv->tv_sec  = (long)tb.time;
	tv->tv_usec = tb.millitm * (long)1000;

	return 0;
}

#endif // OMISCID_ON_WINDOWS

#if defined OMISCID_ON_WINDOWS || defined __APPLE__

// Add libc extension from gnu to Windows/Mac OSX

void * Omiscid::memrchr( const void * Buf, int c, size_t size )
{
	char *	TmpChar;
	size_t		Pos;
	char	TmpC;

	if ( Buf == NULL || size <= 0 )
	{
		return NULL;
	}

	TmpChar = (char*)Buf;
	TmpC = (char)c;

	for( Pos = size-1; Pos >= 0; Pos-- )
	{
		if ( TmpChar[Pos] == TmpC )
		{
			return (void*)&TmpChar[Pos];
		}
	}
	return NULL;
}

#endif // defined OMISCID_ON_WINDOWS || defined __APPLE__

#ifndef __APPLE__

// Port of this interesting apple function
size_t Omiscid::strlcpy(char *dst, const char *src, size_t size)
{
	size_t i;
	size_t max;

	if ( dst == NULL || src == NULL || size == 0 )
	{
		return (size_t)0;
	}

	// max pos to copy (we want to keep a place for '\0'
	max = size-1;

	// copy data
	for( i = 0; i < max && src[i] != '\0'; i++ )
	{
		dst[i] = src[i];
	}

	// Just to be sure, let's put a '\0' at end
	dst[i] = '\0';

	return i;
}

#endif


void Omiscid::RandomInit()
{
		struct timeval t;
		gettimeofday(&t, NULL);

#ifdef OMISCID_ON_WINDOWS
		srand( t.tv_sec ^ t.tv_usec ^ (long int)GetTickCount());
#else	// OMISCID_ON_WINDOWS
		srandom( t.tv_sec ^ t.tv_usec ^ (long int)getpid() );
#endif	// OMISCID_ON_WINDOWS
}

SimpleString Omiscid::GetLoggedUser()
{
	SimpleString Login;

#ifdef OMISCID_ON_WINDOWS
	// Just for visibility reason
	#define MAX_LOGIN_LEN 512

	DWORD len;
	MemoryBuffer UserName(MAX_LOGIN_LEN);	// for debugging purpose

	// init data to empty login
	*((char*)UserName) = '\0';
	len = MAX_LOGIN_LEN-1;

	GetUserName( UserName, &len );
	Login = (char*)UserName;

	#undef MAX_LOGIN_LEN
#else
	// alternatively we could use getpwuid( geteuid() );
	Login = getenv("LOGNAME");

	// Backward compatibility
	if ( Login.IsEmpty() )
	{
		Login = getenv("USER");
	}
#endif

	return Login;
}

