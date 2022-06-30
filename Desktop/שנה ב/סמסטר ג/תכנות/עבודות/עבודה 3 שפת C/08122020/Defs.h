#ifndef DEFS_H
#define DEFS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
//macros to use is toUpper methode
#define IS_LOWER(c) ((c) >= 'a' && (c) <='z')
#define TO_UPPER(c) (IS_LOWER(c)? (c) - 'a' + 'A' : (c))

#define BUFFER_SIZE 300 //the program max buffer size
#define EXIT_CODE 10 //the exit code we wish to use when we exit
#define HASH_SIZE 1 //the size of the HashTable

typedef enum e_status {
    success, failure, memoryFail
} status;

typedef void *Element;

typedef Element(*CopyFunction)(Element);

typedef status(*FreeFunction)(Element);

typedef status(*PrintFunction)(Element);

typedef int(*TransformIntoNumberFunction)(Element);

typedef bool(*EqualFunction)(Element, Element);

#endif