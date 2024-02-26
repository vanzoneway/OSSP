
#include "recursive_reading.h"



void recursive_parse(const char* path, FLAG_OPTIONS* base_flags, DynamicArray* array_of_strings)
{
    struct dirent *dir_entry;
    struct stat file_info;
    DIR *dir;

    dir = opendir(path);
    if (!dir) {
        perror("opendir(""/"")");
        exit(errno);
    }

    while((dir_entry = readdir(dir)) != NULL)
    {
        char full_path[PATH_MAX];
        if(strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, dir_entry->d_name);

        if (lstat(full_path, &file_info) == -1) {
            perror("lstat");
            continue;
        }

        if((S_IFDIR & file_info.st_mode) && base_flags->DIRECTORIES_FLAG)
        {
            append_dynamic_array(array_of_strings, full_path);
        }else if(S_ISLNK(file_info.st_mode) && base_flags->SOFT_LINKS_FLAG)
        {
            append_dynamic_array(array_of_strings, full_path);
        }else if((S_IFREG & file_info.st_mode) && base_flags->FILES_FLAG)
        {
            append_dynamic_array(array_of_strings, full_path);
        }else if (!base_flags->DIRECTORIES_FLAG && !base_flags->SOFT_LINKS_FLAG
                  && !base_flags->FILES_FLAG && ((S_IFREG & file_info.st_mode) || S_ISLNK(file_info.st_mode) || (S_IFDIR & file_info.st_mode)))
        {
            append_dynamic_array(array_of_strings, full_path);
        }else
            continue;

        if(dir_entry->d_type == DT_DIR)
        {
            recursive_parse(full_path, base_flags, array_of_strings);
        }
    }
    closedir(dir);

}
