#include <stdio.h>
#include <stdint.h>
#include <aes.h>
#include <wmmintrin.h>
#include <omp.h>

#define PRE_IN_LEN 6
#define PRE_OUT_LEN 10

#define cpuid(func,ax,bx,cx,dx)\
        __asm__ __volatile__ ("cpuid":\
        "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func)); 

int Check_CPU_support_AES()  
{ 
    unsigned int a,b,c,d; 
    cpuid(1, a,b,c,d); 
    return (c & 0x2000000) >> 25; 
} 

/*
static void PRE_Process(const uint64_t *in, // input: it is supposed to be 6 times 64 bits
                 uint64_t *out)      // output buffer: will contain 10 times 64 bits
{
    uint64_t c_1__p__c_2__b_1;
    uint64_t c_3__p__c_2__b_2;   
    uint64_t b_2__p__b_1__a_1;    

    // in = (a_1 , b_1 , b_2 , c_1 , c_2 , c_3)
    //                                              
    // some precomputations
#pragma omp parallel sections 
    {
#pragma omp section
        c_1__p__c_2__b_1 = in[3] + in[4] * in[1];
#pragma omp section
        c_3__p__c_2__b_2 = in[5] + in[4] * in[2];
#pragma omp section
        b_2__p__b_1__a_1 = in[2] + in[1] * in[0];
    }

    // (c_1 , c_2 , c_3) first group of out that will be input of 
    // first block AES
    out[0] = in[3]; 
    out[1] = in[4]; 
    out[2] = in[5];

    // (b_1 , b_2 , c_1 + c_2 * b_1 , c_3 + c_2 * b_2) second 
    // group of out that will be input of second block AES
    out[3] = in[1]; 
    out[4] = in[2]; 
    out[5] = c_1__p__c_2__b_1; out[6] = c_3__p__c_2__b_2;

    // (a_1 , b_2 + b_1 * a_1 , (c_3 + c_2 * b_2) + (c_1 + c_2 * b_1) * a_1 ) 
    // third group of out that will be input of third block AES
    out[7] = in[0]; 
    out[8] = b_2__p__b_1__a_1; 
    out[9] = c_3__p__c_2__b_2 + c_1__p__c_2__b_1 * in[0];
}

static void POST_Process(const uint64_t *in, // input: supposed to be 7 times 64 bits
                         uint64_t *out)      // output buffer: will contain 3 times 64 bits
{
    uint64_t y_1__t__y_2;
    uint64_t y_1__t__y_3;
    uint64_t y_2__t__y_3;

    //TODO: find definition of the matrix
    // the multiplications
#pragma omp parallel sections
    {
#pragma omp section
        y_1__t__y_2 = in[0] * in[1];
#pragma omp section
        y_1__t__y_3 = in[0] * in[2];
#pragma omp section
        y_2__t__y_3 = in[1] * in[2];
    }

    // apply the matrix
}

void Compression_Func(const uint64_t *in,  // input of compression function (6 * 64 bits)
                      uint64_t *out)       // output buffer (3 * 64 bits)
{
    uint64_t PRE_out [PRE_OUT_LEN];

    uint64_t AES_in_1 [2];
    uint64_t AES_in_2 [2];
    uint64_t AES_in_3 [2];

    uint64_t AES_out_1 [2];
    uint64_t AES_out_2 [2];
    uint64_t AES_out_3 [2];

    __mm128i key_1 [10];
    __mm128i key_2 [12];
    __mm128i key_3 [10];

    // PRE Processing

    PRE_Process((const uint64_t *)in , PRE_out);

    // Between C^PRE and C^POST: the actual use of three block ciphers (AES) in parallel

    AES_in_1[0] = PRE_out[0]; AES_in_1[1] = 0;
    AES_in_2[0] = PRE_out[3]; AES_in_2[1] = 0;
    AES_in_3[0] = PRE_out[7]; AES_in_3[1] = 0;

#pragma omp parallel sections 
    {
#pragma omp section
        {
            AES_128_Key_Expansion((const unsigned char *) (PRE_out + 1) , 
                                  (unsigned char *)key_1);
            AES_128_Encrypt((const unsigned char *)AES_in_1 , 
                            (unsigned char *)AES_out_1 , 
                            (const unsigned char *)key_1);
        }
#pragma omp section
        {
            AES_192_Key_Expansion((const unsigned char *) (PRE_out + 4) , 
                                  (unsigned char *)key_2);
            AES_192_Encrypt((const unsigned char *)AES_in_2 , 
                            (unsigned char *)AES_out_2 , 
                            (const unsigned char *)key_2);
        }
#pragma omp section
        {
            AES_128_Key_Expansion((const unsigned char *) (PRE_out + 8) , 
                                  (unsigned char *)key_3);
            AES_128_Encrypt((const unsigned char *)AES_in_3 , 
                            (unsigned char *)AES_out_3 , 
                            (const unsigned char *)key_3);
        }
    }

    // POST Processing

}

void MerkleDamgard(const unsigned char *message,    // message to hash
                   const size_t mlen,               // size of the message to hash
                   unsigned char *hash)             // buffer for the hash value
{ 

}
*/

void main(void)
{
    printf("CPU supports AES: %s\n", Check_CPU_support_AES() ? "true":"false");
}
