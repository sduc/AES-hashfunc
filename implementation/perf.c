/**
 * Author: Sebastien Duc
 *
 * Performance testing of the blockcipher based hash function
 * Function parallelized over for INTEL Westmere CPU using AES instruction set.
 *
 * To test it we will simply generate a random msg of MSG_BYTE_LEN bytes to be hashed.
 * We compute the number of cycles needed to hash this message.
 * We repeat the process REPEAT times.
 * We print the average cycles per byte needed to hash the message.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wmmintrin.h>

#include "config.h"
#include "hash.h"
#include "perf.h"
#include "aes/aes.h"

#define MSG_BYTE_LEN 100000
#define RAND_STREAM "/dev/urandom"

inline uint64_t get_Clks(void) { 
    uint64_t tmp; 
    __asm__ volatile( 
        "rdtsc\n\t\
        mov %%eax,(%0)\n\t\
        mov %%edx,4(%0)"::"rm"(&tmp):"eax","edx"); 
    return tmp; 
} 

inline static void get_rand_bytes(unsigned char *bytes, size_t len)
{
    if (!bytes)
        return;

    FILE *fp = fopen(RAND_STREAM,"rb");
    if (!fp)
        exit(EXIT_FAILURE);

    fread(bytes,1,len,fp);
    fclose(fp);
}

inline static void perf_aes(void)
{
    __m128i key128 [11];
    __m128i key192 [13];
    char uk [24];
    char in [16];
    char out [16];
    get_rand_bytes(uk,24);
    get_rand_bytes(in,16);

    printf("AES128 key schedule:\n");
    MEASURE(AES_128_Key_Expansion(uk,(unsigned char *)key128));
    printf("%f\n",total_clk);
    
    printf("AES192 key schedule:\n");
    MEASURE(AES_192_Key_Expansion(uk,(unsigned char *)key192));
    printf("%f\n",total_clk);

    printf("AES128 encrypt:\n");
    MEASURE(AES_128_Encrypt(in,out,(unsigned char *)key128));
    printf("%f\n",total_clk);

    printf("AES192 encrypt:\n");
    MEASURE(AES_192_Encrypt(in,out,(unsigned char *)key192));
    printf("%f\n",total_clk);
}

inline static void perf_testing(size_t n_bytes)
{
    unsigned char *msg = calloc (n_bytes,sizeof(unsigned char));
    char hashval[HASH_LEN];

    if(!msg)
    	exit(EXIT_FAILURE);
    
    get_rand_bytes(msg,n_bytes);
    
    MEASURE(hash(msg,n_bytes,hashval));

    printf("%d:%f\n",n_bytes,total_clk/n_bytes);

    free(msg);
}

int main(int argc, const char **argv)
{
    if (argc < 2){
    	fprintf(stderr,"usage: perf n_bytes [full]\n");
	return EXIT_FAILURE;
    }
    int nbytes = atoi(argv[1]);
    if (argc > 2 && strncmp(argv[2],"full",4) == 0){
        perf_aes();
    }
    perf_testing(nbytes);
}
