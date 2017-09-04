#include<stdio.h>
#include<assert.h>
#define BITS_PER_WORD 32
#define DEFAULT_NBITS 64

void decimal_to_binary(unsigned int decimal, int array[])
{
   int remain = decimal;
   int curr_index = BITS_PER_WORD-1;
   for(int i=0;i<BITS_PER_WORD;i++)//initialize array to be all zeros
   {
      array[i]=0;
   }
   
   while(remain>0)
   {
      if(remain%2==0)
      {
         array[curr_index]=0;
      }
      else
      {
         array[curr_index]=1;
      }
      remain /=2;
      curr_index--;
      assert(curr_index>0);
   }
}


int main(void)
{
   unsigned int decimal = 1024;
   int array[BITS_PER_WORD]={0};
   decimal_to_binary(decimal, array);
   
   for(int i=0;i<BITS_PER_WORD;i++)
   {
      printf("%d",array[i]);
   }
   printf("\n");
   return 0;
}






