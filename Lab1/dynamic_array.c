#include "dynamic_array.h"




void init_dynamic_array(struct DynamicArray* array, size_t initial_capacity) {
    array->data = (char**)malloc(initial_capacity * sizeof(char*));
    array->size = 0;
    array->capacity = initial_capacity;
}

void destroy_dynamic_array(struct DynamicArray* array) {
    for (size_t i = 0; i < array->size; ++i) {
        free(array->data[i]);
    }
    free(array->data);
}

void resize_dynamic_array(struct DynamicArray* array, size_t new_capacity) {
    array->data = (char**)realloc(array->data, new_capacity * sizeof(char*));
    array->capacity = new_capacity;
}

void append_dynamic_array(struct DynamicArray* array, const char* value) {
    if (array->size == array->capacity) {
        size_t new_capacity = array->capacity * 2;
        resize_dynamic_array(array, new_capacity);
    }

    size_t length = strlen(value);
    char* new_string = (char*)malloc((length + 1) * sizeof(char));
    strcpy(new_string, value);
    array->data[array->size] = new_string;
    array->size++;
}

const char* get_dynamic_array_element(const struct DynamicArray* array, size_t index) {
    if (index < array->size) {
        return array->data[index];
    } else {
        printf("Error: Index out of range\n");
        exit(1);
    }
}

void print_dynamic_array(const struct DynamicArray* array) {
    for (size_t i = 0; i < array->size; ++i) {
        printf("%s %lu\n", array->data[i], strlen(get_dynamic_array_element(array, i)));
    }
}

int compare_strings(const void* a, const void* b) {
    const char* str1 = *(const char**)a;
    const char* str2 = *(const char**)b;
    return strcmp(str1, str2);
}