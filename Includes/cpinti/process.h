#ifndef CPCDOS_PROCESS
#define CPCDOS_PROCESS

#include <cpinti/types.h>

namespace cpinti::gestionnaire_tache
{
    uinteger cpinti_creer_processus(uinteger ID_KERNEL, uinteger ID_OS, uinteger ID_USER,
                                    uinteger PID_Parent, const char *NomProcessus);

    bool cpinti_arreter_processus(uinteger ID_KERNEL, uinteger PID);

    void cpinti_set_etat_processus(uinteger ID_KERNEL, uinteger PID, uinteger ACTION);

    uinteger cpinti_get_etat_processus(uinteger ID_KERNEL, uinteger PID);

    const char *cpinti_get_nom_processus(uinteger PID);

    uinteger cpinti_get_nombre_processus();

} // namespace cpinti::gestionnaire_tache

#endif /* CPCDOS_PROCESS */
