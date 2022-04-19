#include <stdlib.h>
#include "./queue.h"
#include "../han.h"

char **queue;
int g_length;
int g_idx;

int g_counter;

void init(int length)
{
   g_length = length;
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

void enqueue(char *str)
{
   if (g_idx >= g_length - 1)
   {
      exit(3);
   }

   queue[g_idx++] = str;
}

char *dequeue()
{
   return queue[--g_idx];
}

char *peek_and_dequeue(int length)
{
   if(g_counter <= 0)
      g_counter = length;

   --g_counter;

   if(g_counter <= 0)
   {
      return queue[--g_idx];
   }
   else
   {
      return queue[g_idx - 1];
   }
}

int count()
{
   return g_idx;
}