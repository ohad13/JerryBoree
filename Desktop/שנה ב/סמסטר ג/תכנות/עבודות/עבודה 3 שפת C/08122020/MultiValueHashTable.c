#include "MultiValueHashTable.h"
#include "HashTable.h"
#include <stdlib.h>


Element copy_LL(Element elem) {
    if (!elem)
        return NULL;
    GenLL copy = (GenLL) elem;
    return copy;
}

status free_LL(Element elem) {
    if (!elem)
        return memoryFail;
    return destroyList((GenLL) elem);
}

status print_LL(Element elem) {
    if (!elem)
        return memoryFail;
    return displayList((GenLL) elem);
}

typedef struct t_MultiValueHashTable {
    hashTable hashTable1;
    CopyFunction copyValue;
    FreeFunction freeValue;
    EqualFunction equalValue;
    PrintFunction printValue;
} MultiHash;

MVHT
createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          EqualFunction equalValue, TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue || !equalKey || !equalValue ||
        !transformIntoNumber)
        return NULL;
    MultiHash *multiHash = (MultiHash *) malloc(sizeof(MultiHash));
    if (!multiHash){ //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    multiHash->hashTable1 = createHashTable(copyKey, freeKey, printKey, copy_LL, free_LL, print_LL, equalKey,
                                            transformIntoNumber, hashNumber);
    multiHash->copyValue = copyValue;
    multiHash->freeValue = freeValue;
    multiHash->equalValue = equalValue;
    multiHash->printValue = printValue;
    return multiHash;
}

status destroyMultiValueHashTable(MVHT multiHash) {
    if (!multiHash)
        return memoryFail;
    if (destroyHashTable(multiHash->hashTable1) != success)
        return memoryFail;
    free(multiHash);
    multiHash = NULL;
    return success;
}

status addToMultiValueHashTable(MVHT multiHash, Element key, Element value) {
    if (!multiHash || !key || !value)
        return memoryFail;

    GenLL list = lookupInMultiValueHashTable(multiHash, key);
    if (!list) {
        GenLL newlist = createLinkedList(multiHash->copyValue, multiHash->freeValue, multiHash->equalValue,
                                         multiHash->printValue);
        if (!newlist)
            return memoryFail;
        appendNode(newlist, value);
        return addToHashTable(multiHash->hashTable1, key, newlist);
    } else
        return appendNode(list, value);
}

GenLL lookupInMultiValueHashTable(MVHT multiHash, Element key) {
    return lookupInHashTable(multiHash->hashTable1, key);
}

status removeFromMultiValueHashTable(MVHT multiHash, Element key, Element value) { // value or part of him
    // key - pc name
    // value - jerry
    if (!multiHash || !key || !value)
        return memoryFail;
    //search for the key
    GenLL list = lookupInMultiValueHashTable(multiHash, key);
    //list - all the jerries with this(key) pc.
    if (!list)
        return failure;
    Element elem = searchByKeyList(list, value);
    // elem - the jerry we need to delete
    if (!elem)
        return failure;
    if (deleteNode(list, elem) == memoryFail)
        return memoryFail;

    //check the list size
    if (getLengthList(list) == 0) {
//        destroyList(list);
        removeFromHashTable(multiHash->hashTable1, key);
    }
    return success;
}

status displayMultiValueHashElementsByKey(MVHT multiHash, Element key) {
    if (!multiHash || !key)
        return memoryFail;
//    GenLL list = lookupInMultiValueHashTable(multiHash, key);

    return displayHashElementByKey(multiHash->hashTable1, key);
}