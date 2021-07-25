/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
* @file ccov.c
*/
#ifdef COVERAGE_ENABLED
#include "lwip/igmp.h"
#include "lwip/opt.h"
#include <string.h>
#include "apps/ccov/ccov.h"

#if LWIP_NETCONN
#include "lwip/api.h"
#include "lwip/sys.h"

#include "netifcfg.h"

#define UDP_MESSAGE_SEND_PORT 18
#define BOX_SIZE 2

static void CoverageSendTask(void* arg);
static void CoverageRecvTask(void* arg);
static void CoverageRecvTask1(void* arg);
/*-----------------------------------------------------------------------------------*/
static char buffer1[4096];
/* structure used to save the pointer to netif passed in udpecho_init */
struct netif *netif_shutdown;
static sys_mutex_t Send_Recv_mutex ;
static sys_mbox_t  Send_Recv_box;
static uint8_t Coverage_test_done_flag = 0;

/**
 * Send task for Coverage
 *
 * @param arg unused
 * Start the mutex mbox and other thread needed for Coverage
 * Creat a mutex, take a mutex, create a task which takes the same mutex, block it for a while
 * Creat a mailbox, create a task which periodically receives the msg, send msg with faster
 * speed in order to overload the mail box 
 */

static void CoverageSendTask(void* arg)
{
  uint8_t i;
  TaskHandle_t TaskHandler;
  sys_mbox_t* out_box;
  out_box = (sys_mbox_t*)arg;
  sys_mutex_new(&Send_Recv_mutex);
  sys_mbox_new(&Send_Recv_box, BOX_SIZE);

  BaseType_t ret =xTaskCreate(CoverageRecvTask, "RecvTask", 256U, NULL, DEFAULT_THREAD_PRIO, &TaskHandler);
  LWIP_ASSERT("failed to create RecvTask", ret == pdPASS);
  OSIF_TimeDelay(50);

  if (Send_Recv_mutex != NULL)
  {
    sys_mutex_lock(&Send_Recv_mutex);
    OSIF_TimeDelay(5000);
    sys_mutex_unlock(&Send_Recv_mutex);
    OSIF_TimeDelay(1000);
  }
  vTaskDelete(TaskHandler);

  xTaskCreate(CoverageRecvTask1, "receiver", 256U, NULL, DEFAULT_THREAD_PRIO, &TaskHandler);
  for (i = 0; i<10; i++)
  {
    sys_mbox_post(&Send_Recv_box, "a");
  }
  vTaskDelete(TaskHandler);

  xTaskCreate(CoverageRecvTask1, "receiver", 256U, NULL, DEFAULT_THREAD_PRIO, &TaskHandler);
  for (i = 0; i<10; i++)
  {
    sys_mbox_post_to_front(&Send_Recv_box, "b");
  }
  vTaskDelete(TaskHandler);

  Coverage_test_done_flag = 1;
  for(;;)
  {
    OSIF_TimeDelay(1000);
  }
}
/*-----------------------------------------------------------------------------------*/
static void CoverageRecvTask(void* arg)
{
  OSIF_TimeDelay(50);

  if (Send_Recv_mutex != NULL)
  {
    OSIF_TimeDelay(500);
    sys_mutex_lock(&Send_Recv_mutex);
    sys_mutex_unlock(&Send_Recv_mutex);
  }
  LWIP_UNUSED_ARG(arg);
  for(;;)
  {
    OSIF_TimeDelay(1000);
  }
}

/* Dummy handler for packets, returns a different value from forward frame, as it is not 
 * processed here. 
 */
unsigned int dummy_ethif_handler(uint8_t eth_instance, ETHIF_BUFFER_t *buff)
{
    /* Do not return forward frame, as it will discard packets */
    return FORWARD_FRAME + 1; 
}

/* additional protocol network handler */
rx_buff_process_condition_handler_t handler = &dummy_ethif_handler;

/*-----------------------------------------------------------------------------------*/
static void CoverageRecvTask1(void* arg)
{
  uint8_t i;
  uint32_t buff;
  uint32_t * buffptr = &buff;
  OSIF_TimeDelay(20000);
  for(i =0; i<10; i++)
  {
    sys_arch_mbox_fetch(&Send_Recv_box, &buffptr, 5000);
    OSIF_TimeDelay(50);
  }
  LWIP_UNUSED_ARG(arg);
  for(;;)
  {
    OSIF_TimeDelay(1000);
  };
}
/*-----------------------------------------------------------------------------------*/
/**
 * Main loop task for Coverage
 *
 * @param arg unused
 * Continuously check for UDP msg from computer
 * when receive COVERAGE_TEST start task for ethernet interface Coverage
 * when receive NETIF_SHUTDOWN turn of the netif for netif shut down fcn test
 * binded to port 18 (UDP_MESSAGE_SEND_PORT)
 */

static void
Coverage_thread(void *arg)
{
  struct netconn *conn;
  struct netbuf *buf;
  TaskHandle_t TaskHandler = NULL;
  err_t err;
  LWIP_UNUSED_ARG(arg);
  ip_addr_t * remote_computer_addr;
  uint16_t * remote_computer_port;

#if LWIP_IPV6
  conn = netconn_new(NETCONN_UDP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, UDP_MESSAGE_SEND_PORT);
#else /* LWIP_IPV6 */
  conn = netconn_new(NETCONN_UDP);
  netconn_bind(conn, IP_ADDR_ANY, UDP_MESSAGE_SEND_PORT);
#endif /* LWIP_IPV6 */

  LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);

  for(;;)
  {
    if ((TaskHandler!= NULL)&&(Coverage_test_done_flag == 1))
    {
      vTaskDelete(TaskHandler);
      TaskHandler = NULL;
      Coverage_test_done_flag = 0;
    }
    /*Polling for msg from connection*/
    err = netconn_recv(conn, &buf);
    if (err == ERR_OK)
    {
      if(netbuf_copy(buf, buffer1, sizeof(buffer1)) != buf->p->tot_len)
      {
        LWIP_DEBUGF(LWIP_DBG_ON, ("netbuf_copy failed\n"));
      }
      /*  no need netconn_connect here, since the netbuf contains the address */
      else
      {
      /* if receive message "COVERAGE_TEST", start to overload the mailbox      */
        if (buf->p->tot_len != 0 && !strncmp(&buffer1[0], "COVERAGE_TEST", 13))
        {
        /*start thread*/
          ETHIF_REGISTER_RX_BUFF_PROCESS_CONDITION_HANDLER(handler);
          BaseType_t ret =xTaskCreate(CoverageSendTask, "SendTask", 256U, NULL, DEFAULT_THREAD_PRIO, &TaskHandler);
          LWIP_ASSERT("failed to create SendTask", ret == pdPASS);
          Coverage_test_done_flag = 0;
          /*get the ip and port of the computer for future use*/
          netconn_getaddr(conn, &remote_computer_addr, &remote_computer_port, 0);
        }
      /* if receive message "NETIF_SHUTDOWN", call function ETHIF_SHUTDOWN() with the
        netif used for this echo server     */
        if (buf->p->tot_len == 14 && !strncmp(&buffer1[0], "NETIF_SHUTDOWN", 14))
        {
          send_tx_pbuffs_dummy_char();
          send_rx_pbuffs_dummy_char();
          ETHIF_REGISTER_RX_BUFF_PROCESS_CONDITION_HANDLER(NULL);
          ETHIF_SHUTDOWN(netif_shutdown);
        }
        netbuf_delete(buf);
      }
    }
  }
}
/*-----------------------------------------------------------------------------------*/
  void
  Coverage_init(struct netif *netif)
  {
    netif_shutdown = netif;
    sys_thread_new("Coverage_thread", Coverage_thread, NULL, 512, DEFAULT_THREAD_PRIO);
  }
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
#endif /*COVERAGE_ENABLED*/
