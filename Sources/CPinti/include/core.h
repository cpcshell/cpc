#include <pthread.h>
#include <setjmp.h>
#include <stdint.h>
#include <sys/time.h>

#include "cpinti/types.h"

extern "C" void cpc_CX_APM_MODE(uinteger mode);

extern "C" void *Thread_Updater(void *);

extern "C" void Interruption_Timer();

extern "C" void ptr_Update_TID(uinteger Adresse, uinteger TID);

#define ENTRER_SectionCritique cpinti::gestionnaire_tache::begin_SectionCritique
#define SORTIR_SectionCritique cpinti::gestionnaire_tache::end_SectionCritique

namespace cpinti
{
	namespace gestionnaire_tache
	{

		/**** Constantes ****/

#define MAX_THREAD 256	  // Temporaire
#define MAX_PROCESSUS 128 // Temporaire
#define MAX_TIMERS 10
#define MAX_STACK 8

#define MAX_CYCLES 160 /* Nombre de cycles divisable MAX par thread*/
#define _PRIORITE_THRD_TRES_FAIBLE 150
#define _PRIORITE_THRD_ASSEZ_FAIBLE 100
#define _PRIORITE_THRD_FAIBLE 60
#define _PRIORITE_THRD_MOYENNE 50
#define _PRIORITE_THRD_MOYENNE_2 40
#define _PRIORITE_THRD_BIEN 30
#define _PRIORITE_THRD_ASSEZ_HAUTE 20
#define _PRIORITE_THRD_HAUTE 10
#define _PRIORITE_THRD_TRES_HAUTE 2

#define _EN_EXECUTION 0x53 // 83
#define _EN_PAUSE 0x65	   // 101
#define _EN_ATTENTE 0x62   // 98
#define _EN_ARRET 0x46	   // 70
#define _ZOMBIE 0x56	   // 86  /* Bloc memoire pret a etre raze a ZERO Mouahaha! */
#define _ARRETE 0x4C	   // 76

#define _ZOMBIE_ESSAI 24 // Nombre d'essais avant de bannir un thread qui ne repond plus.

#define CX_StandbyMode 0x1
#define CX_SuspendMode 0x2
#define CX_Shutdown 0x3
#define Restart 0x4

		/**** Variables ****/

		extern bool EVALUATION_CPU;
		extern uinteger NombreCycles;
		extern uinteger NombreCyles_MAX;
		extern uinteger InLiveCompteur;
		extern time_t Temps_Depart;
		extern time_t Temps_Actuel;
		extern double Temps_total;
		extern uinteger saut_comptage;
		extern int Compteur_Cycle_cpu;

		extern uinteger Nombre_Processus;

		extern struct itimerval instance_Timer[MAX_TIMERS];
		extern bool SECTION_CRITIQUE;
		extern int compteur;
		extern uinteger Nombre_Tache;
		extern uinteger Nombre_Threads;
		extern uinteger Nombre_Timer;
		extern uinteger Thread_en_cours;

		/**** Structure du thread ****/
		struct liste_threads
		{
			int Priorite;		  /** Priorite 0-128 **/
			int Priorite_count;	  /** Compteur priorite **/
			uinteger Etat_Thread; /** ARRETE, PAUSE, EXECUTION **/
			bool DM_arret;
			uinteger Zombie_Count; /** Nombre boucle avant mort **/
			uinteger KID;		   /** ID kernel **/
			uinteger OID;		   /** ID OS **/
			uinteger UID;		   /** ID USER **/
			uinteger PID;		   /** ID Process **/
			uinteger TID;		   /** ID Thread (Lui meme) **/
			uinteger PTID;		   /** Pthread ID Thread (Lui meme) **/
			char Nom_Thread[32];   /** Nom Thread **/
			pthread_t thread;
		};

		/**** Structure du processus ****/
		struct list_processus
		{
			int Priorite;			 /** **/
			uinteger Etat_Processus; /** ARRETE, PAUSE, EXECUTION **/

			uinteger KID;		 /** ID kernel **/
			uinteger OID;		 /** ID OS **/
			uinteger UID;		 /** ID USER **/
			uinteger PID;		 /** ID Process (Lui meme) **/
			uinteger PID_Parent; /** ID Process (qui l'a cree) **/
			uinteger TID_Parent; /** ID Thread  (Qui l'a cree **/

			char Nom_Processus[32]; /** Nom Processus **/

			bool Threads_Enfant[MAX_THREAD];
			// static liste_threads 	Liste_Threads[MAX_THREAD] = {};
		};

#ifndef Liste_Processus
		extern list_processus Liste_Processus[MAX_PROCESSUS];
#endif

#ifndef Liste_Threads
		extern liste_threads Liste_Threads[MAX_THREAD];
#endif

		/**** Methodes ****/
		bool initialiser_Multitache();
		void IamInLive();
		uinteger get_cycle_cpu();
		uinteger get_cycle_MAX_cpu();
		void eval_cycle_cpu();

		/** Processus **/
		uinteger ajouter_Processus(const char *NomProcessus);
		bool supprimer_Processus(uinteger pid);
		uinteger get_EtatProcessus(uinteger TID);
		void set_EtatProcessus(uinteger TID, uinteger Etat);
		uinteger get_NombreProcessus();

		/** Threads **/
		uinteger ajouter_Thread(void *(*Fonction)(void *arg), const char *NomThread, uinteger pid, int Priorite, uintptr_t Arguments);
		bool supprimer_Thread(uinteger tid, bool force);

		void Interruption_Timer();

		uinteger get_ID_Thread();

		uinteger get_EtatThread(uinteger TID);
		void set_EtatThread(uinteger TID, uinteger Etat);

		uinteger get_TacheEnCours();
		uinteger get_ThreadEnCours();
		uinteger get_NombreThreads();
		const char *get_NomThread(uinteger TID);

		void begin_SectionCritique();
		void end_SectionCritique();
		bool state_SectionCritique();

		bool fermer_core();

	} // namespace gestionnaire_tache

} // namespace cpinti
