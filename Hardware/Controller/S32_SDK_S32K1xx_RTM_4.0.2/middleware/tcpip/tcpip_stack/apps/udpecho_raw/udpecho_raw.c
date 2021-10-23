/*
 * Copyright (c) 2016 Stephan Linz <linz@li-pro.net>, Li-Pro.Net
 * All rights reserved.
 *
 * Based on examples provided by
 * Iwan Budi Kusnanto <ibk@labhijau.net> (https://gist.github.com/iwanbk/1399729)
 * Juri Haberland <juri@sapienti-sat.org> (https://lists.gnu.org/archive/html/lwip-users/2007-06/msg00078.html)
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
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Stephan Linz rewrote this file to get a basic echo example.
 */

/**
 * @file
 * UDP echo server example using raw API.
 *
 * Echos all bytes sent by connecting client,
 * and passively closes when client is done.
 *
 */

#include "lwip/opt.h"
#include "lwip/igmp.h"
#include "lwip/debug.h"
#include "lwip/mld6.h"
#include "lwip/stats.h"
#include "lwip/udp.h"
#include <string.h>
#include "udpecho_raw.h"

#if LWIP_UDP

static struct udp_pcb *udpecho_raw_pcb;

static void
udpecho_raw_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p,
                 const ip_addr_t *addr, u16_t port)
{
  LWIP_UNUSED_ARG(arg);
  if (p != NULL)
  {
    char *data_recv = NULL;
#if LWIP_IGMP
    data_recv = p->payload;
    if (p->tot_len == 12 && !strncmp(&data_recv[0], "IGMP", 4))   /* if received data start by IGMP */
    {
      err_t err;
      ip_addr_t gaddr;
      IP_ADDR4(&gaddr, data_recv[8], data_recv[9],data_recv[10], data_recv[11]);
      if (!strncmp(&data_recv[4], "JOIN", 4))
      {
        err = igmp_joingroup(ip_2_ip4(IP_ADDR_ANY), ip_2_ip4(&gaddr));
        if(err != ERR_OK)
        {
            LWIP_DEBUGF(LWIP_DBG_ON, ("function igmp_joingroup failed %d\n", (int)err));
        }
      }
      if (!strncmp(&data_recv[4], "LEAV", 4))
      {
        err = igmp_leavegroup(ip_2_ip4(IP_ADDR_ANY), ip_2_ip4(&gaddr));
        if(err != ERR_OK)
        {
            LWIP_DEBUGF(LWIP_DBG_ON, ("function igmp_leavegroup failed %d\n", (int)err));
        }
      }
    }
#endif /*LWIP_IGMP*/

#if LWIP_IPV6
    data_recv = p->payload;
    if (p->tot_len == 24 && !strncmp(&data_recv[0], "MLD6", 4))
    {
      err_t err;
      /* Already set the IP address of the group using IP address contain in the message */
      ip6_addr_t gaddr;
      IP6_ADDR(&gaddr, *((u32_t*)(&data_recv[8])), *((u32_t*)(&data_recv[12])), *((u32_t*)(&data_recv[16])), *((u32_t*)(&data_recv[20])));
    
      if (!strncmp(&data_recv[4], "JOIN", 4)) /*if message is join*/
      {
        err = mld6_joingroup(IP6_ADDR_ANY6, &gaddr);  /*called join group function with group include in the message */
        if(err != ERR_OK)
        {
          LWIP_DEBUGF(LWIP_DBG_ON, ("function mld6_joingroup failed %d\n", (int)err));
        }
      }
      if (!strncmp(&data_recv[4], "LEAV", 4)) /*if message is leav*/
      {
        err = mld6_leavegroup(IP6_ADDR_ANY6, &gaddr);   /*called leave, group function with group include in the message */
        if(err != ERR_OK)
        {
          LWIP_DEBUGF(LWIP_DBG_ON, ("function mld6_leavegroup failed %d\n", (int)err));
        }
      }
    }
 #endif /*LWIP_IPV6*/

    /* send received packet back to sender */
    udp_sendto(upcb, p, addr, port);
    /* free the pbuf */
    pbuf_free(p);
  }
}


void
udpecho_raw_init(void)
{
  udpecho_raw_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  if (udpecho_raw_pcb != NULL)
  {
    err_t err;

    err = udp_bind(udpecho_raw_pcb, IP_ANY_TYPE, 7);
    if (err == ERR_OK)
    {
      udp_recv(udpecho_raw_pcb, udpecho_raw_recv, NULL);
    }
    else
    {
      /* abort? output diagnostic? */
    }
  }
  else
  {
    /* abort? output diagnostic? */
  }
}

#endif /* LWIP_UDP */
