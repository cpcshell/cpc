/* Entete de taches.cpp */

#include <sys/time.h>

#ifndef doevents
#define doevents(temps) cpinti::cpinti_doevents(temps)
#endif

#ifndef DEF_gestionnaire_tache
#define DEF_gestionnaire_tache 1
namespace gestionnaire_tache
{

#define _MAX_PROCESSUS 1024
#define _MAX_THREADS 6500

#define _EN_EXECUTION 0x53
#define _EN_PAUSE 0x65
#define _EN_ATTENTE 0x62
#define _EN_ARRET 0x46
#define _ARRETE 0x4C /* Bloc memoire pret a etre raze a ZERO Mouahaha! */

	class Gestionnaire_des_taches
	{
	private:
		// **********************************************
		// **** Structure d'une instance d'un thread ****
		// **********************************************

		struct thread_instance
		{
		public:
			uinteger ID_KERNEL; // Numero d'instance KERNEL
			uinteger PID;		// Numero de PID associe

			std::string NomThread; // Nom du thread

			uinteger ID_THREAD; // Numero d'instance du thread

			uinteger Etat; // Etat du thread (Execution, pause...)
			int Priorite;  // Priorite du thread courant (Ordonnancement)

			uinteger taille_allocation; // Taille du bloc memoire en octets
			uinteger taille_heap;		// Taille de la heap en octets

			// Instance d'un thread dans un smart pointer
			// std::shared_ptr<thread_instance_AUTO> THREAD_INSTANCE; /*(new thread_instance_AUTO);*/

			void *adresse_offset_DEBUT; // Adresse memoire depart
			void *adresse_offset_FIN;	// Adresse memoire fin
		};

		// *************************************************
		// **** Structure d'une instance d'un processus ****
		// *************************************************

		struct processus_instance
		{
		public:
			uinteger ID_KERNEL; // Numero d'instance KERNEL
			uinteger PID;		// Numero de PID
			uinteger NB_Thread; // Nombre de threads associe au processus

			uinteger Etat; // Etat du thread (Execution, pause...)

			std::string NomProc; // Nom du processus

			// Tableau contenant la liste des threads
			// std::vector<std::shared_ptr<thread_instance>> threads_liste;
		};

		static bool switch_SIMPLE_doevents;
		static uinteger NB_Cycle_CPU;
		static uinteger NB_MAX_Cycle_CPU;
		static uinteger Cycle_CPU_res;
		static uinteger Test_TIME;

		static uinteger NB_processus; // Nombre de processus TOTAL
		static uinteger NB_thread;	  // Nombre de threads TOTAL

		static time_t Temps_DEPART;
		static time_t Temps_ARRIVE;
		static double Temps_Total;

		std::vector<processus_instance> processus_liste;
		processus_instance myTEMP_proc;

	public:
		int init__gestionnaire(uinteger _ID, int P0);

		/***************************************************************/
		/********************** P R O C E S S U S **********************/
		/***************************************************************/
		uinteger Creer_Processus(uinteger ID_KERNEL, const std::string NomProcessus);
		uinteger Etat_Processus(uinteger ID_KERNEL, uinteger PID);
		int Gerer_Processus(uinteger ID_KERNEL, uinteger PID, uinteger ACTION);
		int Arreter_Processus(uinteger ID_KERNEL, uinteger PID);
		std::string get_Nom_Processus(uinteger ID_KERNEL, uinteger PID);

		/***************************************************************/
		/************************ T H R E A D S ************************/
		/***************************************************************/
		uinteger Creer_Thread(uinteger ID_KERNEL, uinteger PID, const std::string NomThread,
							  int Priorite, void *(*Fonction)(void *));
		uinteger Joindre_Thread(uinteger ID_KERNEL, uinteger PID, uinteger TID, int CYCLES);
		uinteger Etat_Thread(uinteger ID_KERNEL, uinteger PID, uinteger TID);
		std::string get_Nom_Thread(uinteger ID_KERNEL, uinteger PID, uinteger TID);
		int Gerer_Threads(uinteger ID_KERNEL, uinteger PID, uinteger TID, uinteger ACTION);
		int Arreter_Thread(uinteger ID_KERNEL, uinteger PID, uinteger TID);
		void Arreter_Thread_SANS_ID(uinteger INDEX_PROCESSUS, uinteger INDEX_THREAD);
		uinteger Thread_est_arrete(uinteger ID_KERNEL, bool Fermer);

		// Obtenir le nombre de processus total
		static uinteger get_Nombre_processus();
		static uinteger get_Nombre_threads();

		static void set_moins_Nombre_thread();
		static void set_plus_Nombre_thread();

		// Obtenir le nombre de threads dans un processus (via son numero de PID)
		uinteger get_Nombre_thread(unsigned PID);

		static void Appeler_Scheduler_CPintiCore();

		static void Appeler_Scheduler_CPintiCore(uinteger Temps);

		// static void ViderVector_Thread			(std::vector <Gestionnaire_des_taches::thread_instance*>
		// &VectorThread);

		static void Update_cycle_CPU();
		static uinteger get_Nombre_cycle_CPU();
		static uinteger get_NB_MAX_Cycle_CPU();
		static void set_NB_MAX_Cycle_CPU(uinteger NombreMax);
		static int get_pourcent_CPU();

		// Constructeur
		Gestionnaire_des_taches();

		// Desctructeur
		~Gestionnaire_des_taches();
	};
} // namespace gestionnaire_tache
#endif