#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include "./han.h"

void main()
{
   SOCKET listening_sock;
   SOCKET client_sock;
   SOCKADDR_IN addr;
   {
      addr.sin_family = AF_INET;
      addr.sin_port = htons(PORT);
      addr.sin_addr.s_addr = INADDR_ANY;
   }

   char buffer[PACKET_SIZE];

   listening_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (listening_sock == -1) 
   {
      perror("Error creating listening socket\r\n");
      return;
   }

   if (bind(listening_sock, (SOCKADDR *)&addr, sizeof(addr)) == -1)
   {
      perror("error binding listening socket\r\n");
      return;
   }

   if (listen(listening_sock, SOMAXCONN) == -1)
   {
      shutdown(listening_sock, 2);
      close(listening_sock);
      perror("Error listening\r\n");
      return;
   }

   printf("Server listening...\r\n");

   client_sock = accept(listening_sock, NULL, NULL);
   if (client_sock == -1)
   {
      perror("Error accepting client\r\n");
      return;
   }

   shutdown(listening_sock, 2);
   close(listening_sock);
   shutdown(client_sock, 2);

   while (1)
   {
      int len = recv(client_sock, buffer, PACKET_SIZE, 0);
      if (len <= 0)
      {
         printf("%s\r\n", buffer);
         printf("Client disconnected\r\n");
         break;
      }
      printf("Received %s\r\n", buffer);

      send(client_sock, buffer, PACKET_SIZE, 0);
   }

   shutdown(client_sock, 2);
   close(client_sock);
}