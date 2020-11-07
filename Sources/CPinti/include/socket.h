/* Entete de socket.cpp */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h> // Deja inclu?
#include <netinet/ip.h>
#include <sys/socket.h>
#include <vector>

// Garder la compatilites CPintiCore avec WATTCP
#define _w32_intel cpinti::Func_Cpinti::_w32_intel
#define _w32_intel16 cpinti::Func_Cpinti::_w32_intel16

// typedef uint32_t socklen_t; // manque dans le socket.h

namespace NP_cpinti_socket
{

#define ALLOC_MAX_BUFFER_PING 1024
#define ALLOC_MAX_BUFFER_SERVEUR 16384

/*------------------------------------*/
#define ICMP_DELAI_MAX 5 // Temps d'attente MAX d'un ping
/*------------------------------------*/
#define INIT_PING_ICMP 1
#define INIT_CLIENT_TCP 2
#define INIT_SERVEUR_TCP 3
#define INIT_CLIENT_UDP 4
#define INIT_SERVEUR_UDP 5
/*------------------------------------*/
#define SOCK_OK 0
#define SOCK_ERR_DESCRIPTEUR_SELECT -1
#define SOCK_ERR_DESCRIPTEUR -2

#define ENOMSG 42 // Pas de message

#define _DEFFERE_FERME 24 // Taille du tableau de sock pour la fermeture differes des sockets

    class cpinti_socket
    {
    private:
        struct TABL_Socket
        {
            std::string ipv4_STR = "0.0.0.0";
            uinteger port = 0;
            int socket = 0;
        };

        time_t statistique_temps_depart;

        // Priorites des threads reseaux (par defaut)
        uinteger THREAD_PRIORITE_YIELD = 0;
        uinteger THREAD_PRIORITE_DESCRIPTEUR = 10;
        uinteger THREAD_PRIORITE_SLEEP = 100;

        // Activite en general
        uinteger STATISTIQUES_Activite = 0;

        // Temps necessaire pour evaluer la NON activite d'un client
        uinteger STATISTIQUES_Client_temps = 2;
        uinteger STATISTIQUES_Client_pass = 2;

        // Nombre de clients actif durant les x secondes
        uinteger STATISTIQUES_Clients = 0;
        uinteger STATISTIQUES_Clients_Final = 0;

        // Nombre de paquets envoyes par secondes
        uinteger STATISTIQUES_Envoie = 0;
        uinteger STATISTIQUES_Envoie_Final = 0;

        // Nombre de paquets receptionnes par secondes
        uinteger STATISTIQUES_Reception = 0;
        uinteger STATISTIQUES_Reception_Final = 0;

        std::vector<cpinti_socket::TABL_Socket> TAB_InfoClient;

        int Type_PROTO;       // Icmp, tcp, udp client, serveur...
        int SocketReseau;     // No du FD du socket courant
        int SocketTemporaire; //  temporaire (ex: pour les clients)
        int FD_MAX;           // Maximum de socket

        int Lecture_Client;

        int NombreClient_Connectes; // Nombre de clients actuellement connectes
        int NombreClients_MAX;      // Maximum de clients
        uinteger PORT;              // Numero de port

        int Ferm_differe_SOCK[_DEFFERE_FERME]; // Tableau de socket contenant les socket A fermer
        int Ferm_differe_COMP[_DEFFERE_FERME]; //  et son compteur de cycle

        int Sock_Famille;  // Famille d'adresse
        int Sock_Type;     // Type de socket
        int Sock_Protocol; // Protocole

        int Sock_Niveau;        // setsockopt Niveau
        int Sock_Option;        // setsockopt Les options
        int Sock_Option_TAILLE; // setsockopt Et sa taille
        void *Sock_Option_VAL;  // setsockopt La valeur de l'option

        fd_set FD_socket;       // FD du socket
        fd_set FD_TEMPORAIRE;   //  une "copie" temporaire
        fd_set FD_TEMPORAIRE_2; //  une "copie" temporaire

        struct timeval TempsMAX; // Temps de delai
        int Delai_ms;            // Delai du select

        struct hostent *Sock_hostent;         // Information IPv4 a propos de l'host
        struct sockaddr_in Sock_sockaddr;     // Structure du sock INTERNET
        struct sockaddr_in Sock_sockaddr_REP; // Structure du sock INTERNET distant

        std::string Erreur_STR = ""; // Message erreur (perror())

        const char *AdresseIP_dest;     // Adresse IP destinataire en char*
        const char *AdresseIP_src;      // Adresse IP source en char*
        const char *AdresseIP_NOM_dest; // Nom domaine destinataire en char*
        const char *AdresseIP_NOM_src;  // Nom domaine source en char*

        std::string AdresseIP_dest_STR;     // Adresse IP destinataire en string (facilite le debug)
        std::string AdresseIP_src_STR;      // Adresse IP source en string (facilite le debug)
        std::string AdresseIP_NOM_dest_STR; // Nom domaine destinataire en string (facilite le debug)
        std::string AdresseIP_NOM_src_STR;  // Nom domaine source en string (facilite le debug)

        std::string BUFFER_STR; // Buffer principal (auto-bufferise)

    public:
        cpinti_socket();  // Constructeur
        ~cpinti_socket(); // Destructeur

        void THREAD_EVALUATION();

        void set_THREAD_PRIORITE_YIELD(uinteger valeur);
        void set_THREAD_PRIORITE_DESCRIPTEUR(uinteger valeur);
        void set_THREAD_PRIORITE_SLEEP(uinteger valeur);

        uinteger get_THREAD_PRIORITE_YIELD();
        uinteger get_THREAD_PRIORITE_DESCRIPTEUR();
        uinteger get_HREAD_PRIORITE_SLEEP();

        uinteger get_STATISTIQUES_Activite();  // Activite en general
        uinteger get_STATISTIQUES_Clients();   // Nombre de clients actif durant les 2 secondes
        uinteger get_STATISTIQUES_Envoie();    // Nombre de paquets envoyes par secondes
        uinteger get_STATISTIQUES_Reception(); // Nombre de paquets receptionnes par secondes

        std::string get_Erreur(); // Obtenir le message d'erreur

        std::string Resolution_DNS(const std::string Adresse); // Resoudre le nom du serveur

        std::string get_IP_dest();  // Obtenir l'IP destinataire
        std::string get_IP_src();   // Obtenir l'IP source
        std::string get_NOM_dest(); // Obtenir le nom du destinataire
        std::string get_NOM_src();  // Obtenir le nom source

        int get_Nombre_client_MAX(); // Obtenir le nombre de client MAX
        int get_Nombre_client();     // Obtenir le nombre de client actuel
        uinteger get_Port();         // Obtenir le numero de port

        std::string get_BUFFER(); // Obtenir le contenu du buffer

        int get_FD_MAX();     // Obtenir le nombre maximum de FD
        int get_FD_SETSIZE(); // Obtenir le FD_SETSIZE

        int get_NoSocket(); // Obtenir le numero de socket courant

        std::string get_IP_Client();     // Obtenir l'adresse IP du client courant
        std::string get_PORT_Client();   // Obtenir le port du client courant
        uinteger get_PORT_Client_uint(); // Obtenir le port du client courant (en uint)

        int Verifier_descripteur(int Descripteur);     // Savoir s'il le descripteur est bon
        bool Verifier_correspondance(int Descripteur); // Savoir si le descripteur correspond

        void set_IP_dest(std::string IP);  // Definir l'IP du destinataire
        void set_IP_src(std::string IP);   // Definir l'IP de l'envoie source
        void set_NOM_dest(std::string IP); // Definir le nom du destinataire
        void set_NOM_src(std::string IP);  // Definir le nom source

        void set_Nombre_client_MAX(int NbClient); // Definir le nombre MAX de clients

        void set_Port(uinteger NumPort); // Definir le numero de port

        // Premiere demarche, initialiser le socket
        bool Initialiser_socket(int Type_Socket);

        // Ensuite le configurer selon le Type de socket choisis
        bool Configurer_socket();

        // Puis le binder
        bool Binder_socket();

        // Se connecter a un serveur (client)
        bool Connecter_serveur();

        // Ecouter le reseau (serveur)
        bool Ecouter_socket();

        // Ecouter le reseau (serveur) depuis un descripteur de fichier
        bool Ecouter_socket_FD();

        // Envoyer une trame reseau
        bool Envoyer_TRAME(uinteger Taille, const void *PAQUET);

        // Envoyer des donnees au socket courant
        int Envoyer_Donnees(std::string DONNEES_STR);

        // IDEM mais des donnees au socket PRINCIPAL ou COURANT
        int Envoyer_Donnees(std::string DONNEES_STR, bool AuServeur);

        // IDEM mais a un socket precis
        int Envoyer_Donnees(std::string DONNEES_STR, int Socket);

        // IDEM mais envoyer a tous les sockets
        int Envoyer_Donnees_TOUT_CLIENTS(std::string DONNEES_STR);

        // Reparer la reception de la trame reseau
        bool Preparer_reception_TRAME();

        // Receptionner une trame
        bool Recevoir_TRAME();

        // Receptionner les donnes d'un socket client COURANT
        int Recevoir_donnees_client();
        // Idem
        int Recevoir_donnees_client(bool AuServeur);
        // Idem
        int Recevoir_donnees_client(int Socket);

        // Ceci permet d'obtenir le descripteur "suivant" a gerer (serveur)
        int get_Descripteur();

        // Ceci permet d'attribuer le descripteur a traiter temporairement
        bool Attribuer_descr_lecture(int descript);

        // Accepter une connexion
        bool Accepter_connexion();

        // Ajouter un client (Lier son descripteur de fichier)
        bool Ajouter_Client();

        // Refuser une connexion
        bool RefuserClient();

        // Refuser une connexion
        bool RefuserClient(std::string Cause);

        // Faire deconnecter TOUS les clients 1 par 1
        bool Fermer_TOUT_Clients();

        // Fermer une connexion client courant
        bool Fermer_SocketClient();

        //  IDEM mais avec un socket precis
        bool Fermer_SocketClient(int SocketClient);

        // Terminer la connexion
        bool Fermer_socket();
        // IDEM
        bool Fermer_socket(bool AuServeur);
        // IDEM
        bool Fermer_socket(int Socket);

        // Fermer apres un laps de temps pour garantir la reception des donnees avant fermeture
        bool Fermeture_differe_UPDATE();

        // Ajouter un socket dans la liste
        bool Fermeture_differe_ADD(int Socket);
    };

    class cpinti_socket_LOCAL
    {
    private:
        static std::string Erreur_STR; // Message erreur (perror())

    public:
        static std::string get_Erreur(); // Obtenir le message d'erreur

        static bool TestAdresseIPv4(const std::string &Adresse); // Tester la forme IPv4 si respectee ou non
    };
} // namespace NP_cpinti_socket
