#ifndef LWIP_TLS_SOCKET_H
#define LWIP_TLS_SOCKET_H

#include "lwip/opt.h"

#if LWIP_SOCKET

/* Use 1024 bits certificate */
#define CERT_1024

void secure_socket_init(void);

#endif /* LWIP_SOCKET */

#endif /* LWIP_TLS_SOCKET_H */
