#ifndef CPCDOS_BUFFER
#define CPCDOS_BUFFER

#include <string>

#include "cpinti/types.h"

namespace cpinti
{
    std::string cpinti_GEST_BUFF(uinteger _ID, int _CHEMIN, std::string _DONNEES);

    int cpinti_GEST_BUFF_c(uinteger _ID, integer _CHEMIN, const char *_DONNEES);

    int cpinti_GEST_BUFF_c(uinteger _ID, integer _CHEMIN, char *_DONNEES);
} // namespace cpinti

#endif /* CPCDOS_BUFFER */
