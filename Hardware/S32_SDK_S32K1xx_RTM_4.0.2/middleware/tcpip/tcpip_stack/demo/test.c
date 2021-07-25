/*
 * Copyright (c) 2001,2002 Florian Schulze.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of the contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * test.c - This file is part of lwIP test
 *
 */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 2.1, Unreachable code.
 * Statement is intented to be unreachable. If the line is executed,
 * then there was insufficient FreeRTOS heap memory available for the idle and/or
 * timer tasks to be created.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.2, old-style function definition for function
 * declaration.
 * This is done for compatibility with all hardware platforms.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be
 * visible when an object or function with external linkage is defined.
 * These are symbols weak symbols defined in platform startup files (.s).
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.3, Symbol redeclared.
 * Declarations are guarded by preprocessor directives.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.5, object/function previously declared.
 * This requirement is fulfilled since the function is declared as external.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.1, Unpermitted operand to operator '||'
 * Variable is of essential boolean type
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type
 * and a pointer to a different object type.
 * This is needed to respect the lwip API which requires argument to be passed as void *.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed to respect the lwip API which requires argument to be passed as void *.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long,
 * cast from unsigned long to pointer and cast from unsigned int to pointer.
 * The cast is required to perform a conversion between a pointer
 * and an unsigned long define, representing an address or vice versa.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 14.3, Controlling expressions shall not be invariant
 * Constant value used in LWIP_ASSERT to easily stop the program in that point if
 * needed.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 14.4, Conditional expression should have essentially Boolean type.
 * This is required for macro constructs in form do {...} while(0).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code
 * structure and better readability.
 *
 * @section [global]
 * Violates MISRA 2012 Mandatory Rule 17.3, Symbol undeclared, assumed
 * to return int.
 * The symbol is defined in another file.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 20.9, Undefined preprocessor variable, assumed 0
 * Preprocessor variable will be defined in further implementations or not defined intentionally.
 *
 */

/* C runtime includes */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#if defined(USING_OS_FREERTOS)
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#endif /* defined(USING_OS_FREERTOS) */

#include "osif.h"

/* lwIP core includes */
#include "lwip/opt.h"

#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/api.h"
#include "lwip/arch.h"

#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"

/* lwIP netif includes */
#include "lwip/etharp.h"
#include "netif/ethernet.h"
#include "netifcfg.h"

/* include the port-dependent configuration */
#include "lwipcfg.h"

/* applications includes */

#if LWIP_HTTPD_APP
#if LWIP_HTTPD_APP_NETCONN
#include "apps/httpserver/httpserver-netconn.h"
#else
#include "lwip/apps/httpd.h"
#endif /* LWIP_HTTPD_APP_NETCONN */
#endif /* LWIP_HTTPD_APP */
#if LWIP_CHARGEN_APP
#include "apps/chargen/chargen.h"
#endif /* LWIP_CHARGEN_APP */
#if LWIP_LWIPERF_APP
#include "lwip/apps/lwiperf.h"
#endif /* LWIP_LWIPERF_APP */
#if LWIP_NETBIOS_APP
#include "lwip/apps/netbiosns.h"
#endif /* LWIP_NETBIOS_APP */
#if LWIP_SNTP_APP
#include "lwip/apps/sntp.h"
#endif /* LWIP_SNTP_APP */
#include "lwip/apps/mdns.h"
#if LWIP_NETIO_APP
#include "apps/netio/netio.h"
#endif /* LWIP_NETIO_APP */
#if LWIP_RTP_APP
#include "apps/rtp/rtp.h"
#endif /* LWIP_RTP_APP */
#if LWIP_SSL_ECHO_APP
#include "apps/secure_socket/secure_socket.h"
#endif /* LWIP_SSL_ECHO_APP */
#if LWIP_SHELL_APP
#include "apps/shell/shell.h"
#endif /* LWIP_SHELL_APP */
#if LWIP_TCPECHO_APP
#if LWIP_TCPECHO_APP_NETCONN
#include "apps/tcpecho/tcpecho.h"
#else
#include "apps/tcpecho_raw/tcpecho_raw.h"
#endif /* LWIP_TCPECHO_APP_NETCONN */
#endif /* LWIP_TCPECHO_APP */
#if LWIP_UDPECHO_APP
#if LWIP_UDPECHO_APP_NETCONN
#include "apps/udpecho/udpecho.h"
#else
#include "apps/udpecho_raw/udpecho_raw.h"
#endif /* LWIP_UDPECHO_APP_NETCONN */
#endif /* LWIP_UDPECHO_APP */
#if LWIP_SOCKET_EXAMPLES_APP
#include "apps/socket_examples/socket_examples.h"
#endif /* LWIP_SOCKET_EXAMPLES_APP */
#ifdef COVERAGE_ENABLED
#include "apps/ccov/ccov.h"
#endif

#if NO_SYS
/* ... then we need information about the timer intervals: */
#include "lwip/ip4_frag.h"
#include "lwip/igmp.h"
#endif /* NO_SYS */

#ifndef LWIP_INIT_COMPLETE_CALLBACK
#define LWIP_INIT_COMPLETE_CALLBACK 0
#endif /* LWIP_INIT_COMPLETE_CALLBACK */

/* network interfaces global variables */
struct netif network_interfaces[ETHIF_NUMBER];

/* dhcp struct for the ethernet netif */
#if LWIP_DHCP
struct dhcp netif_dhcp;
#endif /* LWIP_DHCP */

/* autoip struct for the ethernet netif */
#if LWIP_AUTOIP
struct autoip netif_autoip;
#endif /* LWIP_AUTOIP */



#if defined(USING_OS_FREERTOS)
/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);

/* Priorities at which the tasks are created. */
#define mainQUEUE_SEND_TASK_PRIORITY         (tskIDLE_PRIORITY + 1)
#endif /* defined(USING_OS_FREERTOS) */

/*-----------------------------------------------------------*/

/*
* The tasks as described in the comments at the top of this file.
*/
static void mainLoopTask(void *pvParameters);

void start_example(void);

#if LWIP_INIT_COMPLETE_CALLBACK
/*
 * Callback invoked when the TCP/IP initialization is complete.
 */
void tcpip_init_complete_callback(void);
#endif /* LWIP_INIT_COMPLETE_CALLBACK */

/*-----------------------------------------------------------*/

#if LWIP_NETIF_STATUS_CALLBACK
static void
status_callback(struct netif *state_netif)
{
  if (netif_is_up(state_netif)) {
#if LWIP_IPV4
    /* status_callback UP, local interface IP is ip4addr_ntoa(netif_ip4_addr(state_netif)) */
#else
    /* status_callback UP */
#endif
#if LWIP_MDNS_RESPONDER
    mdns_resp_netif_settings_changed(state_netif);
#endif
  } else {
    /* status_callback DOWN */
  }
}
#endif /* LWIP_NETIF_STATUS_CALLBACK */

#if LWIP_NETIF_LINK_CALLBACK
static void
link_callback(struct netif *state_netif)
{
  if (netif_is_link_up(state_netif)) {
    /* link_callback UP */
  } else {
    /* link_callback DOWN */
  }
}
#endif /* LWIP_NETIF_LINK_CALLBACK */


/* This function initializes all network interfaces
 * Implements enetif_init_Activity
 */
static void interface_init(void)
{
  for(int i = 0; i < ETHIF_NUMBER; i++)
  {
#if LWIP_IPV4
    ip4_addr_t ipaddr, netmask, gw;
#endif /* LWIP_IPV4 */
#if LWIP_DHCP || LWIP_AUTOIP
    err_t err;
#endif /* LWIP_AUTOIP || LWIP_DHCP */

#if LWIP_IPV4
    ip4_addr_set_zero(&gw);
    ip4_addr_set_zero(&ipaddr);
    ip4_addr_set_zero(&netmask);
    /* network_interfaces[i] takes the IPV4 addresses from the respective configuration */
    if ((!netif_cfg[i]->has_dhcp) && (!netif_cfg[i]->has_auto_ip))
    {
      IP4_ADDR((&gw), netif_cfg[i]->gw[0], netif_cfg[i]->gw[1], netif_cfg[i]->gw[2], netif_cfg[i]->gw[3]);
      IP4_ADDR((&ipaddr), netif_cfg[i]->ip_addr[0], netif_cfg[i]->ip_addr[1], netif_cfg[i]->ip_addr[2], netif_cfg[i]->ip_addr[3]);
      IP4_ADDR((&netmask), netif_cfg[i]->netmask[0], netif_cfg[i]->netmask[1], netif_cfg[i]->netmask[2], netif_cfg[i]->netmask[3]);
    }
#endif /* LWIP_IPV4 */

#if NO_SYS
    netif_set_default(netif_add(&network_interfaces[i], &ipaddr, &netmask, &gw, NULL, ETHIF_INIT, netif_input));
#else /* NO_SYS */
    netif_set_default(netif_add(&network_interfaces[i], &ipaddr, &netmask, &gw, NULL, ETHIF_INIT, tcpip_input));
#endif /* NO_SYS */

#if LWIP_IPV6
    if (netif_cfg[i]->has_IPv6)
    {
      netif_create_ip6_linklocal_address(&network_interfaces[i], 1);

#if PRINTF_SUPPORT
    printf("ip6 linklocal address: ");
#endif
    ip6_addr_debug_print(0xFFFFFFFFU & ~LWIP_DBG_HALT, netif_ip6_addr(&network_interfaces[i], 0));
    }
#endif /* LWIP_IPV6 */

#if LWIP_NETIF_STATUS_CALLBACK
    netif_set_status_callback(&network_interfaces[i], status_callback);
#endif /* LWIP_NETIF_STATUS_CALLBACK */

#if LWIP_NETIF_LINK_CALLBACK
    netif_set_link_callback(&network_interfaces[i], link_callback);
#endif /* LWIP_NETIF_LINK_CALLBACK */

#if LWIP_AUTOIP
    if (netif_cfg[i]->has_auto_ip)
    {
      autoip_set_struct(&network_interfaces[i], &netif_autoip);
    }
#endif /* LWIP_AUTOIP */

#if LWIP_DHCP
    if (netif_cfg[i]->has_dhcp)
    {
      dhcp_set_struct(&network_interfaces[i], &netif_dhcp);
    }
#endif /* LWIP_DHCP */
    netif_set_up(&network_interfaces[i]);
#if LWIP_DHCP
    if (netif_cfg[i]->has_dhcp)
    {
      err = dhcp_start((struct netif *)&network_interfaces[i]);
      LWIP_ASSERT("dhcp_start failed", err == (err_t)ERR_OK);
    }
#endif /* LWIP_DHCP */

#if LWIP_AUTOIP
    else if (netif_cfg[i]->has_auto_ip)
    {
      err = autoip_start(&network_interfaces[i]);
      LWIP_ASSERT("autoip_start failed", err == (err_t)ERR_OK);
    }
#endif /* LWIP_AUTOIP */
  }
}

#if LWIP_DNS_APP && LWIP_DNS
static void
dns_found(const char *name, const ip_addr_t *addr, void *arg)
{
  LWIP_UNUSED_ARG(arg);
#if PRINTF_SUPPORT
  printf("%s: %s\n", name, addr ? ipaddr_ntoa(addr) : "<not found>");
#else
  LWIP_UNUSED_ARG(name);
  LWIP_UNUSED_ARG(addr);
#endif
}

static void
dns_dorequest(void *arg)
{
  const char* dnsname = "3com.com";
  ip_addr_t dnsresp;
  LWIP_UNUSED_ARG(arg);

  if (dns_gethostbyname(dnsname, &dnsresp, dns_found, 0) == ERR_OK) {
    dns_found(dnsname, &dnsresp, 0);
  }
}
#endif /* LWIP_DNS_APP && LWIP_DNS */

#if LWIP_LWIPERF_APP
static void
lwiperf_report(void *arg, enum lwiperf_report_type report_type,
  const ip_addr_t* local_addr, u16_t local_port, const ip_addr_t* remote_addr, u16_t remote_port,
  u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec)
{
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(local_addr);
  LWIP_UNUSED_ARG(local_port);

#if PRINTF_SUPPORT
  printf("IPERF report: type=%d, remote: %s:%d, total bytes: %lu, duration in ms: %lu, kbits/s: %lu\n",
    (int)report_type, ipaddr_ntoa(remote_addr), (int)remote_port, bytes_transferred, ms_duration, bandwidth_kbitpsec);
#else
  LWIP_UNUSED_ARG(report_type);
  LWIP_UNUSED_ARG(remote_addr);
  LWIP_UNUSED_ARG(remote_port);
  LWIP_UNUSED_ARG(bytes_transferred);
  LWIP_UNUSED_ARG(ms_duration);
  LWIP_UNUSED_ARG(bandwidth_kbitpsec);
#endif
}
#endif /* LWIP_LWIPERF_APP */

#if LWIP_MDNS_RESPONDER && LWIP_HTTPD_APP
static void srv_txt(struct mdns_service *service, void *txt_userdata)
{
   err_t res = mdns_resp_add_service_txtitem(service, "path=/", 6);
   LWIP_ERROR("mdns add service txt failed\n", (res == (err_t)ERR_OK), return);
   LWIP_UNUSED_ARG(txt_userdata);
}
#endif

/* This function initializes applications
 * Implements apps_init_Activity
 */
static void
apps_init(void)
{
#if LWIP_DNS_APP && LWIP_DNS
  /* wait until the netif is up (for dhcp, autoip or ppp) */
  sys_timeout(5000, dns_dorequest, NULL);
#endif /* LWIP_DNS_APP && LWIP_DNS */

#if LWIP_CHARGEN_APP && LWIP_SOCKET
  chargen_init();
#endif /* LWIP_CHARGEN_APP && LWIP_SOCKET */

#if LWIP_NETBIOS_APP && LWIP_UDP
  netbiosns_init();
#ifndef NETBIOS_LWIP_NAME
#if LWIP_NETIF_HOSTNAME
  netbiosns_set_name(netif_default->hostname);
#else
  netbiosns_set_name("NETBIOSLWIPDEV");
#endif
#endif
#endif /* LWIP_NETBIOS_APP && LWIP_UDP */

#if LWIP_HTTPD_APP && LWIP_TCP
#if LWIP_HTTPD_APP_NETCONN
  http_server_netconn_init();
#else /* LWIP_HTTPD_APP_NETCONN */
  httpd_init();
#endif /* LWIP_HTTPD_APP_NETCONN */
#endif /* LWIP_HTTPD_APP && LWIP_TCP */

#if LWIP_MDNS_RESPONDER
  mdns_resp_init();
#if LWIP_NETIF_HOSTNAME
  (void)mdns_resp_add_netif(netif_default, netif_default->hostname, 3600);
#else
  (void)mdns_resp_add_netif(netif_default, "lwip", 3600);
#endif
#if LWIP_HTTPD_APP
  (void)mdns_resp_add_service(netif_default, "lwipweb", "_http", DNSSD_PROTO_TCP, (u16_t)HTTPD_SERVER_PORT, 3600, srv_txt, NULL);
#endif
#endif

#if LWIP_NETIO_APP && LWIP_TCP
  netio_init();
#endif /* LWIP_NETIO_APP && LWIP_TCP */

#if LWIP_RTP_APP && LWIP_SOCKET && LWIP_IGMP
  rtp_init();
#endif /* LWIP_RTP_APP && LWIP_SOCKET && LWIP_IGMP */

#if LWIP_SNTP_APP && LWIP_SOCKET
  sntp_init();
#endif /* LWIP_SNTP_APP && LWIP_SOCKET */

#if LWIP_SHELL_APP && LWIP_NETCONN
  shell_init();
#endif /* LWIP_SHELL_APP && LWIP_NETCONN */
#if LWIP_SSL_ECHO_APP && LWIP_SOCKET
  secure_socket_init();
#endif
#if LWIP_TCPECHO_APP
#if LWIP_NETCONN && LWIP_TCPECHO_APP_NETCONN
  tcpecho_init();
#else /* LWIP_NETCONN && LWIP_TCPECHO_APP_NETCONN */
  tcpecho_raw_init();
#endif
#endif /* LWIP_TCPECHO_APP && LWIP_NETCONN */
#if LWIP_UDPECHO_APP
#if LWIP_NETCONN && LWIP_UDPECHO_APP_NETCONN
  for(int i = 0; i < ETHIF_NUMBER; i++)
  {
    udpecho_init(&network_interfaces[i]);
  }
#else /* LWIP_NETCONN && LWIP_UDPECHO_APP_NETCONN */
  udpecho_raw_init();
#endif /* LWIP_NETCONN && LWIP_UDPECHO_APP_NETCONN */
#endif /* LWIP_UDPECHO_APP */
#if LWIP_LWIPERF_APP
  (void)lwiperf_start_tcp_server_default(lwiperf_report, NULL);
#endif
#if LWIP_SOCKET_EXAMPLES_APP && LWIP_SOCKET
  socket_examples_init();
#endif /* LWIP_SOCKET_EXAMPLES_APP && LWIP_SOCKET */

#if defined (COVERAGE_ENABLED) && (NO_SYS == 0)
  /* Start code coverage thread */
  for(int i = 0; i < ETHIF_NUMBER; i++)
  {  
    Coverage_init(&network_interfaces[i]);
  }
#endif

#ifdef LWIP_APP_INIT
  LWIP_APP_INIT();
#endif
}

/* This function initializes this lwIP test. When NO_SYS=1, this is done in
 * the main_loop context (there is no other one), when NO_SYS=0, this is done
 * in the tcpip_thread context */
static void
test_init(void* arg)
{
/* remove compiler warning */
#if NO_SYS
  LWIP_UNUSED_ARG(arg);
#else /* NO_SYS */
  sys_sem_t* init_sem = (sys_sem_t*)arg;
  LWIP_ASSERT("init_sem != NULL", init_sem != NULL);
#endif /* NO_SYS */

  /* init network interfaces */

  interface_init();

  /* init apps */
  apps_init();

#if !NO_SYS
  sys_sem_signal(init_sem);
#endif /* !NO_SYS */
}

static void mainLoopTask(void* pvParameters)
{
  (void)pvParameters;

  /* initialize lwIP stack, network interfaces and applications */
#if !NO_SYS
  err_t err;
  sys_sem_t init_sem;

#if defined (CPU_MPC5777C)|| defined (CPU_MPC5746R)
#ifdef PIT_0
#define PIT PIT_0
#endif
    PIT->MCR &= (uint32_t)(~PIT_MCR_FRZ_MASK);/* stop the timer in debug workaround for compability on MPC5777C PIT timer*/
#endif

  err = sys_sem_new(&init_sem, 0);
  LWIP_ASSERT("failed to create init_sem", err == (err_t)ERR_OK);
  LWIP_UNUSED_ARG(err);
  tcpip_init(test_init, (void*)&init_sem);
  /* we have to wait for initialization to finish before
   * calling update_adapter()! */
  (void)sys_sem_wait(&init_sem);
  sys_sem_free(&init_sem);
#if (LWIP_SOCKET || LWIP_NETCONN) && LWIP_NETCONN_SEM_PER_THREAD
  netconn_thread_init();
#endif
#else /* !NO_SYS */
  sys_init();
  lwip_init();
  test_init(NULL);
#endif /* !NO_SYS */


#if LWIP_INIT_COMPLETE_CALLBACK
  tcpip_init_complete_callback();
#endif /* LWIP_INIT_COMPLETE_CALLBACK */

  /* MAIN LOOP for driver update (and timers if NO_SYS) */
  while (1) {
#if NO_SYS
    /* handle timers (already done in tcpip.c when NO_SYS=0) */
    sys_check_timeouts();
  for(int i = 0; i < ETHIF_NUMBER ; i++)
  {
    (void)ETHIF_POLL_INTERFACE(&network_interfaces[i]);
  }
#else /* NO_SYS */
    sys_msleep(5000);
#endif /* NO_SYS */
  }

#if (LWIP_SOCKET || LWIP_NETCONN) && LWIP_NETCONN_SEM_PER_THREAD
  netconn_thread_cleanup();
#endif
  /* release the network interfaces... */
  for(int i = 0 ; i < ETHIF_NUMBER ; i++)
  {
	ETHIF_SHUTDOWN(&network_interfaces[i]);
  }
}

void start_example(void)
{
#if defined(USING_OS_FREERTOS)
  BaseType_t ret =xTaskCreate(mainLoopTask, "mainloop", 256U, NULL, DEFAULT_THREAD_PRIO, NULL);
                                     /* Start the tasks and timer running. */
  LWIP_ASSERT("failed to create mainloop", ret == pdPASS);
  vTaskStartScheduler();

  /* If all is well, the scheduler will now be running, and the following
  line will never be reached.  If the following line does execute, then
  there was insufficient FreeRTOS heap memory available for the idle and/or
  timer tasks to be created.  See the memory management section on the
  FreeRTOS web site for more details. */
  for (;;)
  {
  }
#else
    mainLoopTask(NULL);
#endif /* defined(USING_OS_FREERTOS) */
}

#if defined(USING_OS_FREERTOS)
void vAssertCalled(uint32_t ulLine, const char * const pcFileName)
{
  /* Called if an assertion passed to configASSERT() fails.  See
  www.freertos.org/a00110.html#configASSERT for more information. */

#if PRINTF_SUPPORT
  printf("ASSERT! Line %d, file %s\r\n", ulLine, pcFileName);
#else
  /* Parameters are not used. */
  LWIP_UNUSED_ARG(ulLine);
  LWIP_UNUSED_ARG(pcFileName);
#endif

  taskENTER_CRITICAL();
  {
    LWIP_ASSERT("configASSERT():", 0);
  }
  taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
  size of the heap available to pvPortMalloc() is defined by
  configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
  API function can be used to query the size of free heap space that remains
  (although it does not provide information on how the remaining heap might be
  fragmented).  See www.freertos.org/a00111.html for more
  information. */
  vAssertCalled(__LINE__, __FILE__);
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
  (void) pcTaskName;
  (void) pxTask;
  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected.  This function is
  provided as an example only as stack overflow checking does not function
  when running the FreeRTOS Windows port. */
  vAssertCalled(__LINE__, __FILE__);
}

/* Dummy functions needed by FreeRTOS to avoid compiler error*/
void vMainConfigureTimerForRunTimeStats( void )
{
  ;
}
uint32_t ulMainGetRunTimeCounterValue( void )
{
  return 0UL;
}

#endif /* defined(USING_OS_FREERTOS) */
