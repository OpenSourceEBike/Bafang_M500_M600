/** @file
 *
 *  secure echo server for lwip
 */
/*
 * Copyright (c) 2003 NBS Card Technology, Paramus, NJ.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: David Haas <dhaas@alum.rpi.edu>
 *
 * Purpose:   server for testing and demonstration purposes
 *
 * This file implements a nice example of handling multiple tcp sockets in a
 * server environment. Just call secure_socket_init() from your application after
 * you have initialized lwip and added your network interfaces. Change the
 * MAX_SERV option to increase or decrease the number of sessions supported.
 *
 * This is also a good example of how to support multiple sessions in an
 * embedded system where you might not have fork().
 */

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/mem.h"

#if LWIP_SOCKET

#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif /* LWIP_DHCP */
#if LWIP_AUTOIP || LWIP_DHCP_AUTOIP_COOP
#include "lwip/autoip.h"
#endif /* LWIP_AUTOIP || LWIP_DHCP_AUTOIP_COOP */

#if LWIP_DHCP
#if LWIP_DHCP_AUTOIP_COOP
#define HAVE_IP(n)      (dhcp_supplied_address(n) || autoip_supplied_address(n))
#else
#define HAVE_IP(n)      dhcp_supplied_address(n)
#endif /* LWIP_DHCP_AUTOIP_COOP */
#elif LWIP_AUTOIP
#define HAVE_IP(n)      autoip_supplied_address(n)
#else
#define HAVE_IP(n)      1
#endif /* LWIP_DHCP */

#include "ssl.h"

#include <string.h>

#include "secure_socket.h"
#include "osif.h"

#ifdef CERT_1024

#define USE_CERT_BUFFERS_1024
#include <certs_test.h>

#define CA_CERT				ca_cert_der_1024
#define CA_CERT_SIZE		sizeof_ca_cert_der_1024
#define SERVER_CERT			server_cert_der_1024
#define SERVER_CERT_SIZE	sizeof_server_cert_der_1024
#define CLIENT_CERT			client_cert_der_1024
#define CLIENT_CERT_SIZE	sizeof_client_cert_der_1024
#define SERVER_KEY			server_key_der_1024
#define SERVER_KEY_SIZE		sizeof_server_key_der_1024

#else /* CERT_1024 */

#define USE_CERT_BUFFERS_2048
#include <certs_test.h>

#define CA_CERT				ca_cert_der_2048
#define CA_CERT_SIZE		sizeof_ca_cert_der_2048
#define SERVER_CERT			server_cert_der_2048
#define SERVER_CERT_SIZE	sizeof_server_cert_der_2048
#define CLIENT_CERT			client_cert_der_2048
#define CLIENT_CERT_SIZE	sizeof_client_cert_der_2048
#define SERVER_KEY			server_key_der_2048
#define SERVER_KEY_SIZE		sizeof_server_key_der_2048

#endif /* CERT_1024 */

#define MAX_SERV                 5         /* Maximum number of services. Don't need too many */

#define socket_server_thread_NAME      "tls_sock"
#define socket_PRIORITY         DEFAULT_THREAD_PRIO
#define socket_server_thread_STACKSIZE configMINIMAL_STACK_SIZE

#include <wolfssl/wolfcrypt/aes.h>

/* how many kB to test (en/de)cryption */
#define NUM_BLOCKS 4096
#define BLOCK_SIZE 1024
#define SPEED(start, end)   (int)(((end) - (start)) > 0.0 ? ((double)NUM_BLOCKS / ((end) - (start))) : 0)

static XGEN_ALIGN byte bench_key[] =
{
    0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,
    0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,
    0x89,0xab,0xcd,0xef,0x01,0x23,0x45,0x67,
    0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef
};
static XGEN_ALIGN byte bench_iv[] =
{
    0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef,
    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
    0x11,0x21,0x31,0x41,0x51,0x61,0x71,0x81
};

Aes*    enc = NULL;
byte* bench_plain = NULL;
byte* bench_cipher = NULL;

double current_time(void);
char* bench_aescbc(void);


char* benchmark_crypto(void)
{
    char* result = NULL;
    result = bench_aescbc();
    return result;
}

char* bench_aescbc()
{
    static char result[32] = "Enc:      KB/s Dec:      KB/s\n";
    char *p;
    double start_time, done_time;
    int i = 0;
    int ret = 0;

    memset(bench_plain, 0, BLOCK_SIZE);
    memset(bench_cipher, 0, BLOCK_SIZE);
    memset(enc, 0, sizeof(*enc));

    /* init keys */
    ret = wc_AesInit(enc, NULL, INVALID_DEVID);
    LWIP_ASSERT("ws_AesInit() failed", ret == 0);
    ret = wc_AesSetKey(enc, (byte*)bench_key, 16, (byte*)bench_iv, AES_ENCRYPTION);
    LWIP_ASSERT("ws_AesSetKey() failed", ret == 0);

    start_time = current_time();
    for (i = 0; i < NUM_BLOCKS; i++) {
        ret = wc_AesCbcEncrypt(enc, bench_plain, bench_cipher, BLOCK_SIZE);
        LWIP_ASSERT("ws_AesCbcEncrypt() failed", ret == 0);
    }
    done_time = current_time();
    p = &result[4];
    lwip_itoa(p, 5, SPEED(start_time, done_time));
    while (*p)
      p++;
    *p = ' ';

    /* init keys */
    ret = wc_AesSetKey(enc, (byte*)bench_key, 16, (byte*)bench_iv, AES_DECRYPTION);
    LWIP_ASSERT("ws_AesSetKey() failed", ret == 0);
    start_time = current_time();
    for (i = 0; i < NUM_BLOCKS; i++) {
        ret = wc_AesCbcDecrypt(enc, bench_plain, bench_cipher, BLOCK_SIZE);
        LWIP_ASSERT("ws_AesCbcDecrypt() failed", ret == 0);
    }
    done_time = current_time();
    p = &result[19];
    lwip_itoa(p, 5, SPEED(start_time, done_time));
    while (*p)
      p++;
    *p = ' ';

    wc_AesFree(enc);

    return result;
}


double current_time(void)
{
    uint32_t msecs = OSIF_GetMilliseconds();
    return (double)msecs / (double)1000;
}

#define PORT   11111

char buffer[80];

struct clientcb {
  struct clientcb *next;
  int socket;
  WOLFSSL* ssl;
  struct sockaddr_storage cliaddr;
  socklen_t clilen;
};

static struct clientcb *clientcb_list = 0;

/**************************************************************
 * void close_socket(struct clientcb *p_clientcb)
 *
 * Close the socket and remove this clientcb from the list.
 **************************************************************/
static void
close_socket(struct clientcb *p_clientcb)
{
  struct clientcb *p_search_clientcb;

  /* Either an error or tcp connection closed on other
   * end. Close here */
  wolfSSL_free(p_clientcb->ssl);
  close(p_clientcb->socket);
  /* Free clientcb */
  if (clientcb_list == p_clientcb) {
    clientcb_list = p_clientcb->next;
  } else {
    for (p_search_clientcb = clientcb_list; p_search_clientcb; p_search_clientcb = p_search_clientcb->next) {
      if (p_search_clientcb->next == p_clientcb) {
        p_search_clientcb->next = p_clientcb->next;
        break;
      }
    }
  }
  mem_free(p_clientcb);
}

/* This is a helper function that blocks until the IP configuration is complete */
static inline void wait_for_ip(void)
{
#if LWIP_IPV4
  while (!(HAVE_IP(netif_default)))
  {
    /* wait for dhcp / auto initialization to finish before using IP */
    sys_msleep(100);
  }
#endif /* LWIP_IPV4 */
}

/**************************************************************
 * void sock_server_thread(void *arg)
 *
 * secure socket task. This server will wait for connections on
 * TCP port number: PORT. For every connection, the server will
 *echo back any message received.
 **************************************************************/
static void
socket_server_thread(void *arg)
{
  int listenfd;
#if LWIP_IPV6
  struct sockaddr_in6 socket_saddr;
#else /* LWIP_IPV6 */
  struct sockaddr_in socket_saddr;
#endif /* LWIP_IPV6 */
  fd_set readset;
  int i, maxfdp1;
  int ret;
  WOLFSSL_CTX* ctx;
  struct clientcb *p_clientcb;
  LWIP_UNUSED_ARG(arg);

  wait_for_ip();

  /* Initialize WOLFSSL */
  ret = wolfSSL_Init();
  LWIP_ASSERT("wolfSSL_Init() failed", ret == SSL_SUCCESS);

  memset(&socket_saddr, 0, sizeof (socket_saddr));
#if LWIP_IPV6
  /* First acquire our socket for listening for connections */
  listenfd = socket(AF_INET6, SOCK_STREAM, 0);
  socket_saddr.sin6_family = AF_INET6;
  socket_saddr.sin6_addr = in6addr_any;
  socket_saddr.sin6_port = lwip_htons(PORT); /* echo server port */
#else /* LWIP_IPV6 */
  /* First acquire our socket for listening for connections */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  socket_saddr.sin_family = AF_INET;
  socket_saddr.sin_addr.s_addr = PP_HTONL(INADDR_ANY);
  socket_saddr.sin_port = lwip_htons(PORT); /* echo server port */
#endif /* LWIP_IPV6 */

  LWIP_ASSERT("socket_server_thread(): Socket create failed.", listenfd >= 0);

  /* Create and initialize CTX */
  ctx = wolfSSL_CTX_new(wolfSSLv23_server_method());
  LWIP_ASSERT("wolfSSL_CTX_new() failed", ctx != NULL);

  /* Limit to AES128 - hardware-accelerated */
  wolfSSL_CTX_set_cipher_list(ctx, "AES128-SHA");

  /* Load CA certificates */
  ret = wolfSSL_CTX_load_verify_buffer(ctx, CA_CERT, CA_CERT_SIZE, SSL_FILETYPE_ASN1);
  LWIP_ASSERT("wolfSSL_CTX_load_verify_locations() failed", ret == SSL_SUCCESS);

  /* Load server certificate */
  ret = wolfSSL_CTX_use_certificate_buffer(ctx, SERVER_CERT, SERVER_CERT_SIZE, SSL_FILETYPE_ASN1);
  LWIP_ASSERT("wolfSSL_CTX_use_certificate_buffer() failed", ret == SSL_SUCCESS);

  /* Load keys */
  ret = wolfSSL_CTX_use_PrivateKey_buffer(ctx, SERVER_KEY, SERVER_KEY_SIZE, SSL_FILETYPE_ASN1);
  LWIP_ASSERT("wolfSSL_CTX_use_PrivateKey_buffer() failed", ret == SSL_SUCCESS);

  if (bind(listenfd, (struct sockaddr *) &socket_saddr, sizeof(socket_saddr)) == -1) {
    LWIP_ASSERT("socket_server_thread(): Socket bind failed.", 0);
  }

  /* Put socket into listening mode */
  if (listen(listenfd, MAX_SERV) == -1) {
    LWIP_ASSERT("socket_server_thread(): Listen failed.", 0);
  }

  enc = (Aes*)mem_malloc(sizeof(Aes));
  bench_plain = (byte*)mem_malloc(BLOCK_SIZE);
  bench_cipher = (byte*)mem_malloc(BLOCK_SIZE);

  /* Wait forever for network input: This could be connections or data */
  for (;;) {
    maxfdp1 = listenfd + 1;

    /* Determine what sockets need to be in readset */
    FD_ZERO(&readset);
    FD_SET(listenfd, &readset);
    for (p_clientcb = clientcb_list; p_clientcb; p_clientcb = p_clientcb->next) {
      if (maxfdp1 < p_clientcb->socket + 1) {
        maxfdp1 = p_clientcb->socket + 1;
      }
      FD_SET(p_clientcb->socket, &readset);
    }

    /* Wait for data or a new connection */
    i = select(maxfdp1, &readset, 0, 0, 0);

    if (i == 0) {
      continue;
    }
    /* At least one descriptor is ready */
    if (FD_ISSET(listenfd, &readset)) {
      /* We have a new connection request!!! */
      /* Lets create a new control block */
      p_clientcb = (struct clientcb *) mem_malloc(sizeof (struct clientcb));
      if (p_clientcb) {
        p_clientcb->socket = accept(listenfd,
                (struct sockaddr *) &p_clientcb->cliaddr,
                &p_clientcb->clilen);
        if (p_clientcb->socket < 0) {
          mem_free(p_clientcb);
        } else {
          /* Keep this tecb in our list */
          p_clientcb->ssl = wolfSSL_new(ctx);
          LWIP_ASSERT("wolfSSL_new() failed.", p_clientcb->ssl != NULL);
          wolfSSL_set_fd(p_clientcb->ssl, p_clientcb->socket);
          p_clientcb->next = clientcb_list;
          clientcb_list = p_clientcb;
        }
      } else {
        /* No memory to accept connection. Just accept and then close */
        int sock;
        struct sockaddr cliaddr;
        socklen_t clilen;

        sock = accept(listenfd, &cliaddr, &clilen);
        if (sock >= 0) {
          close(sock);
        }
      }
    }
    /* Go through list of connected clients and process data */
    for (p_clientcb = clientcb_list; p_clientcb; p_clientcb = p_clientcb->next) {
      if (FD_ISSET(p_clientcb->socket, &readset)) {
        /* This socket is ready for reading. This could be because someone typed
         * some characters or it could be because the socket is now closed. Try reading
         * some data to see. */
        int readcount;
        readcount = wolfSSL_read(p_clientcb->ssl, &buffer, sizeof(buffer) - 1);
        if (readcount <= 0) {
            close_socket(p_clientcb);
            break;
        }
        buffer[readcount] = 0;
        if (!strncmp(buffer, "benchmark", 9)) {
            strncpy(buffer, benchmark_crypto(), 40);
        }
        if (wolfSSL_write(p_clientcb->ssl, buffer, strlen(buffer)) < 0) {
          close_socket(p_clientcb);
          break;
        }
      }
    }
  }
  mem_free(bench_plain);
  mem_free(bench_cipher);
  mem_free(enc);

  wolfSSL_CTX_free(ctx);
  wolfSSL_Cleanup();
  close(listenfd);
}

/**************************************************************
 * void socket_init(void)
 *
 * This function initializes the secure socket service.
 **************************************************************/
void
secure_socket_init(void)
{
  sys_thread_t thread;
  thread = sys_thread_new(socket_server_thread_NAME, socket_server_thread, 0, 6 * 1024, socket_PRIORITY);
  LWIP_ASSERT("secure_socket_init() failed", thread != NULL);
}

#endif /* LWIP_SOCKET */
