#include <sys/stat.h>

#include <iostream>

#include <cpinti/filesystem.h>

namespace cpinti::filesystem
{

    bool file_exist(const char *path)
    {
        struct stat st;

        stat(path, &st);

        return S_ISREG(st.st_mode);
    }

    bool folder_exist(const char *path)
    {
        struct stat st;

        if (stat(path, &st) != 0)
            return false;

        if (S_ISDIR(st.st_mode) > 0)
            return true;

        return false;
    }
} // namespace cpinti::filesystem