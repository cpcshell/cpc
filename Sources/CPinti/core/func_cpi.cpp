#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

#include "core.h"
#include "func_cpi.h"

extern "C" int fseeko(FILE *_stream, off_t _offset, int _mode)
{
    return fseek(_stream, _offset, _mode);
}

extern "C" long ftello(FILE *stream)
{
    long pos;
    pos = ftell(stream);
    return (long)pos;
}

namespace cpinti
{
    namespace Func_Cpinti
    {

        unsigned long _w32_intel(unsigned long val)
        {
            // Cette methode permet de convertir en BIGENDIAN 32 bits
            return ((val & 0x000000FFU) << 24) | // Deplacer le bit faible a gauche
                   ((val & 0x0000FF00U) << 8) |
                   ((val & 0x00FF0000U) >> 8) |
                   ((val & 0xFF000000U) >> 24); // Deplacer le bit fort a droite
        }

        unsigned short _w32_intel16(unsigned short val)
        {
            // Cette methode permet de convertir en BIGENDIAN 16 bits
            return ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
        }

        std::string to_string(int nombre)
        {
            // Cette methode permet de convertir du int --> string

            std::stringstream convertir;
            convertir << nombre;
            return convertir.str();
        }

        std::string to_string(long nombre)
        {
            // Cette methode permet de convertir du long --> string

            std::stringstream convertir;
            convertir << nombre;
            return convertir.str();
        }

        std::string to_string(double nombre)
        {
            // Cette methode permet de convertir du double --> string

            std::stringstream convertir;
            convertir << nombre;
            return convertir.str();
        }

        std::string to_string(uinteger nombre)
        {
            // Cette methode permet de convertir du double --> string

            std::stringstream convertir;
            convertir << nombre;
            return convertir.str();
        }

        std::string to_string(unsigned short nombre)
        {
            // Cette methode permet de convertir du double --> string

            std::stringstream convertir;
            convertir << nombre;
            return convertir.str();
        }

        int to_int(std::string nombre)
        {
            // Cette methode permet de convertir du String --> int
            int Resultat;
            std::istringstream(nombre) >> Resultat;
            return Resultat;
        }

        uinteger to_uint(std::string nombre)
        {
            // Cette methode permet de convertir du String --> uinteger
            uinteger Resultat;
            std::istringstream(nombre) >> Resultat;
            return Resultat;
        }

        long to_long(std::string nombre)
        {
            // Cette methode permet de convertir du String --> long
            long Resultat;
            std::istringstream(nombre) >> Resultat;
            return Resultat;
        }

        std::string to_str_hex(int nombre)
        {
            // Cette methode permet de convertir du string --> hexa

            std::stringstream sstream;
            sstream << std::hex << nombre;
            return sstream.str();
        }

        std::string to_str_hex(uinteger nombre)
        {
            // Cette methode permet de convertir du string --> hexa

            std::stringstream sstream;
            sstream << std::hex << nombre;
            return sstream.str();
        }

        unsigned short Generer_CheckSum(void *TRAME, size_t Taille)
        {
            unsigned short *Buffer = (unsigned short *)TRAME;
            uinteger SUM = 0;
            unsigned short Resultat;

            for (SUM = 0; Taille > 1; Taille -= 2)
                SUM += *Buffer++;

            if (Taille == 1)
                SUM += *(unsigned char *)Buffer;

            SUM = (SUM >> 16) + (SUM & 0xFFFF);
            SUM += (SUM >> 16);
            Resultat = ~SUM;
            return Resultat;
        }

        long Comparer_Temps(struct timeval *Temps_DEPART, struct timeval *Temps_ACTUEL)
        {
            long Resultat = (long int)(((Temps_DEPART->tv_sec * 1000000) + (uinteger)Temps_DEPART->tv_usec) - ((Temps_ACTUEL->tv_sec * 1000000) + (uinteger)Temps_ACTUEL->tv_usec)) / 100000000;
            return Resultat;
        }
    } // namespace Func_Cpinti

    //***********************//

    void cpinti_Sleep(uinteger secondes)
    {
        sleep(secondes * 1000 ^ 2); // Hook par Cpcdos
    }

    void cpinti_USleep(uinteger MicroSecondes)
    {
        usleep(MicroSecondes); // Hook par Cpcdos
    }

} // namespace cpinti
