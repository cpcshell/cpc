#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "core.h"
#include "cpinti/debug.h"
#include "func_cpi.h" // doevent
#include "io.h"

namespace cpinti::gestionnaire_fichier
{
    bool Lire_Fichier_complet(const char *path, const char *mode, char *output, uinteger output_size)
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
} // namespace cpinti::gestionnaire_fichier
