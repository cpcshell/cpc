#include <cstdio>

#include <cpinti/filesystem.h>
#include <cpinti/debug.h>

namespace cpinti::filesystem
{
    bool file_read_all(const char *path, const char *mode, char *output, uinteger output_size)
    {
        debug::trace("Opening file '%s'", path);

        FILE *f = fopen(path, mode);

        if (!f)
        {
            debug::error("Failled to open '%s'", path);
            return false;
        }

        size_t offset = 0;

        while (offset < output_size)
        {
            offset += fread(&output[offset], 1, output_size - offset, f);
        }

        debug::trace("Read success for '%s'", path);

        return true;
    }
} // namespace cpinti::filesystem
