#include <malloc.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include "cpinti.h"
#include "cpinti/buffer.h"
#include "stack.h"

#include "debug.h"
#include "func_cpi.h"

namespace cpinti
{
    std::string cpinti_GEST_BUFF(uinteger _ID, int _CHEMIN, std::string _DONNEES)
    {
        if (_CHEMIN < 6)
        {
            for (uinteger index_tab = 0; index_tab < cpinti::stack_kernel.size(); index_tab++)
            {
                if (cpinti::stack_kernel.at(index_tab)->tag_1 == _ID)
                {
                    // Si non indexe, donc non instancie, on evite les crashs Ahaha!! ;-)
                    if (((_CHEMIN < 9) && (index_tab >= 512)) && (_ID == 0))
                    {
                        return "";
                    }

                    /********** STOCKAGE DANS LA STACK **********/
                    if (_CHEMIN == _STACK_STOCKER_POUR_CPCDOS)
                    {
                        /** SERVEUR ----> STOCKAGE CPCDOS **/
                        cpinti::stack_kernel.at(index_tab)->add_Stack(_DONNEES); // Stocker
                        return "";
                    }

                    else if (_CHEMIN == _STACK_STOCKER_POUR_SERVEUR)
                    {
                        /** CPCDOS ----> STOCKAGE SERVEUR **/
                        cpinti::stack_server.at(index_tab)->add_Stack(_DONNEES); // Stocker
                        return "";
                    }
                    else if (_CHEMIN == _STACK_EXTRACT_POUR_CPCDOS)
                    {
                        return cpinti::stack_kernel.at(index_tab)->get_Stack(0); // Recuperer
                    }
                    else if (_CHEMIN == _STACK_EXTRACT_POUR_SERVEUR)
                    {
                        return cpinti::stack_server.at(index_tab)->get_Stack(0); // Recuperer
                    }
                }
            }

            return (std::string) "#DECO";
        }
        else
        {
            if (_CHEMIN == _STACK_INITIALISER)
            {
                // Creer une nouvelle instance d'un stack

                // Pour verifier si tout est ok
                bool Stack_init_ok = false;

                // Debug
                std::string _ID_STR = std::to_string(_ID);

                cpinti_dbg::CPINTI_DEBUG("(ID:" + _ID_STR + ") Recherche d'un emplacement libre... ",
                                         "(ID:" + _ID_STR + ") Research free block... ",
                                         "STACK", "cpinti_GEST_BUFF", Ligne_saute, Alerte_action, Date_avec, Ligne_r_normal);

                // Debug
                std::string _MAX_Stack_block_STR = std::to_string(_MAX_Stack_block);
                std::string index_STR = std::to_string(0);

                cpinti_dbg::CPINTI_DEBUG("[OK]", "[OK]", "", "",
                                         Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

                cpinti_dbg::CPINTI_DEBUG("[vector<unique_ptr<cpinti::cpinti_stack_inv>>] Creation d'un stack 'KERNEL' de " + _MAX_Stack_block_STR + " block(s) de memoire a l'index " + index_STR + " ... ",
                                         "[vector<unique_ptr<cpinti::cpinti_stack_inv>>] Creating 'KERNEL' stack with " + _MAX_Stack_block_STR + " memory block(s) at " + index_STR + " ... ",
                                         "STACK", "cpinti_GEST_BUFF", Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

                /************** VERIFIER QUE CA PLANTE PAS ICI **************/

                // Instancier une nouvelle instance de unique_ptr
                cpinti::stack_kernel.emplace_back(std::make_shared<cpinti::cpinti_stack_inv>());

                // Initialiser le gestionnaire de stack de CPinti Core
                cpinti::stack_kernel.back()->stack__init(_MAX_Stack_block);

                // Ajouter l'id
                cpinti::stack_kernel.back()->tag_1 = _ID;

                cpinti_dbg::CPINTI_DEBUG("[OK]", "[OK]", "", "",
                                         Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

                cpinti_dbg::CPINTI_DEBUG("[vector<unique_ptr<cpinti::cpinti_stack_inv>>] Creation d'un stack 'SERVEUR' de " + _MAX_Stack_block_STR + " block(s) de memoire a l'index " + index_STR + " ... ",
                                         "[vector<unique_ptr<cpinti::cpinti_stack_inv>>] Creating 'SERVER' stack with " + _MAX_Stack_block_STR + " memory block(s) at " + index_STR + " ... ",
                                         "STACK", "cpinti_GEST_BUFF", Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

                // Instancier une nouvelle instance de unique_ptr
                cpinti::stack_server.emplace_back(std::make_shared<cpinti::cpinti_stack_inv>());

                // Initialiser le gestionnaire de stack de CPinti Core
                cpinti::stack_server.back()->stack__init(_MAX_Stack_block);

                // Ajouter l'id
                cpinti::stack_kernel.back()->tag_1 = _ID;

                /************** VERIFIER QUE CA PLANTE PAS ICI **************/

                cpinti_dbg::CPINTI_DEBUG("[OK]", "[OK]", "", "",
                                         Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

                Stack_init_ok = true;

                if (Stack_init_ok == true)
                    cpinti_dbg::CPINTI_DEBUG("Allocation memoire dynamique d'un stack communiquant 'KERNEL' <--> 'SERVEUR' termine!",
                                             "Dynamic communicating memory allocation 'KERNEL' <--> 'SERVER' terminated!",
                                             "STACK", "cpinti_GEST_BUFF", Ligne_saute, Alerte_validation, Date_avec, Ligne_r_normal);
                else
                    cpinti_dbg::CPINTI_DEBUG("Impossible d'allouer de la memoire dynamique d'un stack communiquant 'KERNEL' <--> 'SERVEUR'!",
                                             "Unable to allocate Dynamic communicating memory 'KERNEL' <--> 'SERVER'",
                                             "STACK", "cpinti_GEST_BUFF", Ligne_saute, Alerte_erreur, Date_avec, Ligne_r_normal);
            }
            else if (_CHEMIN == _STACK_SUPPRIMER)
            {
                // Creer une nouvelle instance d'un stack

                // Debug
                std::string _ID_STR = std::to_string(_ID);

                cpinti_dbg::CPINTI_DEBUG("(ID:" + _ID_STR + ") Recherche de l'instance a desallouer... ",
                                         "(ID:" + _ID_STR + ") Research instance to dealloc... ",
                                         "STACK", "cpinti_GEST_BUFF", Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

                cpinti_dbg::CPINTI_DEBUG("[OK]", "[OK]", "", "",
                                         Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

                bool Suppression_ok = false;
                // Recuperer l'ID DEPUIS le vector et non depuis Stack__PORT_ATTRIB
                for (uinteger index_tab = 0; index_tab < cpinti::stack_kernel.size(); index_tab++)
                    if (cpinti::stack_kernel.at(index_tab)->tag_1 == _ID)
                    {
                        // Debug
                        std::string _MAX_Stack_block_STR = std::to_string(_MAX_Stack_block);
                        std::string index_STR = std::to_string(index_tab);

                        cpinti_dbg::CPINTI_DEBUG("[vector<shared_ptr<cpinti::cpinti_stack_inv>>] Suppression d'un stack 'KERNEL' de " + _MAX_Stack_block_STR + " block(s) de memoire a l'index " + index_STR + " ... ",
                                                 "[vector<shared_ptr<cpinti::cpinti_stack_inv>>] Deleting 'KERNEL' stack with " + _MAX_Stack_block_STR + " memory block(s) at " + index_STR + " ... ",
                                                 "STACK", "cpinti_GEST_BUFF", Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

                        /************** VERIFIER QUE CA PLANTE PAS ICI **************/

                        // Verifier son existence
                        if (cpinti::stack_kernel.empty() == false)
                        {
                            // Si il reste 1 element alors on clean TOUT
                            if (cpinti::stack_kernel.size() < (uinteger)2)
                                cpinti::stack_kernel.clear();

                            else
                            {
                                // Supprimer le gestionnaire de stack de CPinti Core
                                cpinti::stack_kernel.at(index_tab).reset();

                                cpinti::stack_kernel.erase(cpinti::stack_kernel.begin() + index_tab);
                            }

                            // Reajuster le vecteur
                            cpinti::stack_kernel.shrink_to_fit();
                        }

                        cpinti_dbg::CPINTI_DEBUG("[OK]", "[OK]", "", "",
                                                 Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

                        cpinti_dbg::CPINTI_DEBUG("[vector<shared_ptr<cpinti::cpinti_stack_inv>>] Suppression d'un stack 'SERVEUR' de " + _MAX_Stack_block_STR + " block(s) de memoire a l'index " + index_STR + " ... ",
                                                 "[vector<shared_ptr<cpinti::cpinti_stack_inv>>] Deleting 'SERVER' stack with " + _MAX_Stack_block_STR + " memory block(s) at " + index_STR + " ... ",
                                                 "STACK", "cpinti_GEST_BUFF", Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

                        // Verifier son existence
                        if (cpinti::stack_server.empty() == false)
                        {
                            // Si il reste 1 element alors on clean TOUT
                            if (cpinti::stack_server.size() < (uinteger)2)
                                cpinti::stack_server.clear();

                            else
                            {
                                // Supprimer le gestionnaire de stack de CPinti Core
                                cpinti::stack_server.at(index_tab).reset();

                                cpinti::stack_server.erase(cpinti::stack_server.begin() + index_tab);
                            }

                            // Reajuster le vecteur
                            cpinti::stack_server.shrink_to_fit();
                        }

                        /************** VERIFIER QUE CA PLANTE PAS ICI **************/

                        cpinti_dbg::CPINTI_DEBUG("[OK]", "[OK]", "", "",
                                                 Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

                        Suppression_ok = true;
                        break;
                    }

                if (Suppression_ok == false)
                    cpinti_dbg::CPINTI_DEBUG("Impossible de trouver l'instance",
                                             "Unable to find instance",
                                             "STACK", "cpinti_GEST_BUFF", Ligne_reste, Alerte_erreur, Date_avec, Ligne_r_normal);
            }
        }
        return _DONNEES;
    }

    int cpinti_GEST_BUFF_c(uinteger _ID, integer _CHEMIN, const char *_DONNEES)
    {
        // Cette methode permet d'utiliser la fonction CPINTI__GEST_BUFF() depuis le freebasic
        // Renvoie 1 si tout est ok et 0 si le serveur est pas dispo

        // Receptionner les donnees
        if (cpinti_GEST_BUFF(_ID, _CHEMIN, std::string(_DONNEES)) == "#DECO")
            return 0;
        else
            return 1;
    }

    int cpinti_GEST_BUFF_c(uinteger _ID, integer _CHEMIN, char *_DONNEES)
    {
        // Cette methode permet d'utiliser la fonction CPINTI__GEST_BUFF() depuis le freebasic
        // Renvoie 1 si tout est ok et 0 si le serveur est pas dispo

        std::string Retour;

        // Receptionner les donnees
        Retour = cpinti_GEST_BUFF(_ID, _CHEMIN, std::string(_DONNEES));

        // Les remplir dans le pointeur du noyau cpcdos
        for (uinteger boucle = 0; boucle < (uinteger)Retour.length(); boucle++)
            _DONNEES[boucle] = Retour.at(boucle);

        // Si le serveur s'est deconnecte ou n'est pas connecte
        if (Retour == "#DECO")
            return 0;
        else
            return 1;
    }
} // namespace cpinti