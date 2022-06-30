#include "KeyValuePair.h"
#include <stdlib.h>

typedef struct t_KeyValuePair {
    Element key;
    Element value;
    CopyFunction copyKey;
    CopyFunction copyVal;
    FreeFunction freeKey;
    FreeFunction freeVal;
    EqualFunction equalFunc;
    PrintFunction printKey;
    PrintFunction printVal;
} KVP;

Pair createKeyValuePair(Element key, Element value, CopyFunction copyKey, CopyFunction copyVal, FreeFunction freeKey,
                        FreeFunction freeVal, EqualFunction equalFunc,
                        PrintFunction printKey, PrintFunction printVal) {
    if (!key || !value || !copyKey || !copyVal || !freeKey || !freeVal || !equalFunc || !printKey || !printVal)
        return NULL;
    Pair pair = (Pair) malloc(sizeof(KVP));
    if (!pair){ //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    pair->copyKey = copyKey;
    pair->copyVal = copyVal;
    pair->freeKey = freeKey;
    pair->freeVal = freeVal;
    pair->equalFunc = equalFunc;
    pair->printKey = printKey;
    pair->printVal = printVal;
    pair->key = pair->copyKey(key); //will save copy of the key data
    pair->value = pair->copyVal(value); //will save copy of the value data
    return pair;
}

status destroyKeyValuePair(Pair pair) {
    if (!pair)
        return memoryFail;
    pair->freeKey(pair->key);
    pair->key = NULL;
    pair->freeVal(pair->value);
    pair->value = NULL;
    free(pair);
    pair = NULL;
    return success;
}

status displayValue(Pair pair) {
    if (!pair)
        return memoryFail;
    pair->printVal(pair->value);
    return success;

}

status displayKey(Pair pair) {
    if (!pair)
        return memoryFail;
    pair->printKey(pair->key);
    return success;
}

Element getValue(Pair pair) {
    if (!pair)
        return NULL;
    return pair->value;
}

Element getKey(Pair pair) {
    if (!pair)
        return NULL;
    return pair->key;
}

//gets pair and key
bool isEqualKey(Pair pair, Element key) {
    return pair->equalFunc(pair->key, key);
}
