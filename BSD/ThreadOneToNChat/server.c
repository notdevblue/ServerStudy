#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "./han.h"
#include "./Queue/queue.h"


void *listening_thread(void *param);
void *recv_thread(void *param);
void *send_thread(void *param);
void create_io_thread(SOCKET* sock);

int g_client_count = 0;
const int g_default_thread_running = 5;
pthread_t *g_recv_t_arr;
pthread_t *g_send_t_arr;
pthread_mutex_t g_lock;

void main()
{
   init(10);

   pthread_t listening_t;

   // g_recv_t_arr = (pthread_t *)malloc(sizeof(pthread_t) * g_default_thread_running);
   // g_send_t_arr = (pthread_t *)malloc(sizeof(pthread_t) * g_default_thread_running);

   pthread_create(&listening_t, NULL, listening_thread, NULL);


   // Wait for threads to terminated
   pthread_join(listening_t, NULL);
   for (int i = 0; i < (g_client_count < g_default_thread_running ? g_default_thread_running : g_client_count); ++i)
   {
      pthread_mutex_lock(&g_lock);
      pthread_join(g_recv_t_arr[i], NULL);
      pthread_join(g_recv_t_arr[i], NULL);
      pthread_mutex_unlock(&g_lock);
   }

   erase();
   free(g_recv_t_arr);
   free(g_send_t_arr);
}


void *listening_thread(void *param)
{
   SOCKET sock;
   SOCKADDR_IN addr;
   {
      addr.sin_family = AF_INET;
      addr.sin_port = htons(PORT);
      addr.sin_addr.s_addr = INADDR_ANY;
   }

   pthread_t *recv_t_arr = (void *)param;
   int optval = 1;

   while (1)
   {
      // create listening socket
      sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

      if (sock == INVALID_SOCKET)
      {
         perror("socket() failed.");
         _exit(1);
      }

      printf("Listening...\r\n");

      // bind listening socket
      if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == FAILED)
      {
         perror("bind() failed.");
         _exit(1);
      }

      // listen for connection
      if (listen(sock, SOMAXCONN) == FAILED)
      {
         perror("listen() failed.");
         _exit(1);
      }
      
      // accept connection
      SOCKET client = accept(sock, NULL, NULL);
      if (client == INVALID_SOCKET)
      {
         perror("accept() failed.");
         _exit(1);
      }      

      close(sock);

      create_io_thread(&client);
   }

   return 0;
}

void *recv_thread(void *param)
{
   SOCKET sock = *((SOCKET *)param);
   char buffer[PACKET_SIZE];
   int len;

   while (1)
   {
      len = recv(sock, buffer, PACKET_SIZE, 0);
      if (len == 0) // normal shutdown
      {
         break;
      }
      else if (len < 0) // error shutdown
      {
         perror("recv() failed.");
         break;
      }

      pthread_mutex_lock(&g_lock);
      enqueue(buffer);
      pthread_mutex_unlock(&g_lock);
      printf("recived %s", buffer);
   }

   shutdown(sock, 2);
   close(sock);
   return 0;
}

void *send_thread(void *param)
{
   SOCKET sock = *((SOCKET *)param);

   while(1)
   {
      if (count() <= 0) continue;
      
      pthread_mutex_lock(&g_lock);
      char *str = peek_and_dequeue(g_client_count);
      pthread_mutex_unlock(&g_lock);

      if (send(sock, str, PACKET_SIZE, 0) == FAILED)
      {
         perror("send() failed.");
         break;
      }
   }
}

void create_io_thread(SOCKET *sock)
{
   pthread_t *temp_recv_arr;
   pthread_t *temp_send_arr;
   int target_size = (++g_client_count + 1) * sizeof(pthread_t);

   pthread_mutex_lock(&g_lock);
   temp_recv_arr = (pthread_t *)realloc((void *)g_recv_t_arr, target_size);
   temp_send_arr = (pthread_t *)realloc((void *)g_send_t_arr, target_size);
   pthread_mutex_unlock(&g_lock);

   if (temp_recv_arr)
      g_recv_t_arr = temp_recv_arr;

   if (temp_send_arr)
      g_send_t_arr = temp_send_arr;

   pthread_create(&g_recv_t_arr[g_client_count], NULL, recv_thread, (void *)sock);
   pthread_create(&g_send_t_arr[g_client_count], NULL, send_thread, (void *)sock);
}
