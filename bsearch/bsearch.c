// Copyright © Stanisaw Piechota (stanislaw-piechota)

#include "bsearch.h"
#include <stdint.h>

int compareInt(const void *num1, const void *num2)
{
    int *left = (int *)num1, *right = (int *)num2;
    return (*left > *right) - (*left < *right);
}

int compareDouble(const void *num1, const void *num2)
{
    double *left = (double *)num1, *right = (double *)num2;
    return (*left > *right) - (*left < *right);
}

int compareChar(const void *char1, const void *char2)
{
    char *left = (char *)char1, *right = (char *)char2;
    return (*left > *right) - (*left < *right);
}

int compareStruct(const void *struct1, const void *struct2)
{
    int left = ((test_t *)struct1)->test_key, right = ((test_t *)struct2)->test_key;
    return (left > right) - (left < right);
}

void *convertIntToPtr(__intptr_t address, int index, size_t size){
    return (void *)(address + index * size);
}

void *bsearch(const void *key, const void *base, size_t num,
              size_t size, int (*compare)(const void *, const void *))
{
    int start = 0, end = num - 1, middle = (start + end) / 2 + 1;
    __intptr_t baseAddress = (__intptr_t) base;

    while (end > start)
    {
        switch ((*compare)(key, convertIntToPtr(baseAddress, middle, size)))
        {
        case 1:
            start = middle;
            middle = (start + end) / 2 + 1;
            break;
        case -1:
            end = middle;
            middle = (start + end) / 2;
            break;
        case 0:
            return convertIntToPtr(baseAddress, middle, size);
            break;
        }
    }

    return NULL;
}