#include "debug.h"

// #include "leakchk.h"

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