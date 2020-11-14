#ifndef INCLUDE_IO
#define INCLUDE_IO

#include <stdio.h>

#include "cpinti/types.h"

namespace cpinti::gestionnaire_fichier
{
    bool cpinti_Fichier_Existe(const char *Source);
    uinteger cpinti_Taille_Fichier(const char *Source);

    bool cpinti_Lire_Fichier_complet(const char *Source, const char *Mode, char *_DONNEES, uinteger TailleFichier);
    bool cpinti_Ecrire_Fichier_complet(const char *Source, const char *_DONNEES, int Mode);

    bool cpinti_Supprimer_Fichier(const char *Source, bool Securise, int NombrePasses);
    bool cpinti_Copier_Fichier(const char *Source, const char *Destination, integer Priorite, const char *VAR_Progression, const char *VAR_Octets, const char *VAR_OctetsParSec);

    int inf(FILE *source, FILE *dest);

    bool Ecrire_fichier(const char *Source, const char *Donnees, int FLAG);

    bool Supprimer_Ficher(const char *Source, bool securise, int NombrePasses);
} // namespace cpinti::gestionnaire_fichier

#endif /* INCLUDE_IO */
