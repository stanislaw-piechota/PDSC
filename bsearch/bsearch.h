// Copyright © Julian Bednarek (Juliuszz666)

#include <stdint.h>
#include <stddef.h>

typedef struct {
    int test_key;
    char *test_value;
} test_t;

// Copyright © Stanislaw Piechota (stanislaw-piechota)

int compareInt(const void *num1, const void* num2);
int compareDouble(const void *num1, const void* num2);
int compareChar(const void *char1, const void* char2);
int compareStruct(const void *struct1, const void* struct2);
void *convertIntToPtr(__intptr_t address, int index, size_t size);
void *bsearch(const void *key, const void *base, size_t num,
              size_t size, int (*compare)(const void *, const void *));