#ifndef MANAGEJERRIES_MULTIVALUEHASHTABLE_H
#define MANAGEJERRIES_MULTIVALUEHASHTABLE_H

#include "Defs.h"
#include "LinkedList.h"

typedef struct t_MultiValueHashTable *MVHT;

MVHT createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,EqualFunction equalValue,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber);

status destroyMultiValueHashTable(MVHT multiHash);

status addToMultiValueHashTable(MVHT multiHash, Element key, Element value);

GenLL lookupInMultiValueHashTable(MVHT multiHash, Element key);

status removeFromMultiValueHashTable(MVHT multiHash, Element key, Element value);

status displayMultiValueHashElementsByKey(MVHT multiHash, Element key);

#endif //MANAGEJERRIES_MULTIVALUEHASHTABLE_H