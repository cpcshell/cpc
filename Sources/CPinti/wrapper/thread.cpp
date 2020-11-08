#include <stdio.h>
#include <string.h>

#include "core.h"
#include "debug.h"

#include "cpinti/threads.h"

// #include "leakchk.h"

namespace cpinti::gestionnaire_tache
{
    uinteger cpinti_creer_thread(uinteger ID_KERNEL, uinteger ID_OS, uinteger ID_USER, uinteger PID, const char *NomThread,
                                 integer Priorite, void *(*Fonction)(void *arg), void *ARG_CP, uinteger ARG_TH)
    {
        (void)ARG_CP;
        // Cette fonction va permettre de creer un nouveau processus vierge (Qui hebergera vos threads)
        // 	ID_KERNEL		: Identificateur unique de l'instance du noyau
        //	NomProcessus	: Nom de processus

        // Retourne	<=0 : Erreur
        //			> 0 : Numero de PID

        ENTRER_SectionCritique();

        // On ajoute un thread depuis le CORE
        uinteger Resultat = gestionnaire_tache::ajouter_Thread(Fonction, NomThread, PID, Priorite, ARG_TH);

        // Si la creation est un succes, on remplit les informations du thread
        if (Resultat > 0)
        {
            /** Identifiant NOYAU **/
            gestionnaire_tache::Liste_Threads[Resultat].KID = ID_KERNEL;

            /** Identifiant OS **/
            gestionnaire_tache::Liste_Threads[Resultat].OID = ID_OS;

            /** Identifiant Utilisateur **/
            gestionnaire_tache::Liste_Threads[Resultat].UID = ID_USER;

            /** Identifiant Processus **/
            gestionnaire_tache::Liste_Threads[Resultat].PID = PID;

            /** Nom du thread **/
            // strncpy((char*) gestionnaire_tache::Liste_Threads[Resultat].Nom_Thread, NomThread, 30);

            SORTIR_SectionCritique();
        }
        else
        {
            std::string Resultat_STR = std::to_string((uinteger)Resultat);
            cpinti_dbg::CPINTI_DEBUG("[ERREUR] Impossible de creer un thread. Retour:" + Resultat_STR,
                                     "[ERROR] Unable to create thread. Return:" + Resultat_STR,
                                     "core::gestionnaire_tache", "free_Thread_zombie()",
                                     Ligne_saute, Alerte_ok, Date_avec, Ligne_r_normal);

            SORTIR_SectionCritique();
        }

        // Retourner son PID
        return Resultat;
    }

    int cpinti_fin_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID)
    {
        (void)ID_KERNEL;
        (void)PID;
        // Cette fonction permet de finaliser l'execution d'un thread, fermeture forcee
        return (int)gestionnaire_tache::supprimer_Thread(TID, true);
    }

    int cpinti_arreter_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID)
    {
        // Fermeture non forcee (signal)
        return cpinti_arreter_thread(ID_KERNEL, PID, TID, false);
    }

    int cpinti_arreter_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID, bool force)
    {
        (void)ID_KERNEL;
        (void)PID;
        // Cette fonction permet d'arreter un thread correctement.
        //  A utiliser sur un autre thread
        // 	ID_KERNEL		: Identificateur unique de l'instance du noyau
        //  PID				: Numero de processus
        //	TID				: Numero du thread
        //	force			: Force la fermeture du thread zombie

        bool Resultat = false;

        Resultat = gestionnaire_tache::supprimer_Thread(TID, force);

        if (Resultat == true)
            return 1;
        else
            return 0;
    }

    bool _exit()
    {
        // return gestionnaire_tache::fermer_core();
        return true;
    }

    uinteger cpinti_joindre_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID, int CYCLES)
    {
        (void)ID_KERNEL;
        (void)PID;
        (void)TID;
        (void)CYCLES;
        // Cette fonction permet d'executer directement un thread sans passer par l'ordonanceur
        // 	ID_KERNEL		: Identificateur unique de l'instance du noyau
        //  PID				: Numero de processus
        //	TID				: Numero du thread
        //	CYCLES			: Nombre de cycles a executer (1 par defaut)

        uinteger Resultat = 0;

        // Retourner le resultat
        return Resultat;
    }

    int cpinti_sortir_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID)
    {
        (void)ID_KERNEL;
        (void)PID;
        (void)TID;
        // Cette fonction permet de sortir du thread
        // 	ID_KERNEL		: Identificateur unique de l'instance du noyau
        //  PID				: Numero de processus
        //	TID				: Numero du thread

        int Resultat = 0;

        return Resultat;
    }

    void *cpinti_thread_args(int NoARG)
    {
        (void)NoARG;
        // Cette fonction permet de retourner un argument pour les threads venant d'etre crees
        //	NoARG	: Numero d'argument

        // Retourne	une valeur ANY PTR

        // if(NoARG == 0)
        // return (void*) NP_cpinti_gestionnaire_tache::thread_args::ARG1;
        // else if(NoARG == 1)
        // return (void*) NP_cpinti_gestionnaire_tache::thread_args::ARG2;
        // else if(NoARG == 2)
        // return (void*) (long) NP_cpinti_gestionnaire_tache::thread_args::ARG3;
        // else if(NoARG == 3)
        // return (void*) (long) NP_cpinti_gestionnaire_tache::thread_args::ARG4;
        // else if(NoARG == 4)
        // return (void*) (long) NP_cpinti_gestionnaire_tache::thread_args::ARG5;
        // else if(NoARG == 1995)
        // return (void*) NP_cpinti_gestionnaire_tache::thread_args::TID; 	/* ID du thread */
        // else if(NoARG == 2803)
        // return (void*) NP_cpinti_gestionnaire_tache::thread_args::PID; 	/* ID du processus */
        // else if(NoARG == 2912)
        // return (void*) NP_cpinti_gestionnaire_tache::thread_args::USER_ID; /* ID du USER */
        // else if(NoARG == 1909)
        // return (void*) NP_cpinti_gestionnaire_tache::thread_args::OS_ID; 	/* ID de l'OS */
        // else if(NoARG == 1507)
        // return (void*) NP_cpinti_gestionnaire_tache::thread_args::KERNEL_ID; /* ID du kernel */
        // else if(NoARG == 2011)
        // return (void*) (long) NP_cpinti_gestionnaire_tache::thread_args::NomThread; /* Nom du thread */

        // Aucun argument correspond
        return nullptr;
    }

    int cpinti_gerer_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID, uinteger ACTION)
    {
        (void)ID_KERNEL;
        (void)PID;
        (void)TID;
        (void)ACTION;
        // Cette fonction permet de modifier l'etat d'un thread
        // 	ID_KERNEL		: Identificateur unique de l'instance du noyau
        //  PID				: Numero de processus
        //	TID				: Numero du thread
        //	ACTION			: Action a appliquer sur le thread

        int Resultat = 0;

        // Gerer le thread
        // Resultat = this->CPintiCore_Gestionnaire_Taches->Gerer_Threads(ID_KERNEL, PID, TID, ACTION);

        return Resultat;
    }

    uinteger cpinti_etat_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID)
    {
        (void)ID_KERNEL;
        (void)PID;
        // Cette fonction permet de connaitre l'etat d'un thread
        //	TID				: Numero du thread

        return get_EtatThread(TID);
    }

    const char *cpinti_get_nom_thread(uinteger TID)
    {
        // Cette fonction permet de recuperer le nom du thread via son ID
        //  NomThread		: Variable nullptr ou sera stocke le nom de variable

        /* NON UTILISE */

        return gestionnaire_tache::get_NomThread(TID);
    }

    uinteger cpinti_get_nombre_threads()
    {
        // Cette fonction permet d'obtenir le nombre de threads total en cours
        uinteger NombreThread = gestionnaire_tache::get_NombreThreads();

        // Corriger
        if (NombreThread > MAX_THREAD)
            NombreThread = MAX_THREAD;

        return NombreThread;
    }
} // namespace cpinti::gestionnaire_tache