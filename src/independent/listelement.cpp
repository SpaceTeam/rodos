



/**
* @file listelement.cc
* @date 2008/04/26 17:12
* @author Sergio Montenegro, Lutz Dittrich
*
*
* Listelement: basic object for operating system objects
*
* @brief linked list management
*/

#include "rodos.h"
#include "hw_specific.h"


namespace RODOS {

int32_t ListElement::listElementCounter = 0;
/*
* create an element and add to the beginning of the list (unsorted list)
*/
ListElement::ListElement(List& list, const char* name, void* myOwner) {
    owner = myOwner;
    this->name = name;
    listElementID = listElementCounter++;
    append(list);
}

void ListElement::append(List& list) {
    next = list;
    list = this;
    RODOS_ASSERT(!taskRunning); // Listelement::Constructor after sys initialisation List was created not static
}

/*
*  destructor
*/
ListElement::~ListElement() {
    RODOS_ERROR("ListElement deleted");
}


}
