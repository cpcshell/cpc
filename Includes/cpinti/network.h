#ifndef CPCDOS_NETWORK
#define CPCDOS_NETWORK

#include "cpinti/types.h"

namespace cpinti::net
{
	int cpinti_ping_icmp(const char *IP_machine, const char *Message);

	int cpinti_serveur(uinteger NumPort, integer NombreClients, uinteger NumeroID, integer TYPE_SERVEUR);

	int cpinti_client(const char *Adresse, uinteger NoPort, uinteger NumeroID, integer TYPE_SERVEUR);

	uinteger cpinti_get_net_envoye();

	uinteger cpinti_get_net_recu();

	uinteger cpinti_get_net_machines();

	uinteger cpinti_get_net_activite();
} // namespace cpinti::net

#endif /* CPCDOS_NETWORK */
