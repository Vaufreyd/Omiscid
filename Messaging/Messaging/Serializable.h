/**
 * @file Messaging/Messaging/Serializable.h
 * \ingroup Messaging
 * @brief Definition of Serializable class
 * @author Dominique Vaufreydaz
 */

#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__

#include <Messaging/ConfigMessaging.h>

#include <System/ReentrantMutex.h>
#include <System/SimpleList.h>
#include <System/SimpleString.h>

#include <Messaging/SerializeValue.h>
#include <Messaging/StructuredMessage.h>

#include <vector>
#include <list>

namespace Omiscid {

class StructuredMessage;

class Serializable : protected ReentrantMutex {
public:

	Serializable();

	virtual ~Serializable();

	virtual void DeclareSerializeMapping() = 0;

	virtual void PreSerializableFonction() {};
	virtual void PostSerializableFonction() {};

	void AddToSerialization( const SimpleString& Key, long& Val );
	void AddToSerialization( const SimpleString& Key, int& Val );
	void AddToSerialization( const SimpleString& Key, long long int& Val );
	void AddToSerialization( const SimpleString& Key, unsigned int& Val );
	void AddToSerialization( const SimpleString& Key, short int& Val );
	void AddToSerialization( const SimpleString& Key, unsigned short& Val );
	void AddToSerialization( const SimpleString& Key, double& Val );
	void AddToSerialization( const SimpleString& Key, float& Val );
	void AddToSerialization( const SimpleString& Key, bool& Val );
	void AddToSerialization( const SimpleString& Key, SimpleString& Val );
	void AddToSerialization( const SimpleString& Key, char*& Val );

	// For recursive purpose
	void AddToSerialization( const SimpleString& Key, Serializable& Val );

	template <typename CurrentType>
	void AddToSerialization( const SimpleString& Key, SimpleList<CurrentType>& Val );

	template <typename CurrentType>
	void AddToSerialization( const SimpleString& Key, std::vector<CurrentType>& Val );

	template <typename CurrentType>
	void AddToSerialization( const SimpleString& Key, std::list<CurrentType>& Val );

	SerializeValue Serialize();
	void Unserialize( const SimpleString& SerializedVal );
	void Unserialize( const SerializeValue& SerializedVal );

	bool PartialUnserializationAllowed = false;
protected:

	/** @brief Callback for the encoding function */
	typedef SerializeValue (*SerializeFunction)(void *);

	/** @brief Callback for the decoding function */
	typedef void (*UnserializeFunction)(const SerializeValue&, void *);

	class EncodeMapping
	{
	public:
		SimpleString Key;
		SerializeFunction FunctionToEncode;
		UnserializeFunction FunctionToDecode;
		void * AddressOfObject;

		inline const char * GetKey()
		{
			return Key.GetStr();
		}

		inline SerializeValue Encode()
		{
			return FunctionToEncode(AddressOfObject);
		}

		inline void Decode( const SerializeValue &Val )
		{
			FunctionToDecode( Val, AddressOfObject );
		}
	};

	SimpleList<EncodeMapping*> SerialiseMapping;

	// Find in local mapping
	EncodeMapping * Find( const SimpleString& Key );

	// Create in local mapping
	EncodeMapping * Create( const SimpleString& Key ) throw (SimpleException);

	bool SerializationDeclared;
	inline void CallDeclareSerializeMappingIfNeeded()
	{
		if ( SerializationDeclared == true )
		{
			return;
		}

		SerializationDeclared = true;
		DeclareSerializeMapping();
	}
};

#define AddVarToSerialization(a) AddToSerialization(#a, a);

SerializeValue Serialize( Serializable& Data );
void Unserialize( const SimpleString& Val, Serializable * pData );
void Unserialize( const SimpleString& Val, Serializable& Data );
void Unserialize( const SerializeValue& Val, Serializable * pData );
void Unserialize( const SerializeValue& Val, Serializable& Data );

inline void UnserializeSerializableFromAddress( const SerializeValue& Val, void * pData )
{
	Unserialize( Val, (Serializable *)pData );
}

inline SerializeValue SerializeSerializableFromAddress( void * pData )
{
	return Serialize( *(Serializable*)pData );
}


template <typename CurrentType>
void Serializable::AddToSerialization( const SimpleString& Key, SimpleList<CurrentType>& Val )
{
	// template SerializeSimpleListFromAddress<CurrentType>( SimpleList<CurrentType> * pAddress );

	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = (SerializeFunction)SerializeSimpleListFromAddress<CurrentType>;
	tmpMapping->FunctionToDecode = (UnserializeFunction)UnserializeSimpleListFromAddress<CurrentType>;
}

template <typename CurrentType>
void Serializable::AddToSerialization( const SimpleString& Key, std::vector<CurrentType>& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = (SerializeFunction)SerializeStdVectorFromAddress<CurrentType>;
	tmpMapping->FunctionToDecode = (UnserializeFunction)UnserializeStdVectorFromAddress<CurrentType>;
}

template <typename CurrentType>
void Serializable::AddToSerialization( const SimpleString& Key, std::list<CurrentType>& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = (SerializeFunction)SerializeStdListFromAddress<CurrentType>;
	tmpMapping->FunctionToDecode = (UnserializeFunction)UnserializeStdListFromAddress<CurrentType>;
}

} // Omiscid

#endif // __SERIALIZABLE_H__

