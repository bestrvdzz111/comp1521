unsigned int translate_array_to_one_word(int int_string[]);

unsigned int translate_array_to_one_word(int int_string[])
{
   unsigned int num = 0;
   unsigned int plus=1;
   for(int i = BITS_PER_WORD-1; i>=0 ; i--)
   {
      if(int_string[i]== 1)
      {
         num += plus; 
      }
      plus*=2;
   }
   return num;
}
