#include <cstdio>
#include <cstring>
#include <cerrno>

#include <cpinti/filesystem.h>
#include <cpinti/debug.h>

#include "core.h"

extern "C" void cpc_CCP_Exec_Commande(const char *COMMANDE, int ID);

namespace cpinti::filesystem
{

    bool file_copy(const char *source, const char *destination, int priorite, const char *var_progression, const char *var_octets, const char *var_octetsparsec)
    {
        // Cette methode permet de copier un fichier source a une destination

        // Priorite = 0	: Copie par defaut a priorite automatisee (selon la charge du CPU)
        // 			= 1	: Copie normale (50%)
        // 			= 2	: Copie prioritaire, priorise plus le thread actuel (70%)
        //			= 3 : Copie a section critique (Bloque tous les autre threads et priorise a 100%)

        // Lire tout le contenu d'un fichier uniquement
        // Retourne :
        //	Si ok = Le contenu texte du fichier
        //	Sinon = 0

        // Definit les attributs temporaires
        bool RETOUR = false;
        int CompteurDoevents = 0;

        // Les descripteurs de fichier
        FILE *Instance_Fichier_SOURCE;
        FILE *Instance_Fichier_DESTINATION;

        if (priorite == 0)
        {
            priorite = 1;
        }

        // Ouvrir le fichier SOURCE
        Instance_Fichier_SOURCE = fopen(source, "r");

        // Ouvrir le fichier DESTINATION
        Instance_Fichier_DESTINATION = fopen(destination, "w");

        // Si c'est ok pour la source
        if (Instance_Fichier_SOURCE != nullptr)
        {
            // Si c'est ok pour la destination
            if (Instance_Fichier_DESTINATION != nullptr)
            {

                uinteger TailleFichier = cpinti::filesystem::file_get_size(source);
                uinteger Position = 0;
                uinteger NombreOctets = 0;
                uinteger NombreOctetsParSec = 0;
                uinteger TempsPasse = 0;

                double valeur = 0;
                double vitesse = 0;

                clock_t TempsDebut;
                clock_t TempsFin;

                char data = '\0';

                char *_Commande_CpcdosCP = (char *)malloc(sizeof(char) * 128);

                ENTRER_SectionCritique();

                // Boucler jusqu'a la fin du fichier
                while (Position <= TailleFichier)
                {

                    // Cette partie va permettre d'alleger le CPU
                    if (CompteurDoevents >= 8192)
                    {
                        CompteurDoevents = 0;

                        /** PROGRESSION EN POURCENTAGE **/
                        if ((var_progression != nullptr) && (strlen(var_progression) > 1))
                        {

                            valeur = ((double)NombreOctets / (double)TailleFichier) * 100;
                            sprintf(_Commande_CpcdosCP, "FIX/ %s = /F:CPC.INT(%f)", var_progression, valeur);
                            cpc_CCP_Exec_Commande(_Commande_CpcdosCP, 5);
                        }

                        /** NOMBRE D'OCTETS COPIES **/
                        if ((var_octets != nullptr) && (strlen(var_octets) > 1))
                        {

                            valeur = (double)NombreOctets;
                            sprintf(_Commande_CpcdosCP, "FIX/ %s = /F:CPC.INT(%f)", var_octets, valeur);
                            cpc_CCP_Exec_Commande(_Commande_CpcdosCP, 5);
                        }

                        if (vitesse > 1)
                        {
                            /** NOMBRE D'OCTETS PAR SECONDES **/
                            if ((var_octetsparsec != nullptr) && (strlen(var_octetsparsec) > 1))
                            {

                                sprintf(_Commande_CpcdosCP, "FIX/ %s = /F:CPC.INT(%f)", var_octetsparsec, vitesse);
                                cpc_CCP_Exec_Commande(_Commande_CpcdosCP, 5);
                                vitesse = 0;
                            }
                        }
                    }
                    else
                        CompteurDoevents++;

                    // Recuperer le caractere SOURCE
                    data = fgetc(Instance_Fichier_SOURCE);

                    // Ecrire le caractere DESTINATION
                    fputc(data, Instance_Fichier_DESTINATION);

                    // Avancer d'une position
                    Position++;

                    NombreOctetsParSec++;
                    NombreOctets++;

                    if ((var_octetsparsec != nullptr) && (strlen(var_octetsparsec) > 1))
                    {
                        TempsFin = clock();

                        TempsPasse = (uinteger)((TempsFin - TempsDebut) / CLOCKS_PER_SEC);

                        if (TempsPasse > 1)
                        {
                            TempsDebut = clock();

                            /** NOMBRE D'OCTETS PAR SECONDES **/

                            vitesse = (double)NombreOctetsParSec;
                            NombreOctetsParSec = 0;
                        }
                    }
                }

                SORTIR_SectionCritique();

                _Commande_CpcdosCP[0] = '\0';

                // OK
                RETOUR = true;
            }
            else
            {
                // PROBLEME
                RETOUR = false;
            }
        }
        else
        {
            // Fichier non disponible
            RETOUR = false;
        }

        // Fermer les instances
        if (Instance_Fichier_DESTINATION != nullptr)
            fclose(Instance_Fichier_DESTINATION);
        if (Instance_Fichier_SOURCE != nullptr)
            fclose(Instance_Fichier_SOURCE);

        if (RETOUR == false)
        {
            cpinti::debug::error("[ERROR] Unable to copy file '%s' to '%s'. Raison: %s", source, destination, strerror(errno));
        }
        return RETOUR;
    }
} // namespace cpinti::filesystem
