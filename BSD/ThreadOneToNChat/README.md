# 익명채팅 서버, 클라이언트

<image src="https://camo.githubusercontent.com/d540627fba16c91057cf7fba9a446368b66f8ff45d85a72847451c675cedba75/68747470733a2f2f63646e2e646973636f72646170702e636f6d2f6174746163686d656e74732f3838383739373033353436383330383535302f3936353832303939323538363932343033332f5468726561644f6e65546f4e2e676966">

* * *

### 5.15.41-gentoo-x86_64, gcc (Gentoo 11.2.1) 을 사용해 테스트, 개발되었습니다.

* * *

## 사용된 헤더들
* pthread
* BSD Socket 헤더들

* * *

## 폴더 구조
ROOT
* Queue
   * queue.c
   * queue.h
* client.c
* han.h
* server.c

* * *

## 분석

### Han.h<br/>

<details>
   <summary>Han.h</summary>

```c
#pragma once

#define PORT 48000
#define PACKET_SIZE 1024
#define FAILED -1
#define INVALID_SOCKET -1
#define CLIENT_LEFT 0

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef int SOCKET;
```
자주 쓰는 타입 정의<br/><br/>


</details>

* * *


### Queue<br/>

<details>
   <summary>IO</summary>

```c
void enqueue(char *str)
{
   // 오버플로우
   if (g_idx >= g_length - 1)
      exit(3);

   queue[g_idx++] = str;
}

// 모든 스레드에서 데이터를 가져가면 하면 dequeue 됨
char *peek_and_dequeue(int length)
{
   if(g_counter <= 0)
      g_counter = length;

   --g_counter;

   if(g_counter <= 0)
      return queue[--g_idx];
   else
      return queue[g_idx - 1];
}
```
입출력<br/><br/>

</details>

<details>
   <summary>메모리 할당</summary>

```c
// 메모리 할당
void init(int length)
{
   g_length = length;
   g_idx = 0;
   queue = (char **)malloc(sizeof(char *) * g_length);

   for (int i = 0; i < g_length; ++i)
      queue[i] = (char *)malloc(sizeof(char) * PACKET_SIZE);
}
```
메모리 할당 작업<br/><br/>

</details>

* * *

### 서버<br/>

<details>
   <summary>Listening Thread</summary>

```c
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

   while (1) {
      // create listening socket
      sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

      if (sock == INVALID_SOCKET) {
         perror("socket() failed.");
         _exit(1);
      }

      printf("Listening...\r\n");

      // bind listening socket
      if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == FAILED) {
         perror("bind() failed.");
         _exit(1);
      }

      // listen for connection
      if (listen(sock, SOMAXCONN) == FAILED) {
         perror("listen() failed.");
         _exit(1);
      }
      
      // accept connection
      SOCKET client = accept(sock, NULL, NULL);
      if (client == INVALID_SOCKET) {
         perror("accept() failed.");
         _exit(1);
      }      

      close(sock);

      // 입출력 쓰레드 생성
      create_io_thread(&client);
   }

   return 0;
}
```
리스닝 <br/><br/>
</details>

<details>
   <summary>Send Thread</summary>

```c
void *send_thread(void *param)
{
   SOCKET sock = *((SOCKET *)param);

   while(1) {
      // 음
      if (count() <= 0) continue;
      
      // 페킷 Queue 에서 dequeue 함
      pthread_mutex_lock(&g_lock);
      char *str = peek_and_dequeue(g_client_count);
      pthread_mutex_unlock(&g_lock);
      
      if (send(sock, str, PACKET_SIZE, 0) == FAILED) {
         perror("send() failed.");
         break;
      }
   }
}
```
dequeue 한 뒤 send 함 <br/><br/>

</details>

<details>
   <summary>Receive Thread</summary>

```c
void *recv_thread(void *param)
{
   SOCKET sock = *((SOCKET *)param);
   char buffer[PACKET_SIZE];
   int len;

   while (1) {
      len = recv(sock, buffer, PACKET_SIZE, 0);
      if (len == 0) // normal shutdown
         break;
      else if (len < 0) { // error shutdown
         perror("recv() failed.");
         break;
      }

      // 페킷 도착 시 enqueue 함
      pthread_mutex_lock(&g_lock);
      enqueue(buffer);
      pthread_mutex_unlock(&g_lock);
      printf("recived %s", buffer);
   }

   shutdown(sock, 2);
   close(sock);
   return 0;
}
```
페킷 도착 시 enqueue 함 <br/><br/>

</details>

<details>
   <summary>IO Thread Createer</summary>

```c
void create_io_thread(SOCKET *sock)
{
   pthread_t *temp_recv_arr;
   pthread_t *temp_send_arr;
   int target_size = (++g_client_count + 1) * sizeof(pthread_t);

   // 재할당
   pthread_mutex_lock(&g_lock);
   temp_recv_arr = (pthread_t *)realloc((void *)g_recv_t_arr, target_size);
   temp_send_arr = (pthread_t *)realloc((void *)g_send_t_arr, target_size);
   pthread_mutex_unlock(&g_lock);

   // 예외 방지
   if (temp_recv_arr)
      g_recv_t_arr = temp_recv_arr;

   if (temp_send_arr)
      g_send_t_arr = temp_send_arr;

   // 인자로 연결이 이루어진 socket 의 주소를 넘기며 새 쓰레드를 생성함
   pthread_create(&g_recv_t_arr[g_client_count], NULL, recv_thread, (void *)sock);
   pthread_create(&g_send_t_arr[g_client_count], NULL, send_thread, (void *)sock);
}

```
IO 쓰레드 생성 <br/><br/>

</details>