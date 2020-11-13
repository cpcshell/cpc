
#ifndef CPCDOS_FUNC_CPI
#define CPCDOS_FUNC_CPI

#include <iostream>
#include <sstream>
#include <sys/time.h>

#include "cpinti/types.h"

extern "C" unsigned short _w32_intel(unsigned short val);
extern "C" unsigned short _w32_intel16(unsigned short val);

namespace cpinti
{
    void cpinti_Sleep(uinteger secondes);
    void cpinti_USleep(uinteger MicroSecondes);

    namespace Func_Cpinti
    {
        unsigned long _w32_intel(unsigned long val);
        unsigned short _w32_intel16(unsigned short val);

        unsigned short Generer_CheckSum(void *TRAME, size_t Taille);
        long Comparer_Temps(struct timeval *Temps_DEPART,
                            struct timeval *Temps_ACTUEL);
    } // namespace Func_Cpinti
} // namespace cpinti
#endif /* CPCDOS_FUNC_CPI */
