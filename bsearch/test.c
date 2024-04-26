// Copyright © Julian Bednarek (Juliuszz666)

#include "bsearch.h"
#include <stdio.h>

int run_test(void *arr, int n, size_t size, void *key, void *expected, int (*compare)(const void *, const void *)) {
    void *item = bsearch(key, arr, n, size, compare);
    if (item != NULL && expected != NULL && compare(item, expected) == 0) {
        return 1;
    } else if (item == NULL && expected == NULL) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    int successful_tests = 0;

    for (int i = 1; i <= 10; i++) {
        int arr_int[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        successful_tests += run_test(arr_int, 10, sizeof(int), &i, &arr_int[i - 1], compareInt);
    }
    printf("Number of successful tests: %d\n", successful_tests);
    for (int i = 1; i <= 10; i++) {
        double arr_double[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 11};
        double key_double = i + 0.1 * i;
        successful_tests += run_test(arr_double, 10, sizeof(double), &key_double, &arr_double[i - 1], compareDouble);
    }
    printf("Number of successful tests: %d\n", successful_tests);
    for (int i = 0; i < 10; i++) {
        char arr_char[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
        char key_char = 'a' + i;
        successful_tests += run_test(arr_char, 10, sizeof(char), &key_char, &arr_char[i], compareChar);
    }
    printf("Number of successful tests: %d\n", successful_tests);
    for (int i = 0; i < 10; i++) {
        test_t arr[] = {{1, "Julian"},{2, "John"},{3, "Doe"},{4, "Jane"},{5, "Doe"},{6, "John"},{7, "Julian"},{8, "Jane"},{9, "Doe"},{10, "John"}};
        int key_struct = i + 1;
        successful_tests += run_test(arr, 10, sizeof(test_t), &key_struct, &arr[i].test_key, compareStruct);
        }
    printf("Number of successful tests: %d\n", successful_tests);

    return 0;
}