/** @file Messaging/Serializable.cpp
 * @ingroup Messaging
 * @ingroup UserFriendlyAPI
 * @brief Definition of Serializable class
 * @date 2004-2011
 * @author Dominique Vaufreydaz
 */

#include <Messaging/Serializable.h>

using namespace Omiscid;


Serializable::Serializable()
	: SerializationDeclared(false)
{
}

Serializable::~Serializable()
{
	SmartLocker SL_this((const LockableObject&)*this);

	while( SerialiseMapping.IsNotEmpty() )
	{
		delete SerialiseMapping.ExtractFirst();
	}
}

#if 0

 /** \find Find an element identified by Key
  * @param Key [in] the key to identifies the pair.
  * @return true if found, false otherwise
  */
SerializeObjectIterator Serializable::Find( const SimpleString& Key, SerializeObject& SerializeManager )
{
	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	SerializeObjectIterator it;

	for (it = SerializeManager.begin(); it!=SerializeManager.end(); ++it)
	{
		if ( same_name( *it, Key ) == true )
		{
			break;
		}
	}

	// Will be JSonManager.end()
	return it;
}

#endif

Serializable::EncodeMapping * Serializable::Find( const SimpleString& Key )
{
	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	for( SerialiseMapping.First(); SerialiseMapping.NotAtEnd(); SerialiseMapping.Next() )
	{
		EncodeMapping * tmpMapping = SerialiseMapping.GetCurrent();
		if ( tmpMapping->Key.EqualsCaseInsensitive( Key ) )
		{
			return tmpMapping;
		}
	}

	return (Serializable::EncodeMapping*)NULL;
}

Serializable::EncodeMapping * Serializable::Create( const SimpleString& Key ) throw (SimpleException)
{
	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Find( Key );

	if ( tmpMapping == (Serializable::EncodeMapping*)NULL )
	{
		// Create and add structure to the list
		tmpMapping = new OMISCID_TLM EncodeMapping;
		tmpMapping->Key = Key;
		SerialiseMapping.AddTail(tmpMapping);
	}
	else
	{
		SimpleString Msg = "Replacing serialise mapping for " + Key;
		throw SimpleException( Msg );
	}

	return tmpMapping;
}

void Serializable::AddToSerialization( const SimpleString& Key, long& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeLongFromAddress;
	tmpMapping->FunctionToDecode = UnserializeLongFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, int& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeIntFromAddress;
	tmpMapping->FunctionToDecode = UnserializeIntFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, unsigned int& Val )
{
	// TODO
	throw SerializeException("unsigned int not supported, please send as a string", SerializeException::UnsupportedType );
}

void Serializable::AddToSerialization( const SimpleString& Key, short int& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeShortIntFromAddress;
	tmpMapping->FunctionToDecode = UnserializeShortIntFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, unsigned short& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeUnsignedShortFromAddress;
	tmpMapping->FunctionToDecode = UnserializeUnsignedShortFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, double& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeDoubleFromAddress;
	tmpMapping->FunctionToDecode = UnserializeDoubleFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, float& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeFloatFromAddress;
	tmpMapping->FunctionToDecode = UnserializeFloatFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, bool& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeBoolFromAddress;
	tmpMapping->FunctionToDecode = UnserializeBoolFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, SimpleString& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeSimpleStringFromAddress;
	tmpMapping->FunctionToDecode = UnserializeSimpleStringFromAddress;
}

void Serializable::AddToSerialization( const SimpleString& Key, char *& Val )
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	Serializable::EncodeMapping * tmpMapping = Create( Key );

	// Fill (new) structure
	tmpMapping->AddressOfObject = (void*)&Val;
	tmpMapping->FunctionToEncode = SerializeCharStarFromAddress;
	tmpMapping->FunctionToDecode = UnserializeCharStarFromAddress;
}

SerializeValue Serializable::Serialize()
{
	SmartLocker SL_this((const LockableObject&)*this);

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	// Call Pre serializable function
	PreSerializableFonction();

	StructuredMessage MySMsg;

	for( SerialiseMapping.First(); SerialiseMapping.NotAtEnd(); SerialiseMapping.Next() )
	{
		Serializable::EncodeMapping * tmpMapping = SerialiseMapping.GetCurrent();

		MySMsg.Put( tmpMapping->GetKey(), tmpMapping->Encode() );
	}

	return MySMsg;
}

void Serializable::Unserialize( const SimpleString& SerializedVal )
{
	SmartLocker SL_this((const LockableObject&)*this);

	StructuredMessage sMsg( SerializedVal );

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	// Parse serialising objet
	for( SerialiseMapping.First(); SerialiseMapping.NotAtEnd(); SerialiseMapping.Next() )
	{
		Serializable::EncodeMapping * tmpMapping = SerialiseMapping.GetCurrent();

		tmpMapping->Decode( sMsg.FindAndGetValue( tmpMapping->Key ) );
	}

	// Call Post serializable function
	PostSerializableFonction();
}


void Serializable::Unserialize( const SerializeValue& SerializedVal )
{
	SmartLocker SL_this((const LockableObject&)*this);

	StructuredMessage sMsg( SerializedVal );

	// Check if SerializeMappingIsDone
	CallDeclareSerializeMappingIfNeeded();

	// Parse serialising objet
	for( SerialiseMapping.First(); SerialiseMapping.NotAtEnd(); SerialiseMapping.Next() )
	{
		Serializable::EncodeMapping * tmpMapping = SerialiseMapping.GetCurrent();

		tmpMapping->Decode( sMsg.FindAndGetValue( tmpMapping->Key ) );
	}

	// Call Post serializable function
	PostSerializableFonction();
}

namespace Omiscid {

SerializeValue Serialize( Serializable& Data ) { return Data.Serialize(); }
void Unserialize( const SimpleString& Val, Serializable * pData ) { pData->Unserialize(Val); }
void Unserialize( const SimpleString& Val, Serializable& Data ) { Data.Unserialize(Val); }
void Unserialize( const SerializeValue& Val, Serializable * pData ) { pData->Unserialize(Val); }
void Unserialize( const SerializeValue& Val, Serializable& Data ) { Data.Unserialize(Val); }

} // namespace Omiscid