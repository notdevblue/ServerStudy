#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void main()
{
   struct sockaddr_in addr;
   int sock, res;
   int cnt = 0;
   char dbuf[1024] = "Gucci Gang\0";

   while (1)
   {
      sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      addr.sin_port = htons(48000);
      addr.sin_family = AF_INET;
      inet_pton(AF_INET, "127.0.0.1", (char *)&addr.sin_addr.s_addr);

      connect(sock, (struct sockaddr *)&addr, sizeof(addr));
      // while (cnt++ < 10)
      // {
         res = send(sock, (char *)&dbuf, sizeof(dbuf), 0);
      //    system("sleep 0.5");
      // }
      cnt = 0;
      shutdown(sock, 2);
   }
}