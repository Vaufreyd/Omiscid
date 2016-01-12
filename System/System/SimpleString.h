/**
 * @file SimpleString.h
 * @ingroup System
 * @ingroup UserFriendly
 * @brief Definition of SimpleString class
 */

#ifndef __SIMPLE_STRING_H__
#define __SIMPLE_STRING_H__

#include <System/ConfigSystem.h>
#include <System/ReentrantMutex.h>

#include <string>

#include <iostream>

namespace Omiscid {

/**
 * @class SimpleString  SimpleString.h  System/SimpleString.h
 * @ingroup System
 * @ingroup UserFriendly
 * @brief 
 *
 * By default, strings are equals to "".
 * @author Sebastien Pesnel
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 */
class SimpleString : public std::string
{
public:
	SimpleString() {} /*!< build an empty string */
	SimpleString(const std::string& str); /*!< build a string with the value 'str'*/
	SimpleString(const char* str); /*!< build a string with the value 'str'*/
	/*! Build a String with the value 'str1' followed by 'str2'*/
	SimpleString(const char* str1, const char* str2);
	/*! Copy constructor of the string */
	SimpleString(const SimpleString& to_copy);
	/*! Copy constructor for 2 strings */
	SimpleString(const SimpleString& to_copy1, const SimpleString& to_copy2);
	/*! Creating a string representing an interger */
	SimpleString(int i);
	/*! Creating a string representing an unsigned interger */
	SimpleString(unsigned int ui);
	/*! Creating a string representing a long interger */
	SimpleString(long int li);
	/*! Creating a string representing a float*/
	SimpleString(float f);
	/*! Creating a string representing a double */
	SimpleString(double d);

public:
	/*! Destructor (free if necessary, the underlying buffer) */
	virtual ~SimpleString();

	/**
	 * @brief To empty a string
	 */
	void Empty();

	/** @name Data Access */
	//@{
	/** @return the character string */
	const char* GetStr() const { return c_str(); } 

	/** @return the length of the string */
	size_t GetLength() const { return length(); }

	//@}

	/**
	 * @return true if the string is empty, false otherwise
	 */
	bool IsEmpty() const;

	/** @name Affectation */
	//@{
	/*! assign 'str' as content for this string */
	const SimpleString& operator= (const SimpleString& str);
	/*! assign a string representation of an interger as content for this string */
	const SimpleString& operator= (int i);
	/*! assign a string representation of an unsigned interger as content for this string */
	const SimpleString& operator= (unsigned int ui);
	/*! assign a string representation of a long interger as content for this string */
	const SimpleString& operator= (long int li);
	/*! assign a string representation of a float as content for this string */
	const SimpleString& operator= (float f);
	/*! assign a string representation of double as content for this string */
	const SimpleString& operator= (double d);
	/*! assign a string representation of size_t as content for this string */
	const SimpleString& operator=(size_t st);
	//@}

	/** @name Data Manipulation : Append String */
	//@{
	/*! add 'str' to the end of this string */
	void Append(const char* str) { append(str); }
	/*! add 'str' to the end of this string */
	void Append(const SimpleString& str) { append(str); }
	/*! add 'str' to the end of this string */
	// SimpleString& operator+= (const char* str);
	/*! add 'str' to the end of this string */
	SimpleString& operator+= (const SimpleString& str);
	/*! add the integer 'i' changed in string to the end of this string */
	SimpleString& operator+= (unsigned int ui);
	/*! add the integer 'i' changed in string to the end of this string */
	SimpleString& operator+= (int i);
	/*! append the character 'c' to the end of this string */
	SimpleString& operator+= (char c);
	/*! add the long integer 'l' changed in string to the end of this string */
	SimpleString& operator+= (long l);
	/*! add the long integer 'l' changed in string to the end of this string */
	SimpleString& operator+= (float f);
	/*! add the integer 'd' changed in string (using no specific formating) to the end of this string */
	SimpleString& operator+= (double d);
	/*! add size 'st' to the end of this string */
	SimpleString& operator+= (size_t st);
	//@}

	/** @name Data Comparaison */
	//@{
	/*! Compare (case sensitive) 2 strings to check if they are equals */
	// bool operator==(const SimpleString& str) const;
	/*! Compare (case sensitive) 2 strings to check if they are equals */
	// bool operator==(const char* str) const;
	/*! Compare (case sensitive) 2 strings to check if they are *not* equals */
	bool operator!=(const SimpleString& str) const;
	/*! Compare (case sensitive) 2 strings to check if they are *not* equals */
	bool operator!=(const char* str) const;
	/*! Compare (case insensitive) 2 strings to check if they are equals */
	bool EqualsCaseInsensitive(const SimpleString& str) const;
	/*! Compare (case insensitive) 2 strings to check if they are equals */
	bool EqualsCaseInsensitive(const char * str) const;
	/*! Compare (case insensitive) 2 strings to check if they are equals */
	bool EqualsCaseInsensitive(const std::string& str) const;
	// to do
	int CompareNoCase(const char* str) const;
	int CompareNoCase(const SimpleString& str) const;
	int CompareNoCase(const std::string& str) const;
	//@}

	static bool Latin1ToUTF8( const char *Src, char * Latin1ToUTF8Buffer, int SizeOfBuffer );

	/** @brief Extract a string of  this string
	 * @param [in] begin index of the first character included in the result string
	 * @param [in] end index of the first excluded character
	 * return A new string conaining the sub-string
	 */
	SimpleString SubString(size_t begin, size_t end) const;

	size_t Find(const SimpleString SearchPattern, size_t StartSearch =0, bool Backward = false) const;

	static SimpleString EmptyString;

private:
	bool InternalReplaceFirst(const SimpleString& SearchPattern, const SimpleString& ReplacedPattern, size_t& EndOfReplace);

public:
	bool ReplaceFirst(const SimpleString& SearchPattern, const SimpleString& ReplacedPattern);
	bool ReplaceAll(const SimpleString& SearchPattern, const SimpleString& ReplacedPattern);

#ifdef OMISCID_RUNING_TEST
	// a function in order to test
	static int TestFunction();
#endif

};


/*
 * @brief An operator+ to collapse 2 SimpleString
 * @param [in] str1, a SimpleString
 * @param [in] str2, a SimpleString
 * @return Returns a SimpleString
 */
const SimpleString operator+(const SimpleString& str1, const SimpleString& str2);

/*
 * @brief An operator+ to collapse a char buffer and a string string
 * @param [in] str1, a char * buffer (ended by '0')
 * @param [in] str2, a SimpleString
 * @return Returns a SimpleString
 */
const SimpleString operator+(const char* str1, const SimpleString& str2);

/*
 * @brief An operator+ to collapse a char buffer and a string string
 * @param [in] str1, a SimpleString
 * @param [in] str2, a char * buffer (ended by '0')
 * @return Returns a SimpleString
 */
const SimpleString operator+(const SimpleString& str1, const char* str2);


} // namespace Omiscid

#endif // __SIMPLE_STRING_H__

