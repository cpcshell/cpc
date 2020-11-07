#include "cpinti.h" // Doevents
#include "debug.h"
#include "io.h"
#include <iostream>
#include <memory>
#include <sys/stat.h>

namespace cpinti
{
    namespace gestionnaire_fichier
    {
        // ===========================================================================
        // ============================ FICHIER EXISTE ===============================
        // ===========================================================================

        bool Fichier_Existe(const char *Chemin)
        {
            // Cette fonction permet de tester si un fichier existe
            // Retourne
            //	0 : Non disponible
            //	1 : OK

            struct stat stat_instance;

            stat(Chemin, &stat_instance);

            return S_ISREG(stat_instance.st_mode);
        }

        // ===========================================================================
        // =========================== REPERTOIRE EXISTE =============================
        // ===========================================================================

        bool Repertoire_Existe(const char *Chemin)
        {
            struct stat stat_instance;

            if (stat(Chemin, &stat_instance) != 0)
                return false;

            if (S_ISDIR(stat_instance.st_mode) > 0)
                return true;

            return false;
        }
    } // namespace gestionnaire_fichier
} // namespace cpinti