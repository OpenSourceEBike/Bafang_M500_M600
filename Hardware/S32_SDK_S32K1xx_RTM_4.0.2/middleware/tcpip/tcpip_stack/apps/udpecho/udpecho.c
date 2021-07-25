/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "lwip/igmp.h"
#include "lwip/opt.h"
#include "udpecho.h"
#include <string.h>

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"

/*-----------------------------------------------------------------------------------*/
static char buffer[4096];

static void
udpecho_thread(void *arg)
{
  struct netconn *conn;
  struct netconn *conn_igmp;
  struct netbuf *buf;
  err_t err;
  LWIP_UNUSED_ARG(arg);
#if LWIP_IPV6
  conn = netconn_new(NETCONN_UDP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, 7);
#if LWIP_IGMP
  /*create a udp IPv4 conn to send igmp join group report*/
  conn_igmp = netconn_new(NETCONN_UDP);
#endif /* LWIP_IGMP */
#else /* LWIP_IPV6 */
  conn = netconn_new(NETCONN_UDP);
  netconn_bind(conn, IP_ADDR_ANY, 7);
  conn_igmp = conn;
#endif /* LWIP_IPV6 */
  LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);

  while (1) {
    /*Polling for msg from connection*/
    err = netconn_recv(conn, &buf);
    if (err == ERR_OK) {
      if(netbuf_copy(buf, buffer, sizeof(buffer)) != buf->p->tot_len) {
        LWIP_DEBUGF(LWIP_DBG_ON, ("netbuf_copy failed\n"));
      }
      /*  no need netconn_connect here, since the netbuf contains the address */
      else
      {
#if LWIP_IPV6_MLD
        if (buf->p->tot_len == 24 && !strncmp(&buffer[0], "MLD6", 4))
        {
        /*Already set the IP address of the group using IP address contain in the message*/
          ip_addr_t gaddr;
          IP_ADDR6(&gaddr, *((u32_t*)(&buffer[8])), *((u32_t*)(&buffer[12])), *((u32_t*)(&buffer[16])), *((u32_t*)(&buffer[20])));

        if (!strncmp(&buffer[4], "JOIN", 4)) /*if message is join*/
          {
          err = netconn_join_leave_group(conn, &gaddr, IP_ADDR_ANY, NETCONN_JOIN);  /*called join group function with group include in the message */
            if(err != ERR_OK)
            {
              LWIP_DEBUGF(LWIP_DBG_ON, ("function netconn_join_leave_group failed %d\n", (int)err));
            }
          }
        if (!strncmp(&buffer[4], "LEAV", 4)) /*if message is leav*/
          {
          err = netconn_join_leave_group(conn, &gaddr,IP_ADDR_ANY, NETCONN_LEAVE);   /*called leave, group function with group include in the message */
            if(err != ERR_OK)
            {
              LWIP_DEBUGF(LWIP_DBG_ON, ("function netconn_join_leave_group failed %d\n", (int)err));
            }
          }
        }
#endif /*LWIP_IPV6_MLD*/
#if LWIP_IGMP
        if (buf->p->tot_len == 12 && !strncmp(&buffer[0], "IGMP", 4))
        {
        /*Already set the IP address of the group using IP address contain in the message*/
          ip_addr_t gaddr;
          IP_ADDR4(&gaddr, buffer[8], buffer[9], buffer[10], buffer[11]);
        if (!strncmp(&buffer[4], "JOIN", 4)) /*if message is join*/
          {
          err = netconn_join_leave_group(conn_igmp, &gaddr, IP_ADDR_ANY, NETCONN_JOIN);  /*called join group function with group include in the message */
            if(err != ERR_OK)
            {
              LWIP_DEBUGF(LWIP_DBG_ON, ("function netconn_join_leave_group failed %d\n", (int)err));
            }
          }
        if (!strncmp(&buffer[4], "LEAV", 4)) /*if message is leav*/
          {
          err = netconn_join_leave_group(conn_igmp, &gaddr, IP_ADDR_ANY, NETCONN_LEAVE);  /*called join group function with group include in the message */
            if(err != ERR_OK)
            {
              LWIP_DEBUGF(LWIP_DBG_ON, ("function netconn_join_leave_group failed %d\n", (int)err));
            }
          }
        }
#endif /*LWIP_IGMP*/
        buffer[buf->p->tot_len] = '\0';
        err = netconn_send(conn, buf);
        if(err != ERR_OK)
        {
         LWIP_DEBUGF(LWIP_DBG_ON, ("netconn_send failed: %d\n", (int)err));
       }
       else
       {
         LWIP_DEBUGF(LWIP_DBG_ON, ("got %s\n", buffer));
       }
     }
     netbuf_delete(buf);
   }
 }
}
/*-----------------------------------------------------------------------------------*/
void
udpecho_init(struct netif *netif)
{
  LWIP_UNUSED_ARG(netif);
  sys_thread_new("udpecho_thread", udpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}
#endif /* LWIP_NETCONN */
