/* Entete stack.cpp */

#ifndef CPCDOS_STACK
#define CPCDOS_STACK

#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#include <cpinti/types.h>

#include "cpinti.h"

#define _STACK_STOCKER_POUR_CPCDOS 1  /* Serveur --> STACK Cpcdos  */
#define _STACK_STOCKER_POUR_SERVEUR 2 /* Cpcdos  --> STACK Serveur */
#define _STACK_EXTRACT_POUR_CPCDOS 3  /* STACK Cpcdos  --> Cpcdos  */
#define _STACK_EXTRACT_POUR_SERVEUR 4 /* STACK Serveur --> Serveur */
#define _STACK_INITIALISER 9          /* Creer une nouvelle instance d'un stack */
#define _STACK_SUPPRIMER 10           /* Supprimer l'instance du stack */

namespace cpinti
{
    class cpinti_stack_inv
    {
    private:
        // Instance du vector
        std::vector<std::string> Stack_STR;

        // Messages d'erreurs
        std::string Erreur_STR_FR;
        std::string Erreur_STR_EN;

    public:
        // Servant aux modules
        uinteger tag_1;
        uinteger tag_2;
        uinteger tag_3;

        // Obtenir la derniere erreur
        std::string get_Erreur();

        // Initialiser une instance
        bool stack__init(uinteger Taille);

        /*---------------------------------------------*/

        // Supprimer tout son cotenu
        void CLEAR_stack();

        /*---------------------------------------------*/

        // Remplir la pile d'une donnee specifiee
        void set_Stack(std::string STR_a_PUSH);

        /*---------------------------------------------*/

        // Ajouter des donnes dans la stack
        bool add_Stack(std::string _DONNEES);

        /*---------------------------------------------*/

        // Obtenir la donne SOUS la pile (la plus obsolete)
        std::string get_Stack(int MODE);

        /*---------------------------------------------*/

        // Obtenir la taille occupee par la stack (nombre d'elements)
        uinteger get_Taille_occupe();

        // Obtenir la taille libre non utilise dans la stack (Nombre d'elements)
        uinteger get_Taille_libre();

        // Obtenir la taille total du stack (nombre d'elements)
        uinteger get_Taille_stack();

        /*---------------------------------------------*/

        // Consctructeur
        cpinti_stack_inv();

        // Desctructeur
        ~cpinti_stack_inv();
    };

    static std::vector<std::shared_ptr<cpinti_stack_inv>> stack_kernel;
    static std::vector<std::shared_ptr<cpinti_stack_inv>> stack_server;

} // namespace cpinti
#endif /* CPCDOS_STACK */
