#ifndef CPCDOS_CLIENT
#define CPCDOS_CLIENT

namespace cpinti
{

#define CLIENT_OK 0
#define CLIENT_ERR_ACCEPTER -1
#define CLIENT_ERR_INIT_SOCK -2
#define CLIENT_ERR_CONFIG_SOCK -3
#define CLIENT_ERR_BIND_SOCK -4
#define CLIENT_ERR_ECOUTE_SOCK -5
#define CLIENT_ERR_FD -6
#define CLIENT_ERR_PIPE_SOCK -7
#define CLIENT_ERR_NOM_DNS -8
#define CLIENT_ERR_MEM -9
#define CLIENT_ERR_INTERNE -10
#define CLIENT_SERVEUR_FERME -11
#define CLIENT_ERR_CONNECTION -12

#define O_NONBLOCK 0x0004
#define TCP_NODELAY 0x01

#define ENOMSG 42 // Pas de message

#define _DEFFERE_FERME 24 // Taille du tableau de sock pour la fermeture differes des sockets

#define TailleBuffer 16384

	//  0	: OK
	// 	-1 	: La machine n'a pas repondu
	//	-2	: Impossible de creer un socket (Driver manquant?)
	//	-3	: Erreur de configuration du socket
	//	-4	: Erreur de binding
	//	-5	: Ecoute impossible
	//	-6	: Erreur de descripteur de fichier (select())
	//	-7	: Erreur de lecture de socket (ERRPIPE)
	//  -8	: Impossible de resoudre le nom (DNS)
	//	-9	: Memoire insuffisante

	namespace net_client
	{
		std::string Resolution_DNS(std::string NomAdresse);
		int Taille_Contenu(int socket);
		void Fermer_socket(int SocketReseau);
		int Demarrer_client(std::string AdresseIP, unsigned int Port, unsigned int NumeroID, int TYPE_CLIENT);

	} // namespace net_client

} // namespace cpinti

#endif /* CPCDOS_CLIENT */
