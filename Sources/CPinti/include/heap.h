#ifndef INCLUDE_HEAP
#define INCLUDE_HEAP

std::string NP_cpinti_socket::cpinti_socket_LOCAL::Erreur_STR = "";

#define _MAX_Stack_instance 512 /* Nombre d'instance maximum */

uinteger cpinti::SATATISTIQUES_NET_port[_MAX_Stack_instance] = {0};
uinteger cpinti::SATATISTIQUES_NET_recu[_MAX_Stack_instance] = {0};
uinteger cpinti::SATATISTIQUES_NET_envoye[_MAX_Stack_instance] = {0};
uinteger cpinti::SATATISTIQUES_NET_activite[_MAX_Stack_instance] = {0};
uinteger cpinti::SATATISTIQUES_NET_clients[_MAX_Stack_instance] = {0};

#endif /* INCLUDE_HEAP */
