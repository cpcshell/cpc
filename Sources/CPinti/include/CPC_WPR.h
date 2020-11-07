#ifndef CPCDOS_CPC_WPR
#define CPCDOS_CPC_WPR

#include "cpinti.h"

extern "C" int cpc_Creer_Contexte(int TAILLEX, int TAILLEYn);
extern "C" void *cpc_Obtenir_Zone_Contexte(int ID);
extern "C" int cpc_Blitter(int ID);
extern "C" void cpc_CCP_Exec_Commande(const char *COMMANDE, int ID);
extern "C" int cpc_CCP_Exec_Thread_cpc(const char *chemin, int Thread_Priorite);

extern "C" char *cpc_Exec_en_cours(int ARG);

extern "C" void cpc_cpinti_debug_plus_cpinticore(const char *texte, int Ecran, int Log, int Alerte, int RetourPLGN, int CR_LF, int DisplDate, int Signature, const char *File);
extern "C" void cpc_cpinti_debug_cpinticore(const char *texte, int alerte);
extern "C" int cpc_cpinti_Fichier_Existe(const char *chemin);
extern "C" uinteger cpc_cpinti_Taille_Fichier(const char *chemin);
extern "C" int cpc_cpinti_Lire_Fichier_complet(const char *Chemin, const char *Mode, char *retour_str);

extern "C" void cpc_cpinti_sleep_us(uinteger temp_us);
extern "C" void cpc_cpinti_sleep_ms(uinteger temp_ms);
extern "C" void cpc_cpinti_sleep_sec(uinteger temp_sec);

extern "C" double cpc_cpinti_Obtenir_Temps_ms(double Temps_av);

#ifndef DEFINITION_WRAPPER
#    define DEFINITION_WRAPPER

#    include <string.h> // Pour memset

class CpcdosOSx_CPintiCore
{
public:
    // ************ For GZE 3D Graphic engine ************
    int Create_Context(int TAILLEX, int TAILLEYn); // Creer un contexte de donnees
    void *Init_Get_Context_PTR(int ID);            // Initialiser et obtenir le pointeur du contexte
    void Blitting(int ID);                         // Blitter le buffer

    // ***************** CpcdosC+ Engine *****************
    void Shell_CCP(const char *COMMAND, int LEVEL);        // Executer une commande CpcdosC+<
    int Shell_FILE(const char *FILE, int THREAD_PRIORITY); // Executer un fichier CpcdosC+ multithread� ou pas

    // ******************** Cpcdos OSx ********************
    char *Get_Path(int ARG); // Obtenir le chemin d'acces en cours

    // ******************* CPinti Core *******************

    int File_exist(char *path);           // Renvoie 1 OK 0 FAUX
    int File_exist(const char *path);     // Renvoie 1 OK 0 FAUX
    uinteger File_size(char *path);       // Obtenir la taille d'un fichier
    uinteger File_size(const char *path); // Obtenir la taille d'un fichier
    int File_read_all(const char *path, const char *mode,
                      char *data); // Lire TOUT le contenu d'un fichier
    int File_read_all(char *path, char *mode,
                      char *data);

    void usleep(uinteger microseconds);  // Pause en micro secondes
    void sleep(uinteger milliseconds);   // Pause en milli secondes
    void ssleep(uinteger seconds);       // Pause en secondes
    double get_time_ms(double Temps_av); // Obtenir le temps actuel en millisecondes
};

//////////////////// For GZE 3D Graphic engine ////////////////////
int CpcdosOSx_CPintiCore::Create_Context(int TAILLEX, int TAILLEYn)
{
    return cpc_Creer_Contexte(TAILLEX, TAILLEYn);
}

void *CpcdosOSx_CPintiCore::Init_Get_Context_PTR(int ID)
{
    return cpc_Obtenir_Zone_Contexte(ID);
}

void CpcdosOSx_CPintiCore::Blitting(int ID)
{
    cpc_Blitter(ID);
}

////////////////////// For CpcdosC+ //////////////////////
void CpcdosOSx_CPintiCore::Shell_CCP(const char *COMMAND, int LEVEL)
{
    cpc_CCP_Exec_Commande(COMMAND, LEVEL);
}

int CpcdosOSx_CPintiCore::Shell_FILE(const char *_file, int Thread_Priority)
{
    return cpc_CCP_Exec_Thread_cpc(_file, Thread_Priority);
}

//////////////////// For Cpcdos OSx ////////////////////
char *CpcdosOSx_CPintiCore::Get_Path(int ARG)
{
    return cpc_Exec_en_cours(ARG);
}

//////////////////// For CPinti Core ////////////////////

// ---
int CpcdosOSx_CPintiCore::File_exist(char *path)
{
    return cpc_cpinti_Fichier_Existe(path);
}
int CpcdosOSx_CPintiCore::File_exist(const char *path)
{
    return cpc_cpinti_Fichier_Existe(path);
}
// ---
uinteger CpcdosOSx_CPintiCore::File_size(char *path)
{
    return cpc_cpinti_Taille_Fichier(path);
}
uinteger CpcdosOSx_CPintiCore::File_size(const char *path)
{
    return cpc_cpinti_Taille_Fichier(path);
}
// ---
int CpcdosOSx_CPintiCore::File_read_all(const char *path, const char *mode, char *data)
{
    return cpc_cpinti_Lire_Fichier_complet(path, mode, data);
}
int CpcdosOSx_CPintiCore::File_read_all(char *path, char *mode, char *data)
{
    return cpc_cpinti_Lire_Fichier_complet(path, mode, data);
}
// ---

// Time
void CpcdosOSx_CPintiCore::usleep(uinteger microseconds)
{
    cpc_cpinti_sleep_us(microseconds);
}
void CpcdosOSx_CPintiCore::sleep(uinteger milliseconds)
{
    cpc_cpinti_sleep_ms(milliseconds);
}
void CpcdosOSx_CPintiCore::ssleep(uinteger seconds)
{
    cpc_cpinti_sleep_sec(seconds);
}

double CpcdosOSx_CPintiCore::get_time_ms(double Temps_av)
{
    return cpc_cpinti_Obtenir_Temps_ms(Temps_av);
}

#endif
#endif /* CPCDOS_CPC_WPR */
