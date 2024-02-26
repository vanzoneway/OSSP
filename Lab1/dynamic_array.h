//
// Created by ivan on 17.2.24.
//

#ifndef LAB1_DYNAMIC_ARRAY_H
#define LAB1_DYNAMIC_ARRAY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct DynamicArray {
    char** data;
    size_t size;
    size_t capacity;
}DynamicArray;

void init_dynamic_array(struct DynamicArray* array, size_t initial_capacity);
void destroy_dynamic_array(struct DynamicArray* array);
void resize_dynamic_array(struct DynamicArray* array, size_t new_capacity);
void append_dynamic_array(struct DynamicArray* array, const char* value);
const char* get_dynamic_array_element(const struct DynamicArray* array, size_t index);
void print_dynamic_array(const struct DynamicArray* array);
int compare_strings(const void* a, const void* b);


#endif //LAB1_DYNAMIC_ARRAY_H

