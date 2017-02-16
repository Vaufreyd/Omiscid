#include <System/SimpleString.h>

#include <System/LockManagement.h>
#include <System/MemoryBuffer.h>
#include <System/Portage.h>

#include <string.h>


using namespace Omiscid;

#define SimpleStringNULL (nullptr)

// static
SimpleString SimpleString::EmptyString("");

SimpleString::SimpleString(const std::string& str) /*!< build a string with the value 'str'*/
{
	std::string::operator=(str);
}

SimpleString::SimpleString(const char* str)
{
	std::string::operator=(str);
}

SimpleString::SimpleString(const char* str1, const char* str2)
{
	operator=(str1);
	operator+=(str2);
}

SimpleString::SimpleString(const SimpleString& to_copy)
{
	operator=(to_copy);
}

/*! Copy constructor for 2 strings */
SimpleString::SimpleString(const SimpleString& to_copy1, const SimpleString& to_copy2)
{
	operator=(to_copy1);
	operator+=(to_copy2);
}

SimpleString::SimpleString(int i)
{
	operator=(i);
}

SimpleString::SimpleString(unsigned int ui)
{
	operator+=(ui);
}

SimpleString::SimpleString(long int li)
{
	operator+=(li);
}

SimpleString::SimpleString(float f)
{
	operator+=(f);
}

SimpleString::SimpleString(double d)
{
	operator+=(d);
}

SimpleString::~SimpleString()
{
}


void SimpleString::Empty()
{
	operator=("");
}

const SimpleString& SimpleString::operator=(const SimpleString& str)
{
	std::string::operator=(str.c_str());

	return *this;
}

const SimpleString& SimpleString::operator=(int i)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%d", i);
	operator=((char*)tmp);

	return *this;
}

const SimpleString& SimpleString::operator=(unsigned int ui)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%u", ui);
	operator=((char*)tmp);

	return *this;
}

const SimpleString& SimpleString::operator=(long int li)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%li", li);
	operator=((char*)tmp);

	return *this;
}

const SimpleString& SimpleString::operator=(float f)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%f", f);
	operator=((char*)tmp);

	return *this;
}

const SimpleString& SimpleString::operator=(double d)
{
	MemoryBuffer tmp(55);
	// warning: ISO C++ does not support the '%lf' printf format
	snprintf((char*)tmp, 100, "%lf", d);
	operator=((char*)tmp);

	return *this;
}

/*! assign a string representation of size_t as content for this string */
const SimpleString& SimpleString::operator=(size_t st)
{
	MemoryBuffer tmp(55);
	// warning: ISO C++ does not support the '%lf' printf format
#ifdef OMISCID_ON_WINDOWS
	snprintf((char*)tmp, 100, "%Iu", st);
#else
	snprintf((char*)tmp, 100, "%zu", st);
#endif
	operator=((char*)tmp);

	return *this;
}


SimpleString& SimpleString::operator+= (const SimpleString& str)
{
	Append(str);
	return *this;
}

SimpleString& SimpleString::operator+= (int i)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%d", i);
	Append((char*)tmp);
	return *this;
}

SimpleString& SimpleString::operator+= (char c)
{
	MemoryBuffer tmp(10);
	snprintf((char*)tmp, 10, "%c", c);
	Append((char*)tmp);
	return *this;
}

SimpleString& SimpleString::operator+= (unsigned int ui)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%u", ui);
	Append((char*)tmp);
	return *this;
}

SimpleString& SimpleString::operator+=(long l)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%li", l);
	Append((char*)tmp);
	return *this;
}

SimpleString& SimpleString::operator+=(float f)
{
	MemoryBuffer tmp(100);
	snprintf((char*)tmp, 100, "%f", f);
	Append((char*)tmp);
	return *this;
}

SimpleString& SimpleString::operator+=(double d)
{
	MemoryBuffer tmp(100);
	// warning: ISO C++ does not support the '%lf' printf format
	snprintf((char*)tmp, 100, "%lf", d);
	Append((char*)tmp);
	return *this;
}

SimpleString& SimpleString::operator+=(size_t st)
{
	MemoryBuffer tmp(100);
	// warning: ISO C++ does not support the '%lf' printf format
#ifdef OMISCID_ON_WINDOWS
	snprintf((char*)tmp, 100, "%Iu", st);
#else
	snprintf((char*)tmp, 100, "%zu", st);
#endif
	Append((char*)tmp);
	return *this;
}

SimpleString SimpleString::SubString(size_t begin, size_t end) const
{
	size_t Len = end-begin;
	return substr( begin, Len );
}

const SimpleString Omiscid::operator+(const SimpleString& str1, const SimpleString& str2)
{
	if ( str1.GetLength() == 0 ) return SimpleString(str2);
	if ( str2.GetLength() == 0 ) return SimpleString(str1);
	return SimpleString(str1, str2);
}

const SimpleString Omiscid::operator+(const char* str1, const SimpleString& str2)
{
	if ( str1 == (const char*)SimpleStringNULL ) return SimpleString(str2);
	if ( str2.GetLength() == 0 ) return SimpleString(str1);
	return SimpleString( str1, str2 );
}

const SimpleString Omiscid::operator+(const SimpleString& str1, const char* str2)
{
	if ( str1.GetLength() == 0 ) return SimpleString(str2);
	if ( str2 == (const char *)SimpleStringNULL ) return SimpleString(str1);
	return SimpleString( str1, str2 );
}

size_t SimpleString::Find(const SimpleString SearchPattern, size_t StartSearch /*= 0*/, bool Backward /* = false */ ) const
{
	if ( Backward == false )
	{
		return find( SearchPattern, StartSearch );
	}
	else
	{
		return rfind( SearchPattern, StartSearch );
	}
}

bool SimpleString::InternalReplaceFirst(const SimpleString& SearchPattern, const SimpleString& ReplacedPattern, size_t& EndOfReplace)
{
	size_t PosInString = Find(SearchPattern);
	if ( PosInString == std::string::npos )
	{
		EndOfReplace = std::string::npos;
		return false;
	}

	replace( PosInString, SearchPattern.GetLength(), ReplacedPattern );

	EndOfReplace = PosInString+ReplacedPattern.GetLength();

	return true;
}

bool SimpleString::ReplaceFirst(const SimpleString& SearchPattern, const SimpleString& ReplacedPattern)
{
	size_t EndOfReplace;
	bool ret = InternalReplaceFirst( SearchPattern, ReplacedPattern, EndOfReplace );

	return ret;
}

bool SimpleString::ReplaceAll(const SimpleString& SearchPattern, const SimpleString& ReplacedPattern)
{
	bool ret = false;

	size_t StartSearch = 0;
	while( InternalReplaceFirst( SearchPattern, ReplacedPattern, StartSearch ) )
	{
		ret = true;
	}

	return ret;
}



bool SimpleString::IsEmpty() const
{
	return ( length() == 0 );
}

bool SimpleString::operator!=(const SimpleString& str) const
{
	return !(strcmp( c_str(), str.c_str() ) == 0);
}

bool SimpleString::operator!=(const char* str) const
{
	return !(strcmp( c_str(), str ) == 0);
}

/*! Compare (case insensitive) 2 strings to check if they are equals */
bool SimpleString::EqualsCaseInsensitive(const SimpleString& str) const
{
	return (strcasecmp( c_str(), str.c_str() ) == 0);
}

/*! Compare (case insensitive) 2 strings to check if they are equals */
bool SimpleString::EqualsCaseInsensitive(const std::string& str) const
{
	return (strcasecmp( c_str(), str.c_str() ) == 0);
}

/*! Compare (case insensitive) 2 strings to check if they are equals */
bool SimpleString::EqualsCaseInsensitive(const char * str) const
{
	return (strcasecmp( c_str(), str ) == 0);
}

int SimpleString::CompareNoCase(const char* str) const
{
	return strcasecmp( c_str(), str );
}

int SimpleString::CompareNoCase(const SimpleString& str) const
{
	return strcasecmp( c_str(), str.c_str() );
}

int SimpleString::CompareNoCase(const std::string& str) const
{
	return strcasecmp( c_str(), str.c_str() );
}

bool SimpleString::Latin1ToUTF8( const char *Src, char * Latin1ToUTF8Buffer, int SizeOfBuffer )
{
	const char * parse = Src;
	int res;

	if ( parse == (const char*)SimpleStringNULL )
		return false;

	for( res=0; res < SizeOfBuffer; parse++, res++ )
	{
		if ( *parse == '\0' )
		{
			Latin1ToUTF8Buffer[res] = '\0';
			break;
		}
		if ( (*parse & 0x80) == 0 ) // ASCII 7
		{
			Latin1ToUTF8Buffer[res] = *parse;
			continue;
		}
		else
		{
			if ( res+1 >= (SizeOfBuffer+1) )
			{
				return false;
			}

			Latin1ToUTF8Buffer[res]   = (char)0xc3;
			Latin1ToUTF8Buffer[++res] = (char)(*parse-64);
		}
	}

	if ( res >= SizeOfBuffer )
		return false;

	return true;
}

#ifdef OMISCID_RUNING_TEST

#include <System/ElapsedTime.h>
#include <System/Event.h>
#include <System/Socket.h>

int SimpleString::TestFunction()
{
	// Test of SimpleString...

	SimpleString Test("metis.local.");

	bool Modif = false;
	Test = Socket::RemoveLocalDomain( Test, Modif );

	SimpleString Tutu;

	Tutu = "\\032Yop";
	OmiscidTrace( "%s\n", Tutu.GetStr() );
	Tutu.ReplaceFirst( "\\032", " " );
	OmiscidTrace( "%s\n", Tutu.GetStr() );

	Tutu = "Yop\\032";
	OmiscidTrace( "%s\n", Tutu.GetStr() );
	Tutu.ReplaceFirst( "\\032", " " );
	OmiscidTrace( "%s\n", Tutu.GetStr() );

	Tutu = "Yop\\032blib";
	OmiscidTrace( "%s\n", Tutu.GetStr() );
	Tutu.ReplaceFirst( "\\032", " " );
	OmiscidTrace( "%s\n", Tutu.GetStr() );

	Tutu = "Yop\\0 liajezijclz jl ijzzlij blib";
	OmiscidTrace( "%s\n", Tutu.GetStr() );
	Tutu.ReplaceFirst( "\\032", " " );
	OmiscidTrace( "%s\n", Tutu.GetStr() );

	Tutu = "Yop\\032liajezijclz jl ijzzlij blib";
	OmiscidTrace( "%s\n", Tutu.GetStr() );
	Tutu.ReplaceAll( "\\032", " " );
	OmiscidTrace( "%s\n", Tutu.GetStr() );

	Tutu = "Yop\\032liajezijclz jl\\032 ijzzlij blib";
	OmiscidTrace( "%s\n", Tutu.GetStr() );
	Tutu.ReplaceAll( "\\032", "+++" );
	OmiscidTrace( "%s\n", Tutu.GetStr() );

	return 0;
}
#endif // OMISCID_RUNING_TEST

#ifdef OMISCID_ON_WINDOWS
	#undef snprintf
#endif

