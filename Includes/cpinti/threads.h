#ifndef CPCDOS_THREADS
#define CPCDOS_THREADS

#include "cpinti/types.h"

namespace cpinti::gestionnaire_tache
{
	struct thread_args
	{
		/**** Arguments du thread ****/
		static int ARG1;
		static uinteger ARG2;

		static const char *ARG3;
		static const char *ARG4;
		static const char *ARG5;

		/**** Identification du thread ****/
		static uinteger PID;	   /* ID du processus */
		static uinteger TID;	   /* ID du thread */
		static uinteger USER_ID;   /* ID de l'utilisateur */
		static uinteger OS_ID;	   /* ID du systeme d'exploitation */
		static uinteger KERNEL_ID; /* ID du kernel */

		static const char *NomThread; /* Nom du thread */
	};

	uinteger cpinti_creer_thread(uinteger ID_KERNEL, uinteger ID_OS, uinteger ID_USER, uinteger PID, const char *NomThread,
								 int Priorite, void *(*Fonction)(void *arg), void *ARG_CP, uinteger ARG_TH);

	int cpinti_arreter_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID);

	int cpinti_arreter_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID, bool force);

	void *cpinti_get_Arguments();

	uinteger cpinti_joindre_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID, int CYCLES);

	int cpinti_sortir_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID);

	int cpinti_fin_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID);

	void *cpinti_thread_args(int NoARG);

	int cpinti_gerer_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID, uinteger ACTION);

	uinteger cpinti_etat_thread(uinteger ID_KERNEL, uinteger PID, uinteger TID);

	const char *cpinti_get_nom_thread(uinteger TID); // non utilise

	uinteger cpinti_get_nombre_threads();

	bool _exit();

} // namespace cpinti::gestionnaire_tache

#endif /* CPCDOS_THREADS */
