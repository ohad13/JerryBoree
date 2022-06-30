#include "LinkedList.h"
#include <stdlib.h>
//#include "Jerry.h"

typedef struct t_Node {
    Element data;
    struct t_Node *next;
} Node;

typedef struct t_GenLL {
    Node *head;
    Node *tail;
    int size;
    CopyFunction copyFunc;
    FreeFunction freeFunc;
    EqualFunction equalFunc;
    PrintFunction printFunc;
} LL;

Node *createNode(Element data) {
    if (!data)
        return NULL;
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL){ //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

GenLL createLinkedList(CopyFunction copyf, FreeFunction freef, EqualFunction equalFunc, PrintFunction printFunc) {
    if (!copyf || !freef || !equalFunc || !printFunc)
        return NULL;
    GenLL list = (GenLL) malloc(sizeof(LL));
    if (!list){ //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->copyFunc = copyf;
    list->freeFunc = freef;
    list->equalFunc = equalFunc;
    list->printFunc = printFunc;
    return list;
}

status destroyList(GenLL list) {
    if (!list)
        return memoryFail;
    Node *holder, *tmp;
    holder = list->head;
    int i;
    for (i = 0; i < list->size; ++i) {
        tmp = holder->next;
        list->freeFunc(holder->data);
        free(holder);
        holder = tmp;
    }
    free(list);
    list = NULL;
    return success;
}

status appendNode(GenLL list, Element data) {
    if (!list || !data)
        return memoryFail;

    Node *newNode = createNode(list->copyFunc(data));
    if (!newNode)
        return memoryFail;
    if (list->size == 0) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
    return success;
}

status deleteNode(GenLL list, Element data) {
    if (!list || !data)
        return memoryFail;
    Node *hold = list->head, *prev;
    int i;
    for (i = 0; i < list->size; ++i) {
        if (list->equalFunc(hold->data, data)) {
            if (i == 0) { // if we delete the first Node
                if (list->size == 1)
                    list->tail = NULL;
                list->head = hold->next;
            } else if (i == list->size - 1) {// if we delete the last Node
                list->tail = prev;
                prev->next = NULL;
            } else // between head and tail
                prev->next = hold->next;

            //free DATA + HOLD
            list->freeFunc(hold->data);
            hold->data = NULL;
            free(hold);
            hold = NULL;
            list->size--;
            return success;

        } else {
            prev = hold;
            hold = hold->next;
        }
    }
    return failure;
}

status displayList(GenLL list) {
    if (!list)
        return memoryFail;
    Node *node = list->head;
    int i;
    for (i = 0; i < list->size; ++i) {
        list->printFunc(node->data);
        node = node->next;
    }
    return success;
}

//TODO think about O(..)
Element getDataByIndex(GenLL list, int index) {
    if (!list)
        return NULL;

    if (index < 1 || list->size < index)
        return NULL;
    Node *node = list->head;
    int i;
    for (i = 0; i < index - 1; ++i)
        node = node->next;
    return node->data;
}

int getLengthList(GenLL list) {
    if (!list)
        return -1;
    return list->size;
}

// search PC in multiHash , key=pc name
Element searchByKeyList(GenLL list, Element key) {
    if (!list || !key)
        return NULL;
    Node *node = list->head;
    int i;
    for (i = 0; i < list->size; ++i) {
        if (list->equalFunc(node->data, key))
            return node->data;
        node = node->next;
    }
    return NULL;
    // 😃 ctrl+alt+;
}

status displayListDataByKey(GenLL list, Element key) {
    if (!list || !key)
        return memoryFail;
    Element element = searchByKeyList(list, key);
    if (!element)
        return failure;
    return list->printFunc(element);
    // 😃 ctrl+alt+;
}