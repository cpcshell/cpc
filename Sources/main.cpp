#include <cstdio>

#include "CPC_WPR.h"

#include <cpinti/signals.h>
#include <cpinti/version.h>

extern "C" long __CPCDOS_INIT_1(long);

void intro()
{
    printf("                                                  \n");
    printf("    ======================                        \n");
    printf("    =      --   --   --  =    --    -     --      \n");
    printf("    =     |    |__| |    =   |  ) (   )   \\       \n");
    printf("    =      --  |     --  =    --    -    --       \n");
    printf("    ===========================================   \n");
    printf("         --   --   --    =  --    -     --    =   \n");
    printf("        |    |__| |      = |  ) (   )   \\     =   \n");
    printf("         --  |     --    =  --    -    --     =   \n");
    printf("                         ======================   \n");
    printf("    CPCDOS OSx                                    \n");
    printf("                       Crée Pour Concevoir Des OS \n");
    printf("                           Created for develop OS \n");
    printf(" Version :                                        \n");
    printf("  - Date    : " BUILD_DATE "\n");
    printf("  - Version : " BUILD_VERSION "\n");
}

int main(int argc, char const *argv[])
{
    intro();

    cpinti::signals::init();

    cpc_CPP_Ini_Command();

    for (int i = 1; i < argc; i++)
    {
        cpc_CCP_Exec_Commande(argv[i], 5);
    }

    return 0;
}
