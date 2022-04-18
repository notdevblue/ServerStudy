#include <stdlib.h>
#include "./queue.h"
#include "../han.h"

char **queue;
int g_length;
int g_idx;

void init(int length)
{
   g_length = g_length;
   g_idx = 0;
   queue = (char **)malloc(sizeof(char *) * g_length);
   for (int i = 0; i < g_length; ++i)
   {
      queue[i] = (char *)malloc(sizeof(char) * PACKET_SIZE);
   }
}

void erase()
{
   for (int i = 0; i < g_length; ++i)
   {
      free(queue[i]);
   }

   free(queue);
}

void enqueue(const char * str)
{
   if (g_idx >= g_length - 1)
   {
      return;
   }

   queue[g_idx++] = str;
}

const char *dequeue()
{
   return queue[g_idx--];
}

int count()
{
   return g_idx;
}