// where_are_the_bits.c ... determine bit-field order
// COMP1521 Lab 03 Exercise
// Written by ...

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
     unsigned int a : 4,
                  b : 8,
                  c : 20;
   
};

union _bit_union{
   struct _bit_fields bit;
   unsigned int num;
};

int main(void)
{
   union _bit_union u;
   u.bit.a=1;
   u.bit.b=0;
   u.bit.c=0;

   printf("when\na=1,\nb=0,\nc=0,\nthe size of x is %u\n",sizeof(u.bit));
   printf("the value of unsigned int u is %u\n",u.num);



   u.bit.a=0;
   u.bit.b=0;
   u.bit.c=1;

   printf("when\na=0,\nb=0,\nc=1,\nthe size of x is %u\n",sizeof(u.bit));
   printf("the value of unsigned int u is %u\n",u.num);
   return 0;
}

