#include "CPC_WPR.h"

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