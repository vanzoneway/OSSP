//
// Created by ivan on 17.2.24.
//

#ifndef LAB1_RECURSIVE_READING_H
#define LAB1_RECURSIVE_READING_H
#include <dirent.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "dynamic_array.h"


typedef struct flag_options
{
    int SOFT_LINKS_FLAG;
    int DIRECTORIES_FLAG;
    int FILES_FLAG;
    int SORT_FLAG;

}FLAG_OPTIONS;

void recursive_parse(const char* path, FLAG_OPTIONS* base_flags, DynamicArray* array_of_strings);
#endif //LAB1_RECURSIVE_READING_H
