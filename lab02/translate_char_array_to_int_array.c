#include<stdio.h>
#include<string.h>

#define  DEFAULT_NBITS 64
void translate_char_array_to_int_array(char* bitseq,int *int_array);
int main(void)
{
   char * char_string ="11111111111111111111111111111111";
   int array[64]={0};
   translate_char_array_to_int_array(char_string, array);
   for(int i=0;i<64;i++)
   {
      printf("%d",array[i]);
   }
}

void translate_char_array_to_int_array(char* bitseq, int *int_array)
{
	for(int i=0;i<DEFAULT_NBITS;i++)
	{
		int_array[i]=0;
	}
   int size= strlen(bitseq);
   
   int bit_counter=size-1;
   int int_counter=DEFAULT_NBITS-1;
   while(bit_counter!=-1 && int_counter!=-1)
   {
      int_array[int_counter]=bitseq[bit_counter]-'0';
      bit_counter--;
      int_counter--;
   }
   
}
