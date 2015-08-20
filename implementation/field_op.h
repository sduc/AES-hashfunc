#ifndef __FIELD_OP_H__
#define __FIELD_OP_H__

#include <wmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include "config.h"
#include <stdio.h>

/*define F64_POLY x^46 + x^4 + x^3 + x^2 + 1*/

#define _F_add_(a,b) a^b

#define _F64_mult_(a,b) F64_mult_efficient(a,b)

inline uint64_t F64_mult_efficient(uint64_t a, uint64_t b)
{
    __m128i tmp1, a128, b128;
    uint64_t tmp2,tmp3,tmp4,tmp5;

    a128 = _mm_set_epi64x(0,a);
    b128 = _mm_set_epi64x(0,b);
    
    tmp1 = _mm_clmulepi64_si128(a128,b128,0x00);

    tmp2 = _mm_extract_epi64(tmp1,1);

    tmp3 = tmp2 >> 60;
    tmp4 = tmp2 >> 61;
    tmp5 = tmp2 >> 62;

    tmp2 = tmp2 ^ tmp3 ^ tmp4 ^ tmp5;

    tmp3 = tmp2 << 4;
    tmp4 = tmp2 << 3;
    tmp5 = tmp2 << 2;

    tmp2 = tmp2 ^ tmp3 ^ tmp4 ^ tmp5;

    tmp3 = _mm_extract_epi64(tmp1,0);

    return tmp3 ^ tmp2;
}

inline void F64_mult_PARA_4(
        uint64_t a1, uint64_t b1, uint64_t *c1,
        uint64_t a2, uint64_t b2, uint64_t *c2,
        uint64_t a3, uint64_t b3, uint64_t *c3,
        uint64_t a4, uint64_t b4, uint64_t *c4)
{
    __m128i tmp1_1, a128_1, b128_1;
    __m128i tmp1_2, a128_2, b128_2;
    __m128i tmp1_3, a128_3, b128_3;
    __m128i tmp1_4, a128_4, b128_4;
    uint64_t tmp2_1,tmp3_1,tmp4_1,tmp5_1;
    uint64_t tmp2_2,tmp3_2,tmp4_2,tmp5_2;
    uint64_t tmp2_3,tmp3_3,tmp4_3,tmp5_3;
    uint64_t tmp2_4,tmp3_4,tmp4_4,tmp5_4;

    a128_1 = _mm_set_epi64x(0,a1);
    b128_1 = _mm_set_epi64x(0,b1);
    a128_2 = _mm_set_epi64x(0,a2);
    b128_2 = _mm_set_epi64x(0,b2);
    a128_3 = _mm_set_epi64x(0,a3);
    b128_3 = _mm_set_epi64x(0,b3);
    a128_4 = _mm_set_epi64x(0,a4);
    b128_4 = _mm_set_epi64x(0,b4);
    
    tmp1_1 = _mm_clmulepi64_si128(a128_1,b128_1,0x00);
    tmp1_2 = _mm_clmulepi64_si128(a128_2,b128_2,0x00);
    tmp1_3 = _mm_clmulepi64_si128(a128_3,b128_3,0x00);
    tmp1_4 = _mm_clmulepi64_si128(a128_4,b128_4,0x00);

    tmp2_1 = _mm_extract_epi64(tmp1_1,1);
    tmp2_2 = _mm_extract_epi64(tmp1_2,1);
    tmp2_3 = _mm_extract_epi64(tmp1_3,1);
    tmp2_4 = _mm_extract_epi64(tmp1_4,1);

    tmp3_1 = tmp2_1 >> 60;
    tmp3_2 = tmp2_2 >> 60;
    tmp3_3 = tmp2_3 >> 60;
    tmp3_4 = tmp2_4 >> 60;
    tmp4_1 = tmp2_1 >> 61;
    tmp4_2 = tmp2_2 >> 61;
    tmp4_3 = tmp2_3 >> 61;
    tmp4_4 = tmp2_4 >> 61;
    tmp5_1 = tmp2_1 >> 62;
    tmp5_2 = tmp2_2 >> 62;
    tmp5_3 = tmp2_3 >> 62;
    tmp5_4 = tmp2_4 >> 62;

    tmp2_1 = tmp2_1 ^ tmp3_1 ^ tmp4_1 ^ tmp5_1;
    tmp2_2 = tmp2_2 ^ tmp3_2 ^ tmp4_2 ^ tmp5_2;
    tmp2_3 = tmp2_3 ^ tmp3_3 ^ tmp4_3 ^ tmp5_3;
    tmp2_4 = tmp2_4 ^ tmp3_4 ^ tmp4_4 ^ tmp5_4;

    tmp3_1 = tmp2_1 << 4;
    tmp3_2 = tmp2_2 << 4;
    tmp3_3 = tmp2_3 << 4;
    tmp3_4 = tmp2_4 << 4;
    tmp4_1 = tmp2_1 << 3;
    tmp4_2 = tmp2_2 << 3;
    tmp4_3 = tmp2_3 << 3;
    tmp4_4 = tmp2_4 << 3;
    tmp5_1 = tmp2_1 << 2;
    tmp5_2 = tmp2_2 << 2;
    tmp5_3 = tmp2_3 << 2;
    tmp5_4 = tmp2_4 << 2;

    tmp2_1 = tmp2_1 ^ tmp3_1 ^ tmp4_1 ^ tmp5_1;
    tmp2_2 = tmp2_2 ^ tmp3_2 ^ tmp4_2 ^ tmp5_2;
    tmp2_3 = tmp2_3 ^ tmp3_3 ^ tmp4_3 ^ tmp5_3;
    tmp2_4 = tmp2_4 ^ tmp3_4 ^ tmp4_4 ^ tmp5_4;

    tmp3_1 = _mm_extract_epi64(tmp1_1,0);
    tmp3_2 = _mm_extract_epi64(tmp1_2,0);
    tmp3_3 = _mm_extract_epi64(tmp1_3,0);
    tmp3_4 = _mm_extract_epi64(tmp1_4,0);

    *c1 = tmp3_1 ^ tmp2_1;
    *c2 = tmp3_2 ^ tmp2_2;
    *c3 = tmp3_3 ^ tmp2_3;
    *c4 = tmp3_4 ^ tmp2_4;
}

inline void F64_mult_PARA_3(
        uint64_t a1, uint64_t b1, uint64_t *c1,
        uint64_t a2, uint64_t b2, uint64_t *c2,
        uint64_t a3, uint64_t b3, uint64_t *c3)
{
    __m128i tmp1_1, a128_1, b128_1;
    __m128i tmp1_2, a128_2, b128_2;
    __m128i tmp1_3, a128_3, b128_3;
    uint64_t tmp2_1,tmp3_1,tmp4_1,tmp5_1;
    uint64_t tmp2_2,tmp3_2,tmp4_2,tmp5_2;
    uint64_t tmp2_3,tmp3_3,tmp4_3,tmp5_3;

    a128_1 = _mm_set_epi64x(0,a1);
    b128_1 = _mm_set_epi64x(0,b1);
    a128_2 = _mm_set_epi64x(0,a2);
    b128_2 = _mm_set_epi64x(0,b2);
    a128_3 = _mm_set_epi64x(0,a3);
    b128_3 = _mm_set_epi64x(0,b3);
    
    tmp1_1 = _mm_clmulepi64_si128(a128_1,b128_1,0x00);
    tmp1_2 = _mm_clmulepi64_si128(a128_2,b128_2,0x00);
    tmp1_3 = _mm_clmulepi64_si128(a128_3,b128_3,0x00);

    tmp2_1 = _mm_extract_epi64(tmp1_1,1);
    tmp2_2 = _mm_extract_epi64(tmp1_2,1);
    tmp2_3 = _mm_extract_epi64(tmp1_3,1);

    tmp3_1 = tmp2_1 >> 60;
    tmp3_2 = tmp2_2 >> 60;
    tmp3_3 = tmp2_3 >> 60;
    tmp4_1 = tmp2_1 >> 61;
    tmp4_2 = tmp2_2 >> 61;
    tmp4_3 = tmp2_3 >> 61;
    tmp5_1 = tmp2_1 >> 62;
    tmp5_2 = tmp2_2 >> 62;
    tmp5_3 = tmp2_3 >> 62;

    tmp2_1 = tmp2_1 ^ tmp3_1 ^ tmp4_1 ^ tmp5_1;
    tmp2_2 = tmp2_2 ^ tmp3_2 ^ tmp4_2 ^ tmp5_2;
    tmp2_3 = tmp2_3 ^ tmp3_3 ^ tmp4_3 ^ tmp5_3;

    tmp3_1 = tmp2_1 << 4;
    tmp3_2 = tmp2_2 << 4;
    tmp3_3 = tmp2_3 << 4;
    tmp4_1 = tmp2_1 << 3;
    tmp4_2 = tmp2_2 << 3;
    tmp4_3 = tmp2_3 << 3;
    tmp5_1 = tmp2_1 << 2;
    tmp5_2 = tmp2_2 << 2;
    tmp5_3 = tmp2_3 << 2;

    tmp2_1 = tmp2_1 ^ tmp3_1 ^ tmp4_1 ^ tmp5_1;
    tmp2_2 = tmp2_2 ^ tmp3_2 ^ tmp4_2 ^ tmp5_2;
    tmp2_3 = tmp2_3 ^ tmp3_3 ^ tmp4_3 ^ tmp5_3;

    tmp3_1 = _mm_extract_epi64(tmp1_1,0);
    tmp3_2 = _mm_extract_epi64(tmp1_2,0);
    tmp3_3 = _mm_extract_epi64(tmp1_3,0);

    *c1 = tmp3_1 ^ tmp2_1;
    *c2 = tmp3_2 ^ tmp2_2;
    *c3 = tmp3_3 ^ tmp2_3;
}
#endif//__FIELD_OP_H__
