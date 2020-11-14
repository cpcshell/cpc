#include <cpinti/filesystem.h>
#include <cpinti/debug.h>
#include "io.h"

namespace cpinti::filesystem
{
    uinteger file_get_size(const char *path)
    {
        FILE *fp = fopen(path, "r");
        
        if (fp == nullptr)
        {
            cpinti::debug::warn("Can't open %s", path);
            return 0;
        }
        
        fseek(fp, 0L, SEEK_END);
        uinteger size = (uinteger)ftell(fp);
        fclose(fp);

        return size;
    }
} // namespace cpinti::filesystem
