#ifndef CPINTI_NETWORK_H
#define CPINTI_NETWORK_H

#include <cpinti/types.h>

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

#endif // CPINTI_NETWORK_H
