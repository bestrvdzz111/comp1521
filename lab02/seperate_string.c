#include<stdio.h>
#include<assert.h>

#define  DEFAULT_NBITS 64
#define BITS_PER_WORD 32
void seperate_string(int nth,int std_int_array[],int long_int_array[]);

int main(void)
{
   int array[64]={0};
   array[1]=array[0]=1;
   
   int std_array[32]={0};
   seperate_string(2,std_array,array);
   for(int i=0; i<32;i++)
   {
      printf("%d",std_array[i]);
   }
   printf("\n");
   return 0;
}

void seperate_string(int nth,int std_int_array[],int long_int_array[])
{
   assert(nth>0);
   int left_index=0;
   int right_index=BITS_PER_WORD-1;
   for(int i = 1; i<nth;i++)
   {
      left_index+=BITS_PER_WORD;
      right_index+=BITS_PER_WORD;
   }
   for(int i = 0;i<BITS_PER_WORD;i++)
   {
      std_int_array[i]=long_int_array[left_index];
      left_index++;
   }
   assert(left_index==right_index+1);
}
