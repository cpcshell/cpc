#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "compat/dos.h"
#include "compat/dpmi.h"
#include "debug.h"
#include "func_cpi.h"

#include "core.h"

namespace cpinti::gestionnaire_tache
{

    /* global var */
    bool EVALUATION_CPU = false;
    uinteger NombreCycles = 0;
    uinteger InLiveCompteur = 0;
    time_t Temps_Depart = 0;
    time_t Temps_Actuel = 0;
    double Temps_total = 0;
    uinteger saut_comptage = 0;
    int Compteur_Cycle_cpu = 0;

    uinteger Nombre_Processus = 0;

    struct itimerval instance_Timer[MAX_TIMERS] = {};
    bool SECTION_CRITIQUE = false;
    int compteur = 0;
    uinteger Nombre_Tache = 0;
    uinteger Nombre_Threads = 0;
    uinteger Nombre_Timer = 0;
    uinteger Thread_en_cours = 0;

#ifndef Liste_Processus
    list_processus Liste_Processus[MAX_PROCESSUS] = {};
#endif

#ifndef Liste_Threads
    liste_threads Liste_Threads[MAX_THREAD] = {};
#endif

    /* end */

    void IamInLive()
    {
        // Ne pas calculer si le CPU est en evaluation
        if (EVALUATION_CPU == false)
        {
            begin_SectionCritique();

            // Cette fonction met a jour les cycles CPU
            //  ce qui permet d'estimer avec une precision de 60%
            //  de la charge du CPU
            InLiveCompteur++;
            if (InLiveCompteur > 27483600) // uinteger
                InLiveCompteur = 0;        // On reinitialise pour eviter les plantages

            // Ceci permet d'economiser du temps CPU
            saut_comptage++;
            if (saut_comptage > 24)
            {
                saut_comptage = 0;
                time(&Temps_Actuel);

                Temps_total = difftime(Temps_Actuel, Temps_Depart);

                if (Temps_total >= 1) // Si 1 seconde
                {
                    NombreCycles = InLiveCompteur;
                    InLiveCompteur = 0; // On reset le compteur
                    time(&Temps_Depart);
                }
            }

            end_SectionCritique();
        }
    }

    volatile int SectionCritique_RECURSIF = 0;
    void begin_SectionCritique()
    {
        // Rendre le systeme non interruptible

        if (SectionCritique_RECURSIF == 0)
        {
            SECTION_CRITIQUE = true;
            disable();
        }

        // Plus on l'appel plus il faudra le remonter pour finir la scope
        SectionCritique_RECURSIF++;
    }

    void end_SectionCritique()
    {
        // Rendre le systeme interruptible

        if (SectionCritique_RECURSIF < 0)
        {
            SectionCritique_RECURSIF--;
        }
        // Rendre interruptible quand le compteur sera a zero
        if (SectionCritique_RECURSIF == 0)
        {
            SECTION_CRITIQUE = false;
            enable();
        }
    }

    bool state_SectionCritique()
    {
        // Retourner l'etat de la section critique
        return SECTION_CRITIQUE;
    }

    bool initialiser_Multitache()
    {
        // Initialiser le multitasking
        cpinti_dbg::CPINTI_DEBUG("Preparation du multitache en cours.",
                                 "Preparating multitask in progress. ",
                                 "core::gestionnaire_tache", "initialiser_Multitache()",
                                 Ligne_saute, Alerte_surbrille, Date_avec, Ligne_r_normal);

        time(&Temps_Depart);

        // Interdire toutes interruptions
        begin_SectionCritique();

        // Allouer un espace memoire pour chaque threads
        for (int index_id = 0; index_id <= MAX_THREAD - 1; index_id++)
        {
            Liste_Threads[index_id].Etat_Thread = _ARRETE;
            Liste_Threads[index_id].Priorite = 0;
            Liste_Threads[index_id].KID = 0;
            Liste_Threads[index_id].OID = 0;
            Liste_Threads[index_id].UID = 0;
            Liste_Threads[index_id].PID = 0;

            // strncpy((char*) Liste_Threads[index_id].Nom_Thread, (const char*) '\0', 32);
        }

        /*** Creer un thread principal "Thread_Updater" ***/

        cpinti_dbg::CPINTI_DEBUG("Creation du thread principal 'Thread_Updater'...",
                                 "Creating main thread 'Thread_Updater'...",
                                 "core::gestionnaire_tache", "initialiser_Multitache()",
                                 Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

        // Incremente le nombre de threads
        Nombre_Threads++;

        // Priorite
        Liste_Threads[0].Priorite = _PRIORITE_THRD_FAIBLE;

        // Son numero de TID (Thread IDentifiant)
        Liste_Threads[0].TID = 0;

        // Etat en execution (A modifier en PAUSE)
        Liste_Threads[0].Etat_Thread = _EN_EXECUTION;

        Liste_Threads[0].DM_arret = false;

        int toto = 0;
        pthread_create(&Liste_Threads->thread, nullptr, Thread_Updater, (void *)&toto);

        // std::string offset_fonction = std::to_string((uinteger) Thread_Updater);
        cpinti_dbg::CPINTI_DEBUG(" [OK] TID:0. Fonction offset 0x" + std::to_string((uintptr_t)Thread_Updater),
                                 " [OK] TID:0. Offset function 0x" + std::to_string((uintptr_t)Thread_Updater),
                                 "", "",
                                 Ligne_saute, Alerte_validation, Date_sans, Ligne_r_normal);

        // Reexecuter le scheduler normalement
        end_SectionCritique();

        Thread_en_cours = 0;

        // Retourner l'ID
        return true;
    }

    bool fermer_core()
    {
        uinteger nombre_threads = 0;
        // Cette fonction permet de fermer le core en terminant tous les threads
        for (uinteger boucle = 1; boucle < MAX_THREAD; boucle++)
        {
            // Si le thread n'est pas arrete ou n'est pas zombie on ferme
            if (Liste_Threads[boucle].Etat_Thread != _ARRETE)
                if (Liste_Threads[boucle].Etat_Thread != _ZOMBIE)
                {
                    nombre_threads++;
                    supprimer_Thread(boucle, false);
                }
        }

        std::string nombre_threads_STR = std::to_string((uinteger)nombre_threads);
        cpinti_dbg::CPINTI_DEBUG("Signal de fermeture envoye aux " + nombre_threads_STR + " thread(s). Attente",
                                 "Closing signal sent to " + nombre_threads_STR + " threads(s). Waiting",
                                 "", "",
                                 Ligne_saute, Alerte_avertissement, Date_sans, Ligne_r_normal);
        fflush(stdout);

        // On attend un peut
        usleep(1500000);

        fflush(stdout);

        // Bloquer tout autres interruptions
        ENTRER_SectionCritique();

        puts("Bye from kernel\n");

        // Activer les interruptions materiel
        enable();

        exit(0);

        return true;
    }

    /******** PROCESSUS ********/

    uinteger get_EtatProcessus(uinteger PID)
    {
        // Obtenir l'etat d'un processus
        return Liste_Processus[PID].Etat_Processus;
    }

    void set_EtatProcessus(uinteger PID, uinteger Etat)
    {
        // Definir l'etat d'un processus
        Liste_Processus[PID].Etat_Processus = Etat;
    }

    uinteger get_NombreProcessus()
    {
        // Retourner le nombre de threads en cours
        return Nombre_Processus;
    }

    uinteger ajouter_Processus(const char *NomProcessus)
    {
        // Cette fonction permet de creer un processus pour heberger des threads

        std::string NomProcessus_STR = NomProcessus;

        // Si on atteint le nombre maximum de processus
        if (Nombre_Processus >= MAX_PROCESSUS)
        {
            std::string nombre_processus_STR = std::to_string((uinteger)MAX_PROCESSUS);
            cpinti_dbg::CPINTI_DEBUG("[ERREUR] Impossible d'attribuer un nouveau PID. Le nombre est fixe a " + nombre_processus_STR + " processus maximum.",
                                     "[ERROR] Unable to attrib new PID. The maximal process number value is " + nombre_processus_STR,
                                     "", "",
                                     Ligne_saute, Alerte_erreur, Date_avec, Ligne_r_normal);
            return 0;
        }

        cpinti_dbg::CPINTI_DEBUG("Creation du processus '" + NomProcessus_STR + "'...",
                                 "Creating process '" + NomProcessus_STR + "'...",
                                 "core::gestionnaire_tache", "ajouter_Processus()",
                                 Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

        uinteger Nouveau_PID = 0;

        // Rechercher un emplacement ID vide
        for (uinteger b = 1; b <= MAX_PROCESSUS; b++)
        {
            if (Liste_Processus[b].PID == 0)
            {
                Nouveau_PID = b;
                break;
            }
        }
        if (Nouveau_PID == 0)
        {
            std::string nombre_threads_STR = std::to_string((uinteger)MAX_THREAD);
            cpinti_dbg::CPINTI_DEBUG(" [ERREUR] Impossible d'attribuer un nouveau PID. Aucune zone memoire libere",
                                     " [ERROR] Unable to attrib new PID. No free memory",
                                     "", "",
                                     Ligne_saute, Alerte_erreur, Date_sans, Ligne_r_normal);
            return 0;
        }

        // Incremente le nombre de processus
        Nombre_Processus++;

        // Nom du processus
        strncpy((char *)Liste_Processus[Nouveau_PID].Nom_Processus, NomProcessus, 32);

        // Son numero de TID (Thread IDentifiant)
        Liste_Processus[Nouveau_PID].PID = Nouveau_PID;

        // Etat en execution
        Liste_Processus[Nouveau_PID].Etat_Processus = _EN_EXECUTION;

        std::string Nouveau_PID_STR = std::to_string((uinteger)Nouveau_PID);
        cpinti_dbg::CPINTI_DEBUG(" [OK] PID " + Nouveau_PID_STR + ".",
                                 " [OK] PID " + Nouveau_PID_STR + ".",
                                 "", "",
                                 Ligne_saute, Alerte_validation, Date_sans, Ligne_r_normal);

        // Retourner l'ID
        return Nouveau_PID;
    }

    bool supprimer_Processus(uinteger pid)
    {
        // Cette fonction permet de signaler l'arret d'un processus
        //  et donc de tous ses threads
        //  si force=true
        volatile int compteur_thread = 0;

        if (pid == 0)
            return false;

        // std::string pid_STR = std::to_string((uinteger) pid);

        if (Liste_Processus[pid].Etat_Processus == _ARRETE)
        {
            cpinti_dbg::CPINTI_DEBUG("Le processus " + std::to_string((uinteger)pid) + " est deja arrete",
                                     "Process " + std::to_string((uinteger)pid) + " is already stopped",
                                     "", "",
                                     Ligne_saute, Alerte_avertissement, Date_sans, Ligne_r_normal);
        }
        else if (Liste_Processus[pid].Etat_Processus == _EN_ARRET)
        {
            cpinti_dbg::CPINTI_DEBUG("Arret du processus " + std::to_string((uinteger)pid) + " deja signale",
                                     "Process stopping " + std::to_string((uinteger)pid) + " is already signaled",
                                     "", "",
                                     Ligne_saute, Alerte_avertissement, Date_sans, Ligne_r_normal);
        }
        else
        {
            cpinti_dbg::CPINTI_DEBUG("Arret du processus PID " + std::to_string((uinteger)pid) + " en cours...",
                                     "Stopping process PID " + std::to_string((uinteger)pid) + " in progress",
                                     "core::gestionnaire_tache", "supprimer_Processus()",
                                     Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

            // Mettre le processus en etat STOP = 0
            Liste_Processus[pid].Etat_Processus = _EN_ARRET;

            // Signaler l'arret a tous les threads heberges
            for (uinteger tid = 1; tid < MAX_THREAD; tid++)
            {
                // Si le thread est bien heberge dans le processus
                if (Liste_Processus[pid].Threads_Enfant[tid] == true)
                {
                    // Attendre 1ms entre chaque signalements
                    usleep(100);

                    // On signale l'arret du thread
                    supprimer_Thread(tid, false);

                    // Compter le nombre de threads signales
                    compteur_thread++;
                }

                // Dans tous les cas on met a FALSE
                Liste_Processus[pid].Threads_Enfant[tid] = false;
            }

            // Attendre 10ms pour etre SAFE
            usleep(1000);

            if (compteur_thread > 0)
            {
                // std::string compteur_thread_STR = std::to_string(compteur_thread);

                // Declarer le processus mort (On conserve certaines infos pour le debug)
                cpinti_dbg::CPINTI_DEBUG("Un signal d'arret a ete envoye a " + std::to_string(compteur_thread) + " thread(s)",
                                         "Stopping signal has been sent to " + std::to_string(compteur_thread) + " thread(s)",
                                         "core::gestionnaire_tache", "supprimer_Processus()",
                                         Ligne_saute, Alerte_ok, Date_avec, Ligne_r_normal);
            }
            else
            {
                cpinti_dbg::CPINTI_DEBUG("Aucun threads heberge dans le processus. Hm.. parfait!",
                                         "Nothing hosted threads in the process. Hm.. perfect!",
                                         "core::gestionnaire_tache", "supprimer_Processus()",
                                         Ligne_saute, Alerte_ok, Date_avec, Ligne_r_normal);
            }
            // Declarer le processus mort (On conserve certaines infos pour le debug)
            cpinti_dbg::CPINTI_DEBUG("Envoi d'un signal d'arret au processus (Attente de la fermeture des threads) ...",
                                     "Sending stopping signal to process (Waiting threads closing) ...",
                                     "core::gestionnaire_tache", "supprimer_Processus()",
                                     Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

            memset(Liste_Processus[pid].Nom_Processus, 0, 32);
            // free(Liste_Processus[pid].Nom_Processus);

            Liste_Processus[pid].Etat_Processus = _ARRETE;
            Liste_Processus[pid].PID = 0;

            cpinti_dbg::CPINTI_DEBUG(" [OK]",
                                     " [OK]",
                                     "", "",
                                     Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);
        }

        Nombre_Processus--;

        cpinti_dbg::CPINTI_DEBUG("Processus " + std::to_string((uinteger)pid) + " supprime!",
                                 "Process " + std::to_string((uinteger)pid) + " deleted!",
                                 "core::gestionnaire_tache", "supprimer_Processus()",
                                 Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

        return true;
    }

    /******** THREADS ********/

    uinteger get_EtatThread(uinteger TID)
    {
        return Liste_Threads[TID].Etat_Thread;
    }

    void set_EtatThread(uinteger TID, uinteger Etat)
    {
        Liste_Threads[TID].Etat_Thread = Etat;
    }

    const char *get_NomThread(uinteger TID)
    {
        return (const char *)Liste_Threads[TID].Nom_Thread;
    }

    uinteger get_NombreThreads()
    {
        // Retourner le nombre de threads en cours
        return Nombre_Threads;
    }

    uinteger get_ThreadEnCours()
    {
        // Retourner la thread en cours
        return Thread_en_cours;
    }

    uinteger ajouter_Thread(void *(*Fonction)(void *arg), const char *NomThread, uinteger pid, int Priorite, uintptr_t Arguments)
    {
        // Cette fonction permet d'ajouter une thread (Thread)
        uinteger Nouveau_TID = 0;

        // std::string NomThread_STR = NomThread;

        // Si on atteint le nombre maximum de threads
        if (Nombre_Threads >= MAX_THREAD)
        {
            // std::string nombre_threads_STR = std::to_string((uinteger) MAX_THREAD);
            cpinti_dbg::CPINTI_DEBUG("[ERREUR] Impossible d'attribuer un nouveau TID. Le nombre est fixe a " + std::to_string(MAX_THREAD) + " thread(s) maximum.",
                                     "[ERROR] Unable to attrib new TID. The maximal thread(s) number value is " + std::to_string(MAX_THREAD),
                                     "", "ajouter_Thread()",
                                     Ligne_saute, Alerte_erreur, Date_avec, Ligne_r_normal);
            return 0;
        }

        std::string pid_STR = std::to_string((uinteger)pid);

        cpinti_dbg::CPINTI_DEBUG("Creation du thread '" + std::string(NomThread) + "' dans le processus " + pid_STR + "...",
                                 "Creating thread '" + std::string(NomThread) + "' in the process " + pid_STR + "...",
                                 "core::gestionnaire_tache", "ajouter_Thread()",
                                 Ligne_reste, Alerte_action, Date_avec, Ligne_r_normal);

        // Si le processus n'existe pas
        if (Liste_Processus[pid].PID != pid)
        {
            cpinti_dbg::CPINTI_DEBUG(" [ERREUR] Le PID " + pid_STR + " n'existe pas. Impossible d'heberger un nouveau thread.",
                                     " [ERROR] PID " + pid_STR + " not exist. Unable to host the new thread.",
                                     "", "ajouter_Thread()",
                                     Ligne_saute, Alerte_erreur, Date_sans, Ligne_r_normal);
            return 0;
        }

        ENTRER_SectionCritique();
        // Rechercher un emplacement ID vide
        for (uinteger b = 1; b <= MAX_THREAD; b++)
        {
            if (Liste_Threads[b].PID == 0)
            {
                Nouveau_TID = b;
                break;
            }
        }

        if (Nouveau_TID == 0)
        {

            cpinti_dbg::CPINTI_DEBUG(" [ERREUR] Impossible d'attribuer un nouveau TID. Aucune zone memoire libere",
                                     " [ERROR] Unable to attrib new TID. No free memory",
                                     "core::gestionnaire_tache", "ajouter_Thread()",
                                     Ligne_saute, Alerte_erreur, Date_sans, Ligne_r_normal);

            SORTIR_SectionCritique();

            return 0;
        }

        // Incremente le nombre de threads
        Nombre_Threads++;

        // Nom du thread
        strncpy((char *)Liste_Threads[Nouveau_TID].Nom_Thread, NomThread, 30);

        // Corriger les priorites
        if (Liste_Threads[Thread_en_cours].Priorite < 2)
        {
            Priorite = 2;
        }
        else if (Liste_Threads[Thread_en_cours].Priorite > MAX_CYCLES)
        {
            Priorite = MAX_CYCLES;
        }

        // Priorite
        Liste_Threads[Nouveau_TID].Priorite = Priorite;

        // Son numero de PID (Processus IDentifiant)
        Liste_Threads[Nouveau_TID].PID = pid;

        // Son numero de TID (Thread IDentifiant)
        Liste_Threads[Nouveau_TID].TID = Nouveau_TID;

        // Etat en execution (A modifier en PAUSE)
        Liste_Threads[Nouveau_TID].Etat_Thread = _EN_EXECUTION;

        // NE pas demander d'arret, ca serai un peu con
        Liste_Threads[Nouveau_TID].DM_arret = false;

        // Incrire le thread dans le processsus
        Liste_Processus[pid].Threads_Enfant[Nouveau_TID] = true;

        // Mettre a jour le TID dans l'adresse memoire
        ptr_Update_TID(Arguments, Nouveau_TID);

        // Creer le thread
        pthread_create(&Liste_Threads[Nouveau_TID].thread, nullptr, *Fonction, (void *)Arguments);

        // Liste_Threads[Nouveau_TID].PTID = (uinteger) &Liste_Threads[Nouveau_TID].thread;

        // std::string offset_fonction_STR = std::to_string((uinteger) Fonction);
        // std::string tid_STR = std::to_string((uinteger) Nouveau_TID);
        cpinti_dbg::CPINTI_DEBUG(" [OK] TID:" + std::to_string((uinteger)Nouveau_TID) + ". Fonction offset 0x" + std::to_string((uintptr_t)Fonction),
                                 " [OK] TID:" + std::to_string((uinteger)Nouveau_TID) + ". Offset function 0x" + std::to_string((uintptr_t)Fonction),
                                 "", "",
                                 Ligne_saute, Alerte_validation, Date_sans, Ligne_r_normal);

        SORTIR_SectionCritique();

        Thread_en_cours = Nouveau_TID;

        // Retourner l'ID
        return Nouveau_TID;
    }

    bool supprimer_Thread(uinteger tid, bool force)
    {
        // Cette fonction permet de signaler l'arret d'un thread
        //  si force=true

        if (force == true)
        {

            Nombre_Threads--;

            cpinti_dbg::CPINTI_DEBUG("Suppression du thread '" + std::string(Liste_Threads[tid].Nom_Thread) + "' TID:" + std::to_string((uinteger)tid) + " PID:" + std::to_string((uinteger)Liste_Threads[tid].PID) + ". " + std::to_string((uinteger)Nombre_Threads) + " thread(s) restant(s)",
                                     "Deleting thread '" + std::string(Liste_Threads[tid].Nom_Thread) + "' TID:" + std::to_string((uinteger)tid) + " PID:" + std::to_string((uinteger)Liste_Threads[tid].PID) + ". " + std::to_string((uinteger)Nombre_Threads) + "remaining thread(s)",
                                     "core::gestionnaire_tache", "supprimer_Thread()",
                                     Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

            Liste_Processus[Liste_Threads[tid].PID].Threads_Enfant[tid] = false;

            Liste_Threads[tid].Nom_Thread[0] = '\0';
            Liste_Threads[tid].Priorite = 0;
            Liste_Threads[tid].PID = 0;
            Liste_Threads[tid].TID = 0;
            Liste_Threads[tid].Etat_Thread = _ARRETE;

            // Quitter le thread
            pthread_exit(&Liste_Threads[tid].thread);
        }
        else
        {
            cpinti_dbg::CPINTI_DEBUG("Envoi d'un signal d'arret au thread '" + std::to_string((uinteger)tid) + "' PID:" + std::to_string((uinteger)Liste_Threads[tid].PID),
                                     "Sending stopping signal to thread '" + std::to_string((uinteger)tid) + "' PID:" + std::to_string((uinteger)Liste_Threads[tid].PID),
                                     "core::gestionnaire_tache", "supprimer_Thread()",
                                     Ligne_saute, Alerte_ok, Date_sans, Ligne_r_normal);

            // Ce changement d'etat va provoquer un arret "automatique" du thread
            // Au bout de quelques secondes le thread va passer en mode "zombie"
            set_EtatThread(tid, _EN_ARRET);
            Liste_Threads[tid].DM_arret = true;
        }

        return true;
    }
} // namespace cpinti::gestionnaire_tache
