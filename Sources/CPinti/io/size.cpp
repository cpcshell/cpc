#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "core.h"
#include "debug.h"
#include "io.h"

namespace cpinti
{
    namespace gestionnaire_fichier
    {

        uinteger Taille_Fichier(const char *Chemin)
        {
            // Cette fonction retourne la taille en octets
            // -1 	: Probleme
            // => 0 : OK
            // Ouvrir fichier
            FILE *Instance_Fichier = fopen(Chemin, "r");

            // Tester si le fichier s'est ouvert sans problemes
            if (Instance_Fichier != NULL)
            {
                // aller a la fin du fichier
                fseek(Instance_Fichier, 0L, SEEK_END);

                // Recuperer la position et donc la taille
                uinteger Taille = (uinteger)ftell(Instance_Fichier);
                // Fermer le fichier
                fclose(Instance_Fichier);

                // Retourner la taille
                return (uinteger)Taille;
            }
            else
            {
                // asm("sti");
                // Erreur
                return 0;
            }
        }
    } // namespace gestionnaire_fichier
} // namespace cpinti
