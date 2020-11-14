#ifndef CPINTI_FILESYSTEM_H
#define CPINTI_FILESYSTEM_H

#include <cpinti/types.h>

namespace cpinti::filesystem
{
    bool file_exist(const char *path);
    bool folder_exist(const char *path);
    uinteger file_get_size(const char *path);
    bool file_read_all(const char *path, const char *mode, char *output, uinteger output_size);
    bool file_copy(const char *source, const char *destination, int priorite, const char *var_progression, const char *var_octets, const char *var_octetsparsec);
}

#endif // CPINTI_FILESYSTEM_H
