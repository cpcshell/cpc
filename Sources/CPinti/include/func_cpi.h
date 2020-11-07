
#ifndef CPCDOS_FUNC_CPI
#define CPCDOS_FUNC_CPI

#include <iostream>
#include <sstream>
#include <sys/time.h>

#include "cpinti/types.h"

#ifndef doevents
#    define doevents(temps) cpinti::cpinti_doevents(temps)
#endif

extern "C" unsigned short _w32_intel(unsigned short val);
extern "C" unsigned short _w32_intel16(unsigned short val);

namespace cpinti
{

    void cpinti_doevents();
    void cpinti_doevents(unsigned long Temps);
    void cpinti_Sleep(uinteger secondes);
    void cpinti_USleep(uinteger MicroSecondes);

    namespace Func_Cpinti
    {
        unsigned long _w32_intel(unsigned long val);
        unsigned short _w32_intel16(unsigned short val);

        std::string to_string(int nombre);
        std::string to_string(uinteger nombre);
        std::string to_string(long nombre);
        std::string to_string(double nombre);
        std::string to_string(unsigned short nombre);

        std::string to_str_hex(int nombre);
        std::string to_str_hex(uinteger nombre);
        std::string to_str_hex(uinteger nombre);

        int to_int(std::string nombre);
        uinteger to_uint(std::string nombre);
        long to_long(std::string nombre);

        unsigned short Generer_CheckSum(void *TRAME, size_t Taille);
        long Comparer_Temps(struct timeval *Temps_DEPART,
                            struct timeval *Temps_ACTUEL);

        uinteger get_MemoireLibre();
    } // namespace Func_Cpinti
} // namespace cpinti
#endif /* CPCDOS_FUNC_CPI */
