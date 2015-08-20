#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
#include <wmmintrin.h>
#include <omp.h>
#include <stdint.h>

int main(void)
{
    uint64_t ukey1 [2] = {0,0};
    uint64_t ukey2 [3] = {1,1,1};
    uint64_t ukey3 [2] = {2,2};
    
    const uint64_t AES_in_1 [2] = {0xdeadbeef,0xdeadbeef};
    const uint64_t AES_in_2 [2] = {0xdeadbeef,0xdeadbeef};
    const uint64_t AES_in_3 [2] = {0xdeadbeef,0xdeadbeef};
    __m128i key_1 [10];
    __m128i key_2 [12];
    __m128i key_3 [10];
    uint64_t AES_out_1 [3] = {0,0,0};
    uint64_t AES_out_2 [3] = {0,0,0};
    uint64_t AES_out_3 [3] = {0,0,0};

#pragma omp parallel sections
    {
#pragma omp section
        {
            AES_128_Key_Expansion((const unsigned char *) (ukey1) ,
                                  (unsigned char *)key_1);
            AES_128_Encrypt((const unsigned char *)AES_in_1 ,
                            (unsigned char *)AES_out_1 ,
                            (const unsigned char *)key_1);
        }
#pragma omp section
        {
            AES_192_Key_Expansion((const unsigned char *) (ukey2) ,
                                  (unsigned char *)key_2);
            AES_192_Encrypt((const unsigned char *)AES_in_2 ,
                            (unsigned char *)AES_out_2 ,
                            (const unsigned char *)key_2);
        }
#pragma omp section
        {
            AES_128_Key_Expansion((const unsigned char *) (ukey3) ,
                                  (unsigned char *)key_3);
            AES_128_Encrypt((const unsigned char *)AES_in_3 ,
                            (unsigned char *)AES_out_3 ,
                            (const unsigned char *)key_3);
        }
    }

    puts((char *)AES_out_1);
    puts((char *)AES_out_2);
    puts((char *)AES_out_3);

}

