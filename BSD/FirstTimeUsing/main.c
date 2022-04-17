#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void main()
{
   struct sockaddr_in addr;
   int sock, client, res;
   char dbuf[1024];
   while (1)
   {
      sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      addr.sin_port = htons(48000);
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = INADDR_ANY;
      bind(sock, (struct sockaddr *)&addr, sizeof(addr));
      listen(sock, SOMAXCONN);
      printf("Server listening...\r\n");

      client = accept(sock, NULL, NULL);
      close(sock);
      sock = client;

      while (1)
      {
         res = recv(sock, dbuf, sizeof(dbuf), 0);
         if (res <= 0)
            break;
         printf("Recived %s\r\n", dbuf);
      }

      shutdown(sock, 2);
   }
}