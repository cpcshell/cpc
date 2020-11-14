#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <iostream>

#include <compat/dos.h>
#include <cpinti/debug.h>
#include "core.h"

namespace cpinti::gestionnaire_tache
{

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
        cpinti::debug::trace("Preparating multitask in progress.");

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
        }

        /*** Creer un thread principal "Thread_Updater" ***/
        cpinti::debug::trace("Creating main thread 'Thread_Updater'...");

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

        cpinti::debug::trace(" [OK] TID:0. Offset function 0x%p", Thread_Updater);

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

        cpinti::debug::trace("Closing signal sent to %lu thread(s). Waiting", nombre_threads);

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

        // Si on atteint le nombre maximum de processus
        if (Nombre_Processus >= MAX_PROCESSUS)
        {
            cpinti::debug::error("Unable to attrib new PID. The maximal process number value is %lu", MAX_PROCESSUS);
            return 0;
        }

        cpinti::debug::trace("Creating process '%s'...", NomProcessus);

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
            cpinti::debug::error("Unable to attrib new PID. No free memory");
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

        cpinti::debug::trace("[OK] PID %lu.", Nouveau_PID);

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
            cpinti::debug::trace("Process %lu is already stopped", pid);
        }
        else if (Liste_Processus[pid].Etat_Processus == _EN_ARRET)
        {
            cpinti::debug::trace("Process stopping %lu is already signaled");
        }
        else
        {
            cpinti::debug::trace("Stopping process PID %lu in progress...", pid);

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
                cpinti::debug::trace("Stopping signal has been sent to %d thread(s)", compteur_thread);
            }
            else
            {
                cpinti::debug::trace("Nothing hosted threads in the process. Hm.. perfect!");
            }
            // Declarer le processus mort (On conserve certaines infos pour le debug)
            cpinti::debug::trace("Sending stopping signal to process (Waiting threads closing) ...");

            memset(Liste_Processus[pid].Nom_Processus, 0, 32);
            // free(Liste_Processus[pid].Nom_Processus);

            Liste_Processus[pid].Etat_Processus = _ARRETE;
            Liste_Processus[pid].PID = 0;
        }

        Nombre_Processus--;

        cpinti::debug::trace("Process %lu deleted!", pid);

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
            cpinti::debug::error("Unable to attrib new TID. The maximal thread(s) number value is %d", MAX_THREAD);
            return 0;
        }

        cpinti::debug::trace("Creating thread '%s' in process %lu...", NomThread, pid);

        // Si le processus n'existe pas
        if (Liste_Processus[pid].PID != pid)
        {
            cpinti::debug::error("PID %lu not exist. Unable to host the new thread.", pid);
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
            cpinti::debug::error("Unable to attrib new TID. No free memory");

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

        cpinti::debug::trace(" [OK] TID: %lu. Function offset 0x%p", Nouveau_TID, Fonction);

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
            cpinti::debug::trace("Deleting thread '%s' TID: %lu PID: %lu. %lu thread(s) restant(s)", Liste_Threads[tid].Nom_Thread, tid, Liste_Threads[tid].PID, Nombre_Threads);

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
            cpinti::debug::trace("Sending stopping signal to thread '%lu' PID: %lu", tid, Liste_Threads[tid].PID);

            // Ce changement d'etat va provoquer un arret "automatique" du thread
            // Au bout de quelques secondes le thread va passer en mode "zombie"
            set_EtatThread(tid, _EN_ARRET);
            Liste_Threads[tid].DM_arret = true;
        }

        return true;
    }
} // namespace cpinti::gestionnaire_tache
