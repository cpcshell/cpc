/*	
	======================================
	==     CPinti ---> Execution wrapp� ==
	======================================
	
	Developpe entierement par Sebastien FAVIER

	Description
		Module d'execution des fonctions de CPinti Core avec
		 support RAII pour une allocation/desallocation propre
		 horodatee et logee depuis le debogeur
	
	Creation 
		19/10/2016


	Mise a jour
		16/10/2018
		
	16-10-2018	: Adaption 2.1 beta 1.1
	07-12-2017	: AMELIORATION du code en suivant une procedure sticte de GCC
	06-10-2017	: Ajout de Copier_Fichier.
	23-08-2017	: Ajout de cpinti_get_nom_thread() & cpinti_get_nom_processus() pour recuperer les noms des threads&Processus
	08-05-2017	: Ajout des arguments d'entre pour les threads crees
	13-02-2017	: Finitions du serveur & client
	02/01/2017 	: Ajout Entrer/Fin Section Critique
	16/01/2017	: Ajout CPinti checker + ajustements
	
*/
#include <memory>

#include "cpinti.h" // inclut taches.h
#include "debug.h"
#include "func_cpi.h"

// #include "leakchk.h"

extern "C" int cpc_clean();

namespace Wrapper_Cpcdos
{

    int cpinti_WRAPPER(integer FunctionID, double _CLE_, const char *ARG_1, integer ARG_2, void *ARG_3, void *ARG_4, void *ARG_5)
    {
        // Cette fonction va permettre d'executer une FunctionID compile dans CONTRIB
        // 	FunctionID		: Numero de la fonction
        // 	PID				: Numero de processus parent

        // Retourne -1 : Erreur memoire
        // 			 0 : Fichier non disponible
        //			 1 : Fichier present
        (void)_CLE_;
        (void)ARG_1;
        (void)ARG_2;
        (void)ARG_3;
        (void)ARG_4;
        (void)ARG_5;

        if (FunctionID == 99)
        {
            cpc_clean();
            return 0;
        }

        std::string FunctionID_STR = std::to_string(FunctionID);

        cpinti_dbg::CPINTI_DEBUG("Creation d'une instance du wrapper CPinti Core... ",
                                 "Creating CPinti Core wrapper... ",
                                 "__CpintiCore_CpcdosOSx__", "cpinti_WRAPPER()",
                                 Ligne_reste, Alerte_validation, Date_avec, Ligne_r_normal);

        cpinti_dbg::CPINTI_DEBUG("Lancement de l'instance...", "Instance execution...",
                                 "", "", Ligne_saute, Alerte_action, Date_sans, Ligne_r_normal);

        return (0);

    } /* WRAPPER_CPCDOS */

} // namespace Wrapper_Cpcdos