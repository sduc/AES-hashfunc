#include <wmmintrin.h>
#include "aes.h"

void AES_Encrypt(const unsigned char *in, unsigned char *out, const char *key, int round_num)
{
    __m128i tmp;
    int i;

    tmp = _mm_loadu_si128 ((__m128i*)in);
    tmp = _mm_xor_si128 (tmp, ((__m128i*)key)[0]);
    for(i = 1; i < round_num; ++i){
        tmp = _mm_aesenc_si128 (tmp,((__m128i*)key)[i]); 
    }
    tmp = _mm_aesenclast_si128 (tmp,((__m128i*)key)[i]); 
    _mm_storeu_si128 ((__m128i*)out,tmp); 
}

void AES_Encrypt_PARA_3(const unsigned char *in1, const unsigned char *in2, const unsigned char *in3,
        unsigned char *out1, unsigned char *out2, unsigned char *out3,
        const char *key1, const char *key2, const char *key3)
{
    __m128i tmp1;
    __m128i tmp2;
    __m128i tmp3;
    int i;

    tmp1 = _mm_loadu_si128 ((__m128i*)in1);
    tmp2 = _mm_loadu_si128 ((__m128i*)in2);
    tmp3 = _mm_loadu_si128 ((__m128i*)in3);
    tmp1 = _mm_xor_si128 (tmp1, ((__m128i*)key1)[0]);
    tmp2 = _mm_xor_si128 (tmp2, ((__m128i*)key2)[0]);
    tmp3 = _mm_xor_si128 (tmp3, ((__m128i*)key3)[0]);
    for(i = 1; i < 10; ++i){
        tmp1 = _mm_aesenc_si128 (tmp1,((__m128i*)key1)[i]); 
        tmp2 = _mm_aesenc_si128 (tmp2,((__m128i*)key2)[i]); 
        tmp3 = _mm_aesenc_si128 (tmp3,((__m128i*)key3)[i]); 
    }
    tmp2 = _mm_aesenc_si128 (tmp2,((__m128i*)key2)[10]); 
    tmp2 = _mm_aesenc_si128 (tmp2,((__m128i*)key2)[11]); 

    tmp1 = _mm_aesenclast_si128 (tmp1,((__m128i*)key1)[i]); 
    tmp2 = _mm_aesenclast_si128 (tmp2,((__m128i*)key2)[12]); 
    tmp3 = _mm_aesenclast_si128 (tmp3,((__m128i*)key3)[i]); 

    _mm_storeu_si128 ((__m128i*)out1,tmp1); 
    _mm_storeu_si128 ((__m128i*)out2,tmp2); 
    _mm_storeu_si128 ((__m128i*)out3,tmp3); 
}

void AES_Decrypt(const unsigned char *in, unsigned char *out, const char *key, int round_num)
{
    __m128i tmp; 
    int i; 

    tmp = _mm_loadu_si128 ((__m128i*)in); 
    tmp = _mm_xor_si128 (tmp,((__m128i*)key)[0]);    
    for(i=1; i < round_num; i++){ 
        tmp = _mm_aesdec_si128 (tmp,((__m128i*)key)[i]); 
    } 
    tmp = _mm_aesdeclast_si128 (tmp,((__m128i*)key)[i]); 
    _mm_storeu_si128 ((__m128i*)out,tmp); 
}
