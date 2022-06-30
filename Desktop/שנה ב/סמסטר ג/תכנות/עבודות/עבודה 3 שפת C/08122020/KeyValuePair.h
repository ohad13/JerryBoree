#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H

#include "Defs.h"

typedef struct t_KeyValuePair *Pair;

Pair createKeyValuePair(Element key, Element value, CopyFunction copyKey, CopyFunction copyVal, FreeFunction freeKey,
                        FreeFunction freeVal, EqualFunction equalFunc,
                        PrintFunction printKey, PrintFunction printVal);

status destroyKeyValuePair(Pair pair);

status displayValue(Pair pair);

status displayKey(Pair pair);

Element getValue(Pair pair);

Element getKey(Pair pair);

bool isEqualKey(Pair pair1, Element key);

#endif //KEYVALUEPAIR_H
