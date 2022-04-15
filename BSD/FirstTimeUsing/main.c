#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void main()
{
   int sock;
   sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

   struct sockaddr_in ServAddr;
   const char *servIP;
   int ServPort;

   memset(&ServAddr, 0, sizeof(ServAddr));
   ServAddr.sin_family = AF_INET;
   ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   ServAddr.sin_port = htons(36000);

   if (bind(sock, (struct sockaddr*)&ServAddr, sizeof(ServAddr)) < 0)
   {
      perror("bind");
      exit(EXIT_FAILURE);
   }

   listen(sock, SOMAXCONN);

   struct sockaddr_in ClntAddr;
   unsigned int clntLen;
   clntLen = sizeof(ClntAddr);
   int clntSock = accept(sock, (struct sockaddr *)&ClntAddr, &clntLen);

   send(clntSock, "Hello", 1024, 0);
   const char *buffer;
   recv(clntSock, buffer, 1024, 0);
}