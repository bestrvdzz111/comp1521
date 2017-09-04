// ADT for Bit-strings
// COMP1521 17s2 Week02 Lab Exercise
// Written by John Shepherd, July 2017
// Modified by ...

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Bits.h"

// assumes that an unsigned int is 32 bits
#define BITS_PER_WORD 32
#define DEFAULT_NBITS 64
// A bit-string is an array of unsigned ints (each a 32-bit Word)
// The number of bits (hence Words) is determined at creation time
// Words are indexed from right-to-left
// words[0] contains the most significant bits
// words[nwords-1] contains the least significant bits
// Within each Word, bits are indexed right-to-left
// Bit position 0 in the Word is the least significant bit
// Bit position 31 in the Word is the most significant bit

typedef unsigned int Word;

struct BitsRep {
   int nwords;   // # of Words
   Word *words;  // array of Words
};


void translate_char_array_to_int_array(char* bitseq,int *int_array);
void seperate_string(int nth,int std_int_array[],int long_int_array[]);
unsigned int translate_array_to_one_word(int int_string[]);
void decimal_to_binary(unsigned int decimal, int array[]);

// make a new empty Bits with space for at least nbits
// rounds up to nearest multiple of BITS_PER_WORD
Bits makeBits(int nbits)
{
   Bits new;
   new = malloc(sizeof(struct BitsRep));
   assert(new != NULL);
   int  nwords;
   if (nbits%BITS_PER_WORD == 0)
      nwords = nbits/BITS_PER_WORD;
   else
      nwords = 1+nbits/BITS_PER_WORD;
   new->nwords = nwords;
   // calloc sets to all 0's
   new->words = calloc(nwords, sizeof(Word));
   assert(new->words != NULL);
   return new;
}

// release space used by Bits
void  freeBits(Bits b)
{
   assert(b != NULL && b->words != NULL);
   free(b->words);
   free(b);
}

// form bit-wise AND of two Bits a,b
// store result in res Bits
void andBits(Bits a, Bits b, Bits res)
{

	for(int i = 0;i < res ->nwords;i++)
	{
		res->words[i] = (a->words[i] & b->words[i]);
	}
}

// form bit-wise OR of two Bits a,b
// store result in res Bits
void orBits(Bits a, Bits b, Bits res)
{

	for(int i = 0;i < res ->nwords;i++)
	{
		res->words[i] = (a->words[i] | b->words[i]);
	}
}

// form bit-wise negation of Bits a,b
// store result in res Bits
void invertBits(Bits a, Bits res)
{
	
   res->nwords= a ->nwords;
   for(int i=0;i < res -> nwords; i++)
   {
	
      //printf("the negation of %d is ",a->words[i]);
      res->words[i]=~a->words[i];
      //printf("%d\n",res->words[i]);
   }
}

// left shift Bits
void leftShiftBits(Bits b, int shift, Bits res)
{
   // challenge problem
	for(int i = 0;i <res ->nwords;i++)
	{
		res->words[i] = b->words[i] << shift;
	}
}

// right shift Bits
void rightShiftBits(Bits b, int shift, Bits res)
{
   // challenge problem
	for(int i = 0;i < res ->nwords;i++)
	{
		res->words[i] = b->words[i] >> shift;
	}
}

// copy value from one Bits object to another
void setBitsFromBits(Bits from, Bits to)
{
   to->nwords=from->nwords;
   for(int i=0; i < to->nwords; i++)
   {
      to->words[i]=from->words[i];
   }
}


// assign a bit-string (sequence of 0's and 1's) to Bits
// if the bit-string is longer than the size of Bits, truncate higher-order bits
void setBitsFromString(Bits b, char *bitseq)
{
   int int_array[DEFAULT_NBITS]={0};
   int std_int_array[BITS_PER_WORD]={0};
   
   translate_char_array_to_int_array(bitseq, int_array);
   
   for(int nth =1;nth <= b->nwords;nth++)
   {
      
      seperate_string(nth,std_int_array,int_array);
      b->words[nth-1]= translate_array_to_one_word(std_int_array);
      //printf("%d",b->words[nth-1]);
   }

}

// display a Bits value as sequence of 0's and 1's
void showBits(Bits b)
{
   //printf("words[0] is %d, words[1] is %d\n",b->words[0],b->words[1]);
   //printf("nwords is %d\n",b->nwords);
   int array[BITS_PER_WORD];
   for(int nth =1; nth <= b->nwords; nth++)
   {
      decimal_to_binary(b->words[nth-1], array);
      for(int i =0 ; i<BITS_PER_WORD;i++)
      {
         printf("%u",array[i]);
      }
   }
	//printf("The first number is %u the second is %u\n",b->words[0],b->words[1]);
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
	assert(num>=0);
	//printf("translation from binary to decimal:%d\n",num);
   return num;
}


void decimal_to_binary(unsigned int decimal, int array[])
{
   unsigned int remain = decimal;
   unsigned int curr_index = BITS_PER_WORD-1;
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
      assert(curr_index>=0);
   }
}










