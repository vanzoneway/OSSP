#include "recursive_reading.h"


int main(int argc, char *argv[])
{
    int option;
    DynamicArray array_of_strings;
    init_dynamic_array(&array_of_strings, 10);
    FLAG_OPTIONS base_flags= {0, 0, 0, 0};

    char* directory = "./";

    while((option = getopt(argc, argv, "ldfs")) != -1)
    {
        switch (option)
        {
            case 'l':
                printf("Option -l is set\n");
                base_flags.SOFT_LINKS_FLAG = 1;
                break;

            case 'd':
                printf("Option -d is set\n");
                base_flags.DIRECTORIES_FLAG = 1;
                break;


            case 'f':
                base_flags.FILES_FLAG = 1;
                printf("Option -f is set\n");
                break;

            case 's':
                base_flags.SORT_FLAG = 1;
                printf("Option -s is set\n");
                break;

            default:
                break;

        }
    }
    if(optind < argc)
        directory = argv[optind];
    printf("Directory: %s\n", directory);


    recursive_parse(directory, &base_flags, &array_of_strings);
    if(base_flags.SORT_FLAG)
        qsort(array_of_strings.data, array_of_strings.size, sizeof(char*), compare_strings);

    print_dynamic_array(&array_of_strings);


    return 0;
}