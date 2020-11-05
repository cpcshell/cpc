// Ce module sert simplement pour initialiser les variables se trouvant dans la HEAP memory

/****** H E A P ******/

std::string NP_cpinti_socket::cpinti_socket_LOCAL::Erreur_STR = "";

#define _MAX_Stack_instance 512 /* Nombre d'instance maximum */

unsigned int cpinti::SATATISTIQUES_NET_port[_MAX_Stack_instance] = {0};
unsigned int cpinti::SATATISTIQUES_NET_recu[_MAX_Stack_instance] = {0};
unsigned int cpinti::SATATISTIQUES_NET_envoye[_MAX_Stack_instance] = {0};
unsigned int cpinti::SATATISTIQUES_NET_activite[_MAX_Stack_instance] = {0};
unsigned int cpinti::SATATISTIQUES_NET_clients[_MAX_Stack_instance] = {0};

/****** H E A P ******/