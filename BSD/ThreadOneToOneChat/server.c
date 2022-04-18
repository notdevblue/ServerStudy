#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "./han.h"

void *recvthread(void *psocket)
{
   SOCKET* sock = (SOCKET *)psocket;
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
         printf("Client exitted.\r\n");
         break;
      }

      printf("Someone: %s\r\n", buffer);
   }

   return 0;
}

void main()
{
   SOCKET sock;
   SOCKET conn_sock;
   SOCKADDR_IN addr;
   {
      addr.sin_family = AF_INET;
      addr.sin_port = htons(PORT);
      addr.sin_addr.s_addr = INADDR_ANY;
   }
   char buffer[PACKET_SIZE];
   int sendresult;

   pthread_t recv_thread;

   printf("Server is starting...\r\n");

   // create listening socket
   sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock == INVALID_SOCKET)
   {
      perror("socket() failed");
      close(sock);
      return;
   }

   // bind
   if (bind(sock, (SOCKADDR *)&addr, sizeof(addr)) == FAILED)
   {
      perror("bind() failed");
      close(sock);
      return;
   }

   printf("Server is listening on port %d...\r\n", PORT);

   // make it listen
   if (listen(sock, 1) == FAILED) // one to one chat; setted backlog to 1
   {
      perror("listen() failed");
      close(sock);
      return;
   }

   // accept connection
   conn_sock = accept(sock, NULL, NULL);
   
   // close listening socket
   close(sock);

   if (conn_sock == INVALID_SOCKET)
   {
      perror("accpet() failed");
      close(conn_sock);
      return;
   }

   printf("Client connected.\r\n");

   // make buffer receive thread
   pthread_create(&recv_thread, NULL, recvthread, (void*)&conn_sock);

   while (1)
   {
      fgets(buffer, PACKET_SIZE, stdin);
      sendresult = send(conn_sock, buffer, PACKET_SIZE, 0);

      if (sendresult == FAILED)
      {
         perror("send() failed");
         break;
      }
   }

   printf("Quitting...\r\n");

   // close socket
   shutdown(conn_sock, 2);
   close(conn_sock);

   // wait for receive thread to terminated
   pthread_join(recv_thread, NULL);

   printf("Press any key to exit...");
   getchar();
}