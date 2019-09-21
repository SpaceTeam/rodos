
/**
 * @file commbuffer.h
 * @date 2008/05/25 16:20
 * @author Sergio Montenegro, Lutz Dittrich
 *
 *
 * @brief simple buffer for asynchronous communication (header)
 */

#pragma once

#include "putter.h"

namespace RODOS {

/**
 * @class CommBuffer
 * @brief A simple buffer for asynchronous communication
 *
 * CommBuffer can be used to interchange data asynchronously.
 * both sides (reader/writer) can work simultaneously and the data
 * will be always consistent.
 * There can be only one reader and only one writer.
 * Each time the reader reads, it gets the latest data written.
 * Due to fast switching between the buffers, the delay in simultaneous access
 * is only short.
 * @param Type Type of the data to synchronise by CommBuffer
 */

template <typename Type> class CommBuffer : public Putter {

protected:
    /// Data area consisting of three independent parts to avoid access conflicts.
    Type buffer[3];
    /// Pointer to buffer area that is currently written or will be written next.
    Type* writer;
    /// Pointer to buffer where last data is stored.
    volatile Type* reader;
    /// Additional buffer, to avoid conflicts between simultaneous read-write access.
    Type* intermediate;
    /** Flag to protect buffer. It indicates access to the current read buffer 
     * in order to prevent data conflicts.
     */
    volatile bool readingNow;
    volatile bool newDataAvailable;

public:
  uint64_t writeCnt;
  uint64_t readCnt;

  CommBuffer() {
    writer = &buffer[0];
    reader = &buffer[1];
    intermediate = &buffer[2];
    readingNow = false;
    newDataAvailable = false;
    writeCnt = readCnt = 0;
  }

  /**
   * Implements the generic interface of class Putter.
   * Put methods should only used by one thread.
   * @param msgLen Length of the message.
   * @param data Pointer to the message data to put.
   * @return true if the put operation is successful, else false. If the
   * msgLen doesn't match the size of the template type the method is never
   * successful.
   */
  bool putGeneric(const long topicId, const unsigned int len, const void* msg, const NetMsgInfo& netMsgInfo) {
    if(len > sizeof(Type)) {
      ERROR("combuffer wrong len");
      return false;
    }
    put(*(Type*)msg);
    return true;
  }

  /**
   * Implements the put operation of data to the CommBuffer. The data is
   * stored in the buffer, and swap read and write buffer for next write.
   * Put methods should only used by one thread.
   * @param data Reference of the message data to put.
   */
  void put(Type& data) {
    *writer = data;

    /* Swap reader <-> writer in order to make them read in the next call to get. */
    Type* swap = (Type*)reader;
    reader = writer;
    writer = swap;

    /* Buffer being read now? Then take the intermediate buffer for the next write. */
    if(readingNow) {
      swap = writer;
      writer = intermediate;
      intermediate = swap;
    }
    writeCnt++;
    newDataAvailable = true;
  }

  /*
   * Writes a "Zero" record
   */
  void putZero() {
    bzero(writer, sizeof(Type));
    put(*writer);
  }

  /**
   * Get the next consistent message from the CommBuffer.
   * The method get should only used by one thread.
   * @param data Reference of the message data to get.
   */
  void get(Type &data) {
	newDataAvailable = false;
    readingNow = true;
    Type* readerTmp = (Type*)reader; // This is assumed to be atomar
    data = *readerTmp;		  // this is not atomar
    readingNow = false;
    readCnt++;
  }
 
 /** like get, but modify out parameter only if there is new data **/

  bool getOnlyIfNewData(Type &data) {
    if(newDataAvailable) {
       get(data);
       return true;
    }
    return false;
  }
};

}

