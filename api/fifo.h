

/**
* @file fifo.h
* @date 2008/05/11 16:13
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief FIFO (first in first out) communication (header)
*
*/

#pragma once

#include "putter.h"
#include "rodos-debug.h"
#include "thread.h"

namespace RODOS {

/**
*  @class Fifo
*  @brief simple FIFO (realized as ring buffer)
*
*  A fifo where both sides (reader & writer) can work asynchronously.
*  If one side has more than one owner, the fifo has to be
*  protected using priority_ceiling, or semaphores.
*
*  @param Type    data type of fifo entries
*  @param len     maximal number of entries must be at least 2 (real capacity = len-1)
*
*/
template < typename Type, size_t len > class Fifo : public Putter {

protected:

    Type buffer[len];

    volatile size_t writeX;
    volatile size_t readX;

    /** advance index to next position
    * with overflow to 0 to implement a ring
    */
    size_t advanceIndex(size_t index) {
        return ((index < (len-1)) ? (index+1) : 0);
    }


public:

    Fifo() {
        readX = 0;
        writeX = 0;
    }

    /** implements the generic interface of putter */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        RODOS_ASSERT_IFNOT_RETURN(msgLen <= sizeof(Type), false);
        return put(*(const Type*)msg);
    }

    /**  returns true == ok, false == fifo full */
    bool put(const Type& val) {
        size_t index =  advanceIndex(writeX);
        if(index == readX) {
            return false; /* full! */
        }

        buffer[writeX] = val;
        writeX = index;
        return true;
    }

    /** return true == ok, false = fifo empty, val not modified */
    bool get(Type& val) {
        if(readX == writeX) {
            return false;
        }
        val = buffer[readX];
        readX = advanceIndex(readX);
        return true;
    }

    size_t getLen() { return len; }

    size_t getElementCount() { ///< warning: not thread safe
        size_t r = readX;
        size_t w = writeX;
        return (r <= w) ? (w-r) : (len-r+w);
    }
    size_t getFreeSpaceCount() { ///< warning: not thread safe
        return len - getElementCount() - 1;
    }
    bool isFull()  { return advanceIndex(writeX)==readX; } ///< warning: not thread safe
    bool isEmpty() { return readX == writeX;}              ///< warning: not thread safe
    void clear()   { readX = writeX = 0; }                 ///< erases all content
};

/**********************************************************************/

/**
* @class SyncFifo
* @brief A synchronized fifo.
*
* Like Fifo, but it provides syncPut which block if the fifo is
* full and syncRead which block if the fifo is empty.
* Each function may have a timeout. If no timeout is given, or timeout == 0
* it will wait until the fifo is not empty/full.
* Warning:
* Only for one writer and one reader. If more than that is required it has
* to be protected using semaphores.
*
*/
template <class Type, size_t len> class SyncFifo : public Fifo<Type, len> {

protected:
    Thread* suspendedReader;
    Thread* suspendedWriter;

public:

    SyncFifo() {
        suspendedReader = 0;
        suspendedWriter = 0;
    }


    /**
    * Implements the generic interface of putter:
    * Warning does not suspends if fifo full
    */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        RODOS_ASSERT_IFNOT_RETURN(msgLen <= sizeof(Type), false);

        bool ok = this->put(*(const Type*)msg);
        {
            PRIORITY_CEILER_IN_SCOPE();
            if (suspendedReader!=0)  suspendedReader->resume();
        }
        return ok;
    }

    /**  suspends while fifo is full. Returns true == ok, false == timeout, */
    bool syncPut(const Type &val, const int64_t timeout = END_OF_TIME) {
        bool ok = false;
        {
            PRIORITY_CEILER_IN_SCOPE();
            ok = this->put(val);
            if (!ok) {
                suspendedWriter = 	Thread::getCurrentThread();
                switch (timeout) {
                case 0:
                    break;
                case END_OF_TIME:
                    Thread::suspendCallerUntil(END_OF_TIME);
                    break;
                default:
                    Thread::suspendCallerUntil(NOW() + timeout);
                }
                suspendedWriter = 0; // after suspend, after resume
            }
        }

        if (!ok && timeout > 0) { ok = this->put(val); }
        if (ok) {
            PRIORITY_CEILER_IN_SCOPE();
            if(suspendedReader) suspendedReader->resume();
        }
        return ok;
    }

    /** return true == ok, false = fifo empty, val not modified */
    bool syncGet(Type &val, const int64_t timeout = END_OF_TIME) {
        bool ok = false;
        {
            PRIORITY_CEILER_IN_SCOPE();
            ok= this->get(val);
            if (!ok) {
                suspendedReader = Thread::getCurrentThread();
                switch (timeout) {
                case 0:
                    break;
                case END_OF_TIME:
                    Thread::suspendCallerUntil(END_OF_TIME);
                    break;
                default:
                    Thread::suspendCallerUntil(NOW() + timeout);
                }
                suspendedReader = 0;
            }
        }

        if (!ok && timeout > 0) { ok = this->get(val); }

        if(ok) {
            PRIORITY_CEILER_IN_SCOPE();
            if (suspendedWriter) suspendedWriter->resume();
        }
        return ok;
    }

};


template <typename Type, size_t len, uint32_t numOfreaders >
class MultipleReaderFifo : public Putter {

protected:

    Type buffer[len];

    volatile size_t writeX;
    volatile size_t readX[numOfreaders];
    uint32_t readerCnt; ///< used only to generate readerId, if user wishes!

    /** advance index to next position
    * with overflow to 0 to implement a ring
    */
    size_t advanceIndex(size_t index) {
        return ((index < (len-1)) ? (index+1) : 0);
    }


public:

    MultipleReaderFifo() {
        readerCnt = 0;
        writeX = 0;
        for(uint32_t i = 0; i < numOfreaders; i++) readX[i] = 0;
    }

    int32_t getReaderId() { //< Warning: not thread safe!!!
        uint32_t myId = readerCnt++;
        if(myId >= numOfreaders) return -1; // no more readers!
        return static_cast<int32_t>(myId);
    }

    /** implements the generic interface of putter */
    bool putGeneric([[gnu::unused]] const uint32_t topicId, const size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        RODOS_ASSERT_IFNOT_RETURN(msgLen <= sizeof(Type), false);
        return put(*(const Type*)msg);
    }

    /**  returns true == ok, false == fifo full */
    bool put(const Type& val) {
        size_t index =  advanceIndex(writeX);
        for(uint32_t i = 0; i < numOfreaders; i++) {
            if(index == readX[i]) { // one reading is to slow -> shift him!
                readX[i] = advanceIndex(readX[i]); // Warning: thread safe?!?!
            }
        }

        buffer[writeX] = val;
        writeX = index;
        return true;
    }

    /** return true == ok, false = fifo empty, val not modified */
    bool get(Type& val, uint32_t readerId) {
        if(readerId >= numOfreaders)  return false;
        if(readX[readerId] == writeX) return false;

        val = buffer[readX[readerId]];
        readX[readerId] = advanceIndex(readX[readerId]);
        return true;
    }

    ///
    int getLen() { return len; }
};


/**
  * A Fifo that lets you write or read continuous Blocks of the stored type.
  * Gives out pointers to the internal space, so you can copy data directly from/to there.
  * Useful for DMA transfers.
  */

template <class Type, size_t len>  class BlockFifo : public Fifo<Type,len> {
protected:
    size_t advanceIndexEx(size_t index,size_t advanceSize) {
        size_t newIndex = index + advanceSize;
        while(newIndex >= len) {
            newIndex -= len;
        }
        return newIndex;
    }


public:
    BlockFifo() { }


    /// returns pointer to write and maximal number of entries you may write
    Type* getBufferToWrite(size_t& maxLen) {
        size_t r = this->readX;
        size_t w = this->writeX;

        if(r == this->advanceIndex(w)) { //full
            maxLen =0;
            return 0;
        }

        if(r > w) { // put till r-1 possible
            maxLen = r - w - 1;
        } else { // put until end of buffer possible
            maxLen = len - w;
            if(r == 0) { maxLen--; }
        }
        return &(this->buffer[w]);
    }

    /// call this after you have written in the buffer you got from getBufferToWrite
    void writeConcluded(size_t numOfWrittenElements) {
        this->writeX = advanceIndexEx(this->writeX, numOfWrittenElements);
    }

    // returns pointer to read and maximal number of entries you may read
    Type* getBufferToRead(size_t& maxLen) {
        size_t r = this->readX;
        size_t w = this->writeX;

        if(r == w) { //empty
            maxLen =0;
            return 0;
        }

        if(r < w) { // get till w-1 possible
            maxLen = w - r;
        } else { // get until end of buffer possible
            maxLen = len - r;
        }
        return &(this->buffer)[r];
    }

    /// call this after you have read in the buffer you got from getBufferToRead
    void readConcluded(size_t sizeRed) {
        this->readX=advanceIndexEx(this->readX,sizeRed);
    }

};


}  // namespace

