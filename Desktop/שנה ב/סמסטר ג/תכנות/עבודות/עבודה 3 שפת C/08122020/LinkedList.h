#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Defs.h"

typedef struct t_GenLL *GenLL;

GenLL createLinkedList(CopyFunction copyf, FreeFunction freef, EqualFunction equalFunc, PrintFunction printFunc);

status destroyList(GenLL list);

status appendNode(GenLL list, Element val);

status deleteNode(GenLL list, Element val);

status displayList(GenLL list);

Element getDataByIndex(GenLL list, int index);

int getLengthList(GenLL list);

Element searchByKeyList(GenLL list, Element key);

status displayListDataByKey(GenLL list, Element key);

#endif //LINKEDLIST_H
