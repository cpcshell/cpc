#ifndef INCLUDE_IO
#define INCLUDE_IO

namespace cpinti
{
    namespace gestionnaire_fichier
    {

        // EXTERNE
        int cpinti_decompress_file(const char *Source, const char *Destination);
        int cpinti_compress_file(const char *Source, const char *Destination);
        bool cpinti_Fichier_Existe(const char *Source);
        uinteger cpinti_Taille_Fichier(const char *Source);
        bool cpinti_Lire_Fichier_complet(const char *Source, const char *Mode, char *_DONNEES, uinteger TailleFichier);
        bool cpinti_Ecrire_Fichier_complet(const char *Source, const char *_DONNEES, int Mode);
        bool cpinti_Supprimer_Fichier(const char *Source, bool Securise, int NombrePasses);
        bool cpinti_Copier_Fichier(const char *Source, const char *Destination, integer Priorite, const char *VAR_Progression, const char *VAR_Octets, const char *VAR_OctetsParSec);

        // INTERNE
        int Fichier_decompress(const char *Source);
        int inf(FILE *source, FILE *dest);

        bool Fichier_Existe(const char *Source);
        bool Repertoire_Existe(const char *Source);
        uinteger Taille_Fichier(const char *Source);
        bool Lire_Fichier_complet(const char *Source, const char *MODE, char *_DONNEES, uinteger Taille_Fichier);
        bool Ecrire_fichier(const char *Source, const char *Donnees, int FLAG);
        bool Supprimer_Ficher(const char *Source, bool securise, int NombrePasses);
        bool Copier_Fichier(const char *Source, const char *Destination, int Priorite, const char *VAR_Progression, const char *VAR_Octets, const char *VAR_OctetsParSec);
    } // namespace gestionnaire_fichier
} // namespace cpinti

#endif /* INCLUDE_IO */
