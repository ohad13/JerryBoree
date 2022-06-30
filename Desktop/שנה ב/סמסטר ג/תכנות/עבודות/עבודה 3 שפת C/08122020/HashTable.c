#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"
#include <stdlib.h>
#include <string.h>

Element copy_kvp(Element elem) { //TODO: Copy, Shallow or Deep? now deep !change!
    if (!elem)
        return NULL;
    return (Pair) elem;
}

status free_kvp(Element elem) {
    if (!elem)
        return memoryFail;
    return destroyKeyValuePair((Pair) elem);
}

status print_kvp(Element elem) {
    if (!elem)
        return memoryFail;
    if (displayKey((Pair) elem) == memoryFail || displayValue((Pair) elem) == memoryFail)
        return memoryFail;
    return success;
}

bool equal_kvp(Element elem1, Element elem2) {

    return isEqualKey((Pair) elem1, elem2);
}

typedef struct hashTable_s {
    GenLL *table;
    int tableSize;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;
} Hash;

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue || !equalKey ||
        !transformIntoNumber)
        return NULL;
    hashTable hash = (hashTable) malloc(sizeof(Hash));
    if (!hash){ //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    hash->tableSize = hashNumber;
    hash->table = (GenLL *) malloc(sizeof(GenLL) * hash->tableSize);
    if (!hash->table){ //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    int i;
    for (i = 0; i < hash->tableSize; ++i) {
        hash->table[i] = createLinkedList(copy_kvp, free_kvp, equal_kvp, print_kvp);
        if (!hash->table[i])
            return NULL;
    }

    hash->copyKey = copyKey;
    hash->copyValue = copyValue;
    hash->freeKey = freeKey;
    hash->freeValue = freeValue;
    hash->equalKey = equalKey;
    hash->printKey = printKey;
    hash->printValue = printValue;
    hash->transformIntoNumber = transformIntoNumber;
    return hash;
}

status destroyHashTable(hashTable hash) {
    if (!hash)
        return memoryFail;
    int i;
    for (i = 0; i < hash->tableSize; ++i) {
        if (destroyList(hash->table[i]) == memoryFail)
            return memoryFail;
        hash->table[i] = NULL;
    }
    free(hash->table);
    hash->table = NULL;
    free(hash);
    hash = NULL;
    return success;
}

status addToHashTable(hashTable hash, Element key, Element value) {
    if (!hash || !key || !value)
        return memoryFail;
    Pair kvp = createKeyValuePair(key, value, hash->copyKey, hash->copyValue, hash->freeKey, hash->freeValue,
                                  hash->equalKey, hash->printKey, hash->printValue);
    if (!kvp)
        return memoryFail;
    int map = (int) hash->transformIntoNumber(key) % hash->tableSize;// TODO: changed the original func
    if (!searchByKeyList(hash->table[map], key)) //TODO: key in hash??
        return appendNode(hash->table[map], kvp);
    return failure; //key already in hashTable!!
}

Element lookupInHashTable(hashTable hash, Element key) {
    if (!hash || !key)
        return NULL;
    int map = (int) hash->transformIntoNumber(key) % hash->tableSize;
    Pair keyValue = searchByKeyList(hash->table[map], key);
    return getValue(keyValue);
}

status removeFromHashTable(hashTable hash, Element key) {
    if (!hash || !key)
        return memoryFail;
    int map = (int) hash->transformIntoNumber(key) % hash->tableSize;
    return deleteNode(hash->table[map], key);
}

status displayHashElements(hashTable hash) {
    if (!hash)
        return memoryFail;

    // TODO !!!
    int i;
    for (i = 0; i < hash->tableSize; ++i) {
        status s = displayList(hash->table[i]);
        if (s != success)
            return s;
    }
    return success;
}

status displayHashElementByKey(hashTable hash, Element key) {
    if (!hash || !key)
        return memoryFail;

    int map = (int) hash->transformIntoNumber(key) % hash->tableSize;
    return displayListDataByKey(hash->table[map], key);
}

Pair lookupForPair(hashTable hash, Element key) { //TODO: REMOVE
    if (!hash || !key)
        return NULL;
    int map = (int) hash->transformIntoNumber(key) % hash->tableSize;
    Pair keyValue = searchByKeyList(hash->table[map], key);
    return keyValue;
}