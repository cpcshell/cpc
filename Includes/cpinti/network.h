#ifndef CPCDOS_NETWORK
#define CPCDOS_NETWORK

#include "cpinti/types.h"

namespace cpinti::net
{
	int cpinti_ping_icmp(const char *IP_machine, const char *Message, int Param);

	int cpinti_serveur(uinteger NumPort, integer NombreClients, uinteger NumeroID, integer TYPE_SERVEUR);

	int cpinti_client(const char *Adresse, uinteger NoPort, uinteger NumeroID, int TYPE_SERVEUR);

	bool cpinti_del_net_info(uinteger NoPort);

	bool cpinti_add_net_info(uinteger NoPort);

	bool cpinti_set_net_info(uinteger NoPort, uinteger pkt_recu, uinteger pkt_envoye, uinteger activite, uinteger clients);

	uinteger cpinti_get_net_info(uinteger NoPort, uinteger _inf);

	uinteger cpinti_get_net_envoye();

	uinteger cpinti_get_net_recu();

	uinteger cpinti_get_net_machines();

	uinteger cpinti_get_net_activite();
} // namespace cpinti::net

#endif /* CPCDOS_NETWORK */
