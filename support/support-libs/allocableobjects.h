

#pragma once
  
#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/**
 * Memory allocation with reference counter, for a "Reference Counting Garbage Collection"
 *
 * Each object has a count of the number of references to it. 
 * object can be allocated with alloc. You can copy references ONLY by using the function copyReference
 * which will increment the reference counter.
 * the allocated object and copies of references shall be released using the function free.
 * when the reference counters reaches 0, free will put the object back to the free list.
 *
 * An object's reference count is incremented when a reference to it is created, and decremented when a reference is destroyed.
 * When the count reaches zero, the object's memory is reclaimed (not automatically, but when calling free)
 *
 * Objects (see Type) has to be subclases from SortedChainable.
 * Objects may have an init method, if so it shall call the init from SortedChainable
 * Thread safe: It uses internally a semaphore
 *
 */

template <typename Type, uint32_t len> class AllocableObejcts {

    Type       buffer[len];
    uint32_t   referenceCnt[len];
    SortedList freeList;
    Semaphore  freeListsProtector;
    uint32_t    numOfFreeItems;

public:

    uint32_t getNumOfFreeItems()  { return numOfFreeItems;}
    uint32_t getIndex(Type* item) { return item - buffer; }

    void init() {
        freeListsProtector.enter(); {
            numOfFreeItems = len;
            for(uint32_t i = 0; i < len; i++) { referenceCnt[i] = 0; buffer[i].init();  }
            for(uint32_t i = 0; i < len; i++) freeList.appendForce(&buffer[i],0);
        } freeListsProtector.leave();
    }
  
    Type* alloc() {
        Type* item;
        freeListsProtector.enter(); {
            item = (Type*)freeList.getAndRemoveTheFirst();
            if(item != 0) {
                uint32_t index = item - buffer;
                if(index < len) referenceCnt[index] = 1; // the if will be allwas true!
                numOfFreeItems--;
            }
        } freeListsProtector.leave();
        return item;
    }
    
    void free(Type* item) {
        uint32_t index = item - buffer;
        if(index >= len) return;
        freeListsProtector.enter(); {
            if(referenceCnt[index] >  0) referenceCnt[index]--; // the if will be allwas true!
            if(referenceCnt[index] == 0) {
                freeList.appendForce(item, 0);
                numOfFreeItems++;
            }
        } freeListsProtector.leave();
    }

    Type* copyReference(Type* item) {
        Type*    copy  = item;
        uint32_t index = item - buffer;
        if(index >= len) return 0;

        freeListsProtector.enter(); {
           if(referenceCnt[index] > 0) referenceCnt[index]++;
           else                        copy = 0; // there shall exist no copy of it! it is in the free list!
        } freeListsProtector.leave();
        return copy;
    }

};

#ifndef NO_RODOS_NAMESPACE
}
#endif

