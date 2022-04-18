#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "./han.h"

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


   sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock == -1)
   {
      perror("Error creating socket");
      return;
   }

   if (connect(sock, (SOCKADDR *)&addr, sizeof(addr)) == -1)
   {
      perror("Error connecting server");
      return;
   }

   printf("Connected to server.\r\n");

   while (1)
   {
      memset(buffer, 0, PACKET_SIZE);
      scanf("%s", buffer);

      if (buffer == "EXIT")
         break;

      // send(sock, buffer, PACKET_SIZE, 0);
   }

   shutdown(sock, 2);
   close(sock);
}