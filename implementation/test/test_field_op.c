/**
 * Author: Sebastien Duc
 *
 * Test the field multiplication
 *
 */
#include <stdio.h>

#include "../field_op.h"
#include "../config.h"


uint64_t test_gfmul(uint64_t a, uint64_t b)
{
    uint64_t res = _F64_mult_(a,b);
    return res;
}


int main(void)
{
   uint64_t a,b;
   
   a = 0xff12123123;
   b = 0xefefffffff;

   uint64_t res = test_gfmul(a,b);
   if(res == 0xde12d0773e085951){
       printf("OK \\/\n");
   }
   else {
       printf("FAIL:\n");
       printf("%llx\n",res);
       printf("8a9a107cee0b487b\n");
   }
}
