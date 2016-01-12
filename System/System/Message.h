/**
 * @file Com/Message.h
 * @ingroup Com
 * @ingroup UserFriendly
 * @brief Definition of Message class
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <System/ConfigSystem.h>
#include <System/MemoryBuffer.h>
#include <System/SimpleString.h>

namespace Omiscid {

/**
 * @class Message Message.h Com/Message.h
 * @ingroup Com
 * @ingroup UserFriendly
 * @brief Group Data about a received message
 *
 * contain the data about a received message :
 * - the length of the message
 * - the message
 * - a pointer on the source of the message
 * - the PerrId of the sender
 * - the id for the message
 */
class Message : public MemoryBuffer
{
   /**
	* @brief The MsgSocket class can acces directly to private members of this class
	*/
	friend class MsgSocket;
   /**
	* @brief The MsgManager class can acces directly to private members of this class
	*/
	friend class MsgManager;

public:
  /** @brief Constructor
   *
   * Allocates a size+1 bytes buffer to contains the data of a message
   * (the added byte is kept before the message, so buffer is a pointer
   * on a buffer of length 'size').
   * @param size [in] the size of the buffer containing the message
   * if size = 0 (default), no allocation are done. Usefull for pointer
   * container.
   */
  Message(size_t size = 0);

  /** @brief Copy constructor
   *
   * @param ToCopy [in] the message to copy
   * container.
   */
  Message(const Message& ToCopy);

  /** @brief Destructor
   *
   * Free the buffer if previsously allocated.
   */
  virtual ~Message() {}

  /** @name Access to information via functions */
  const char * GetBuffer() const; /*!< Get buffer to data */
  size_t GetLenght() const; /*!< Get len of data */
  MessageOrigine GetOrigine() const; /*!< The origine  of the Message (udp, tcp, shared memory, maybe other (??) in future work) */
  unsigned int GetPeerId() const; /*!< The PeerId who send this message */
  unsigned int GetMsgId() const; /*!< The unique message id comming from a Peer */
  //@}

  /** @brief ToString
   *
   * Generate a description of the message
   */
  SimpleString ToString(unsigned int MaxBufferLenToWrite = 30) const;

protected:
  /** @name Direct access to data, for members and friens classes MsgSocket and MsgManager */
  //@{
  const char * buffer;
  size_t len;
  MessageOrigine origine;
  unsigned int pid; /*!< peer id */
  unsigned int mid; /*!< message id */
  //@}
};

} // namespace Omiscid

#endif // __MESSAGE_H__

