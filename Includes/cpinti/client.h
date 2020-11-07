#ifndef CPCDOS_CLIENT
#define CPCDOS_CLIENT

//  0	: OK
#define CLIENT_OK 0

// 	-1 	: La machine n'a pas repondu
#define CLIENT_ERR_ACCEPTER -1

//	-2	: Impossible de creer un socket (Driver manquant?)
#define CLIENT_ERR_INIT_SOCK -2

//	-3	: Erreur de configuration du socket
#define CLIENT_ERR_CONFIG_SOCK -3

//	-4	: Erreur de binding
#define CLIENT_ERR_BIND_SOCK -4

//	-5	: Ecoute impossible
#define CLIENT_ERR_ECOUTE_SOCK -5

//	-6	: Erreur de descripteur de fichier (select())
#define CLIENT_ERR_FD -6

//	-7	: Erreur de lecture de socket (ERRPIPE)
#define CLIENT_ERR_PIPE_SOCK -7

//  -8	: Impossible de resoudre le nom (DNS)
#define CLIENT_ERR_NOM_DNS -8

//	-9	: Memoire insuffisante
#define CLIENT_ERR_MEM -9

#define CLIENT_ERR_INTERNE -10

#define CLIENT_SERVEUR_FERME -11

#define CLIENT_ERR_CONNECTION -12

#define TCP_NODELAY 0x01

#define ENOMSG 42 // Pas de message

#define _DEFFERE_FERME 24 // Taille du tableau de sock pour la fermeture differes des sockets

#define TailleBuffer 16384
namespace cpinti
{

    namespace net_client
    {
        std::string Resolution_DNS(std::string NomAdresse);
        int Taille_Contenu(int socket);
        void Fermer_socket(int SocketReseau);
        int Demarrer_client(std::string AdresseIP, uinteger Port, uinteger NumeroID, int TYPE_CLIENT);

    } // namespace net_client

} // namespace cpinti

#endif /* CPCDOS_CLIENT */
