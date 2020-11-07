#define DWORD uinteger

Extern "C++" lib "cpnti"

	declare sub dump_page(NumPAGE as uinteger)
	declare function get_numbers_of_pages() as uinteger

	declare sub main_exec_thread()

	namespace cpinti_dbg
		Declare sub 		debug_mode		(a as integer)
	End namespace

	' == WRAPPER MODULES C++ ==
	namespace Wrapper_Cpcdos
		declare function cpinti_WRAPPER			cdecl (FunctionID as integer, _CLE_ as double, ARG_1 as CONST ZString PTR, ARG_2 as integer, ARG_3 as any ptr, ARG_4 as any ptr, ARG_5 as any ptr) as integer
	End namespace

	namespace cpinti

		' == NETWORK ==
		namespace net
			declare function cpinti_ping_icmp			(AdresseIP as CONST ZString ptr, Message as CONST ZString ptr) as integer
			declare function cpinti_serveur				(port as uinteger, NombreClients as integer, ID as uinteger, TypeServeur as integer) as integer
			declare function cpinti_client				(AdresseDistant as CONST ZString ptr, port as uinteger, PID as uinteger, TypeClient as integer) as integer
		end namespace

		declare function cpinti_GEST_BUFF_c			(_ID as uinteger, _Source as integer, _DONNEES as ZString ptr) as integer

		' == FICHIERS ==
		namespace gestionnaire_fichier

			declare function cpinti_decompress_file			(Source as CONST ZString ptr, Destination as CONST ZString ptr) as integer
			declare function cpinti_compress_file			(Source as CONST ZString ptr, Destination as CONST ZString ptr) as integer

			declare function cpinti_Fichier_Existe			(Source as CONST ZString ptr) as boolean
			Declare function cpinti_Taille_Fichier			(Source as CONST ZString ptr) as double
			declare function cpinti_Lire_Fichier_complet	(Source as CONST ZString ptr, Mode as CONST ZString ptr, Retour_ptr as ZString ptr, TailleFichier as uinteger) as boolean
			declare function cpinti_Supprimer_Fichier		(Source as CONST ZString ptr, Securise as boolean, NombrePasses as integer) as boolean
			declare function cpinti_Copier_Fichier			(Source as CONST ZString ptr, Destination as CONST ZString ptr, Priorite as integer, VAR_Progression as CONST ZString ptr, VAR_Octets as CONST ZString ptr, VAR_OctetsParSecondes as CONST ZString ptr) as boolean
		end namespace

		' == EXTRA ==



		Declare sub 		CPINTI_DEBUG_C	(TexteFrancais as CONST ZString ptr, TexteAnglais as CONST ZString ptr, Declancheur as CONST ZString ptr, NomFonction as CONST ZString ptr, DebutLigne as integer, NiveauAlerte as integer)

		' ***********************************************************
		' ****** G E S T I O N N A I R E   D E S   T A C H E S ******
		' ***********************************************************

		declare sub cpinti_doevents						(Temps as uinteger)
		declare sub cpinti_Sleep						(Temps_MS as uinteger)
		declare sub cpinti_USleep						(Temps_US as uinteger)

		' == INTERCEPTION D'ERREURS ==
		namespace signals
			declare sub panic (numsignal as integer, ligne as integer, fichier as ZString PTR, fonction as ZString ptr)
		End namespace

		' == TASK MANAGER ==
		namespace gestionnaire_tache


			declare function 	_exit 					() as boolean
			declare function	state_SectionCritique	cdecl() as boolean
			declare sub			begin_SectionCritique	cdecl()
			declare sub			end_SectionCritique		cdecl()
			declare function	initialiser_Multitache	cdecl() as boolean


			' === P R O C E S S U S ===
			declare function 	cpinti_creer_processus			cdecl(ID_KERNEL as uinteger, ID_OS as uinteger, ID_USER as uinteger, _
																	PID_PARENT as uinteger, NomProcessus as CONST ZString PTR) as uinteger
			declare function 	cpinti_arreter_processus		cdecl(ID_KERNEL as uinteger, PID as uinteger) as boolean
			declare sub		 	cpinti_set_etat_processus		cdecl(ID_KERNEL as uinteger, PID as uinteger, ACTION as uinteger)
			declare function 	cpinti_get_etat_processus		cdecl(ID_KERNEL as uinteger, PID as uinteger) as uinteger
			declare function 	cpinti_get_nom_processus		cdecl(PID as uinteger) as CONST ZString PTR
			declare function	cpinti_get_nombre_processus		cdecl() as uinteger


			' === T H R E A D S ===
			declare function 	cpinti_creer_thread		cdecl (ID_KERNEL as uinteger, ID_OS as uinteger, ID_USER as uinteger, _
																PID as uinteger,  NomThread as CONST ZString PTR, Priorite as integer, Fonction as function (arg as any ptr) as any ptr, ARG_CP as any ptr, ARG_TH as uinteger) as uinteger
			declare function 	cpinti_arreter_thread	cdecl (ID_KERNEL as uinteger, ID_OS as uinteger, PID as uinteger, force as boolean) as boolean
			declare function 	cpinti_get_Arguments	cdecl () as any ptr
			declare function 	cpinti_etat_thread		cdecl (ID_KERNEL as uinteger, PID as uinteger, TID as uinteger) as uinteger
			declare function 	cpinti_fin_thread		cdecl (ID_KERNEL as uinteger, PID as uinteger, TID as uinteger) as boolean
			declare function 	cpinti_get_nombre_threads cdecl() as uinteger
			declare function 	cpinti_get_nom_thread	cdecl (TID as uinteger) as CONST ZString ptr

			declare function 	get_ThreadEnCours		cdecl () as uinteger

			declare sub			IamInLive				cdecl ()
			declare function	get_cycle_cpu			cdecl () as uinteger
			declare function	get_cycle_MAX_cpu		cdecl () as uinteger

			' === S H E D U L E R ===
			declare sub			Interruption_Timer		cdecl ()

		end namespace ' gestionnaire taches
	end namespace ' cpinti
end extern

declare sub fb_Sleep alias "fb_Sleep" ( byval amount as long = -1 )

#define doevents(temps) 				cpinti.cpinti_doevents(temps)
#define ENTRER_SectionCritique() 		cpinti.gestionnaire_tache.begin_SectionCritique()
#define SORTIR_SectionCritique() 		cpinti.gestionnaire_tache.end_SectionCritique()

type FnIntHandler as function cdecl( byval as uinteger) as integer

declare function fb_dos_timer_handler cdecl alias "fb_dos_timer_handler" (byval irq as uinteger) as integer

declare function fb_isr_set cdecl alias "fb_isr_set"( _
	byval irq_number as uinteger, _
	byval pfnIntHandler as FnIntHandler, _
	byval size as uinteger, _
	byval stack_size as uinteger = 0) as integer

declare function fb_isr_reset cdecl alias "fb_isr_reset"( _
	byval irq_number as uinteger ) as integer

declare function fb_isr_get cdecl alias "fb_isr_get"( _
	byval irq_number as uinteger ) as FnIntHandler


dim shared isr_data_start as byte
dim shared timer_ticks as integer
dim shared old_isr as FnIntHandler
dim shared isr_data_end as byte
