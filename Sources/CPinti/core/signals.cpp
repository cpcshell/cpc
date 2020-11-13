#include <cstdio>
#include <signal.h>
#include <unistd.h>

#include <cpcdos/cpcdos.h>
#include <cpinti/debug.h>
#include <cpinti/signals.h>

#include "core.h"

namespace cpinti::signals
{
    static struct sigaction _sigactions[12]{};
    static volatile bool _nested_exception{};

    void signal_handler(int signalnum)
    {
        panic(signalnum, 0, nullptr, nullptr);
    }

    void init()
    {
        debug::info("Initializing signals handlers...");

        _sigactions[0].sa_handler = signal_handler;
        sigemptyset(&_sigactions[0].sa_mask);
        _sigactions[0].sa_flags = 0;
        sigaction(SIGSEGV, &_sigactions[0], 0);

        _sigactions[1].sa_handler = signal_handler;
        sigemptyset(&_sigactions[1].sa_mask);
        _sigactions[1].sa_flags = 0;
        sigaction(SIGILL, &_sigactions[1], 0);

        _sigactions[2].sa_handler = signal_handler;
        sigemptyset(&_sigactions[2].sa_mask);
        _sigactions[2].sa_flags = 0;
        sigaction(SIGABRT, &_sigactions[2], 0);

        _sigactions[3].sa_handler = signal_handler;
        sigemptyset(&_sigactions[3].sa_mask);
        _sigactions[3].sa_flags = 0;
        sigaction(SIGTRAP, &_sigactions[3], 0);

        _sigactions[4].sa_handler = signal_handler;
        sigemptyset(&_sigactions[4].sa_mask);
        _sigactions[4].sa_flags = 0;
        sigaction(SIGINT, &_sigactions[4], 0);
    }

    static void logo()
    {
        printf("\n");
        printf("-------------------------------------------------------------------------------\n");
        printf("               ######## ########  ########   #######  ########                 \n");
        printf("               ##       ##     ## ##     ## ##     ## ##     ##                \n");
        printf("               ######   ########  ########  ##     ## ########                 \n");
        printf("               ##       ##   ##   ##   ##   ##     ## ##   ##                  \n");
        printf("               ######## ##     ## ##     ##  #######  ##     ##                \n");
        printf("-------------------------------------------------------------------------------\n");
        printf("\n");
    }

    static void footer()
    {
        printf(" French forum    : http://forum-cpcdos.fr.nf\n");
        printf(" Discord support : https://discord.com/invite/3Qm8xDp\n");
        printf("\n");
        printf("-- 'R' Restart computer  |  'S' Stop kernel (Attempt to return to DOS) --\n");
    }

    static void press_any_key()
    {
        printf("Press any key to continue...");
        getchar();
    }

    void panic(int signalnum, int ligne, char *fichier, char *fonction)
    {
        if (_nested_exception)
        {
            debug::fatal("Nested exception!");
        }
        else
        {
            _nested_exception = true;
        }

        auto critical_section = gestionnaire_tache::state_SectionCritique();

        gestionnaire_tache::begin_SectionCritique();

        cpc_SCREEN(0);
        cpc_COLOR(0, 12);

        logo();

        if (!ligne)
        {
            printf(" * Internal runtime error not identified.\n");
        }
        else
        {
            printf(" * Internal runtime error identified !\n");
            cpc_COLOR(1, 12);
            printf("   --> In function %s() [%s:%d]\n", fonction, fichier, ligne);
            cpc_COLOR(0, 12);
        }

        printf(" * Current Thread ID : %lu\n", cpinti::gestionnaire_tache::get_ThreadEnCours());
        printf(" * Total Thread(s)   : %lu\n", cpinti::gestionnaire_tache::get_NombreThreads());

        if (critical_section)
        {
            printf(" * Critical section  : ENABLED\n");
        }
        else
        {
            printf(" * Critical section  : DISABLED\n");
        }

        printf(" * Error number      : %d\n", signalnum);
        printf(" * Informations      : ");

        if (signalnum == SIGABRT)
        {
            printf("Arrêt du programme. (SIGABRT)\n");
        }

        if (signalnum == SIGILL)
        {
            printf("Instruction illegale. (SIGILL)\n");
        }

        if (signalnum == SIGINT)
        {
            printf("Interruption utilisateur. (SIGINT)\n");
        }

        if (signalnum == SIGSEGV)
        {
            printf("Violation segmentation de memoire. (SIGSEGV)\n");
        }

        if (signalnum == SIGALRM)
        {
            printf("Alarme. (SIGALRM)\n");
        }

        cpc_COLOR(7, 0);
        footer();
        press_any_key();

        debug::fatal("Good bye...");
    }

} // namespace cpinti::signals