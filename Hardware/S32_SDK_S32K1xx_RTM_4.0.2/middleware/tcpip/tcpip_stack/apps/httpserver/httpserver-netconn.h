#ifndef LWIP_HTTPSERVER_NETCONN_H
#define LWIP_HTTPSERVER_NETCONN_H

/** The server port for HTTPD to use */
#if !defined HTTPD_SERVER_PORT
#define HTTPD_SERVER_PORT                   80
#endif

void http_server_netconn_init(void);

#endif /* LWIP_HTTPSERVER_NETCONN_H */
