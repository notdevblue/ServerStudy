#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include "./han.h"

pthread_mutex_t g_lock;

void *recvthread(void *psocket)
{
   SOCKET *sock = (SOCKET *)psocket;
   char buffer[PACKET_SIZE];
   int len;

   while (1)
   {
      len = recv(*sock, buffer, PACKET_SIZE, 0);

      if (len == FAILED)
      {
         perror("recv() failed");
         return 0;
      }

      if (len <= CLIENT_LEFT)
      {
         printf("Lost connection with server.\r\n");
         break;
      }

      printf("Someone: %s", buffer);
   }

   return 0;
}

void main()
{
   SOCKET sock;
   SOCKADDR_IN addr;
   {
      addr.sin_family = AF_INET;
      addr.sin_port = htons(PORT);
      inet_pton(AF_INET, "127.0.0.1", (char *)&addr.sin_addr.s_addr);
   }
   char buffer[PACKET_SIZE];
   int sendresult;

   pthread_t recv_thread;

   printf("Client is starting...\r\n");

   // create listening socket
   sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock == INVALID_SOCKET)
   {
      perror("socket() failed");
      close(sock);
      return;
   }

   // connect
   if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == FAILED)
   {
      perror("connect() failed");
      return;
   }

   printf("Connected.\r\n");

   // make buffer receive thread
   pthread_create(&recv_thread, NULL, recvthread, (void *)&sock);

   while (1)
   {
      fgets(buffer, PACKET_SIZE, stdin);
      sendresult = send(sock, buffer, PACKET_SIZE, 0);

      if (sendresult == FAILED)
      {
         perror("send() failed");
         break;
      }
   }

   printf("Quitting...\r\n");

   // close socket
   shutdown(sock, 2);
   close(sock);

   // wait for receive thread to terminated
   pthread_join(recv_thread, NULL);

   printf("\r\nPress any key to exit...\r\n");
   getchar();
}