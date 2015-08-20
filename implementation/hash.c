/**
 * Author: Sebastien Duc
 *
 * Implementation of a hash function using a block-cipher based compression function in a Merkle-Damgard Scheme.
 * The block cipher used in the compression function is AES.
 *
 * Some usefull conventions:
 * - a variable called   a_b  contains the multiplication of a * b
 * - a variable called   a__b  contains the sum of a + b
 * - the prefix pc in pc_varname means that the variable has something to do 
 *   with some precomputation related to parallelization
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aes.h>
#include <wmmintrin.h>

#include "config.h"
#include "hash.h"
#include "field_op.h"

#include <assert.h>

#define PRE_IN_LEN 6
#define PRE_OUT_LEN 10
#define POST_IN_LEN 7
#define POST_OUT_LEN 3
#define PC_DATA_LEN 3
#define BLOCK_LEN_B64 3

static uint64_t IV [BLOCK_LEN_B64] = {0ull,0ull,0ull};

#define HASH_BLOCK_BYTE_LEN 24 // size in bytes of a block

#define PAD_SIZE_BYTE_LEN 8 // size in bytes to encode the length in the padding

// returns 1 if everything went correctly, 0 if there was an error
inline int padding(const uint8_t *msg, // message
                   const size_t mlen,  // size of the message, at most 2^64 - 1
                   uint8_t **pmsg,     // buffer for the padded message
                   size_t *pmlen)      // size of the padded message
{
    if (!pmsg) {
        fprintf(stderr,"pmsg null pointer exception\n");
        return 0;
    }

    uint8_t *tmp_pmsg;
    
    // determine the padding length
    int padd_len = HASH_BLOCK_BYTE_LEN - (mlen % HASH_BLOCK_BYTE_LEN);
    if (padd_len <= PAD_SIZE_BYTE_LEN) padd_len += HASH_BLOCK_BYTE_LEN;

    // allocate memory for the padded msg
    *pmsg = (uint8_t *) calloc (mlen + padd_len, sizeof(uint8_t));
    if(!(*pmsg)) {
        fprintf(stderr,"error: could not allocate memory\n");
        return 0;
    }

    // FILL THE PADDED MSG
    // the padded msg is : msg || 1 0 ... 0 || mlen
    //                     mlen   1             64
    memcpy(*pmsg, msg, mlen);
    tmp_pmsg = *pmsg + mlen;
    memset(tmp_pmsg, 0, padd_len);

    *tmp_pmsg = 0x80; // put 1 (bit) just after the message

    tmp_pmsg += padd_len - PAD_SIZE_BYTE_LEN;
    *((uint64_t *)tmp_pmsg) = (uint64_t) mlen;// put mlen at the end

    *pmlen = mlen + padd_len;// set the new size, of the padded msg

    return 1;
}

static inline void AES_Proc(const uint64_t *PRE_out,
                            const uint64_t *AES_in_1,
                            const uint64_t *AES_in_2,
                            const uint64_t *AES_in_3,
                            uint64_t *AES_out_1,
                            uint64_t *AES_out_2,
                            uint64_t *AES_out_3)
{
    __m128i key1 [11]; 
    __m128i key2 [13]; 
    __m128i key3 [11]; 

    AES_Key_Expansion_PARA_3(
            (const unsigned char *) (PRE_out + 1) ,
            (const unsigned char *) (PRE_out + 4) ,
            (const unsigned char *) (PRE_out + 8) ,
            (unsigned char *) key1 ,
            (unsigned char *) key2 ,
            (unsigned char *) key3 );

    AES_Encrypt_PARA_3(
            (const unsigned char *) AES_in_1 ,
            (const unsigned char *) AES_in_2 ,
            (const unsigned char *) AES_in_3 ,
            (unsigned char *) AES_out_1 ,
            (unsigned char *) AES_out_2 ,
            (unsigned char *) AES_out_3 ,
            (const unsigned char *) key1 ,
            (const unsigned char *) key2 ,
            (const unsigned char *) key3 );

}


/**
 * This function is the preprocessing for the first block. It will be a bit different
 * because of the parallelization, namely one multiplication is not done at the same time than the three other.
 */
inline static void FIRST_Pre_Proc(const uint64_t *in, // input: it is supposed to be 6 times 64 bits
                                                      // in = [a1,b1,b2,c1,c2,c3]
                                  uint64_t *out)      // output buffer: will contain 10 times 64 bits
{
    uint64_t c1__c2_b1;
    uint64_t c3__c2_b2;   
    uint64_t b2__b1_a1;

    // in = (a_1 , b_1 , b_2 , c_1 , c_2 , c_3)
    //                                              
    // some precomputations
    F64_mult_PARA_3(
            in[4],in[1],&c1__c2_b1,
            in[4],in[2],&c3__c2_b2,
            in[1],in[0],&b2__b1_a1);
    c1__c2_b1 = _F_add_(in[3],c1__c2_b1);
    c3__c2_b2 = _F_add_(in[5],c3__c2_b2);
    b2__b1_a1 = _F_add_(in[2],b2__b1_a1);

    // (c_1 , c_2 , c_3) first group of out that will be input of 
    // first block AES
    out[0] = in[3]; 
    out[1] = in[4]; 
    out[2] = in[5];

    // (b_1 , b_2 , c_1 + c_2 * b_1 , c_3 + c_2 * b_2) second 
    // group of out that will be input of second block AES
    out[3] = in[1]; 
    out[4] = in[2]; 
    out[5] = c1__c2_b1; out[6] = c3__c2_b2;

    // (a_1 , b_2 + b_1 * a_1 , (c_3 + c_2 * b_2) + (c_1 + c_2 * b_1) * a_1 ) 
    // third group of out that will be input of third block AES
    out[7] = in[0]; 
    out[8] = b2__b1_a1; 
    out[9] = _F_add_(c3__c2_b2 , _F64_mult_(c1__c2_b1 , in[0]));
}

inline static void STD_Pre_Proc(const uint64_t *in,             // input_ it is supposed to be 6 times 64 bits
                                                                // in = [a1,b1,b2,c1,c2,c3]
                                const uint64_t apc_dep_mult,    // precomputed multiplication c_1  + c_2 * b_1
                                uint64_t *out)                  // output buffer: will contain 10 times 64 bits
{
    uint64_t c1__c2_b1_a1;
    uint64_t c3__c2_b2;   
    uint64_t b2__b1_a1;    

    // in = (a_1 , b_1 , b_2 , c_1 , c_2 , c_3)
    //                                              
    // some precomputations
    F64_mult_PARA_3(
            apc_dep_mult,in[0],&c1__c2_b1_a1,
            in[4],in[2],&c3__c2_b2,
            in[1],in[0],&b2__b1_a1);
    c3__c2_b2 = _F_add_(in[5],c3__c2_b2);
    b2__b1_a1 = _F_add_(in[2],b2__b1_a1);

    // (c_1 , c_2 , c_3) first group of out that will be input of 
    // first block AES
    out[0] = in[3]; 
    out[1] = in[4]; 
    out[2] = in[5];

    // (b_1 , b_2 , c_1 + c_2 * b_1 , c_3 + c_2 * b_2) second 
    // group of out that will be input of second block AES
    out[3] = in[1]; 
    out[4] = in[2]; 
    out[5] = apc_dep_mult; out[6] = c3__c2_b2;

    // (a_1 , b_2 + b_1 * a_1 , (c_3 + c_2 * b_2) + (c_1 + c_2 * b_1) * a_1 ) 
    // third group of out that will be input of third block AES
    out[7] = in[0]; 
    out[8] = b2__b1_a1; 
    out[9] = _F_add_(c3__c2_b2 , c1__c2_b1_a1);
}

// in = [a1,b1,b2,c1,y1,y2,y3] , out = M*in , for matrix M
#define STD_Post_apply_matrix(in,y2_y3,y1_y3,y1_y2,out)\
    out[0] = _F_add_(in[1] , _F_add_(in[3] , _F_add_(in[4] , y2_y3)));\
    out[1] = _F_add_(in[0] , _F_add_(in[3] , _F_add_(in[5] , y1_y3)));\
    out[2] = _F_add_(in[0] , _F_add_(in[2] , _F_add_(in[6] , y1_y2)));


inline static void STD_Post_Proc(const uint64_t *in,        // input: supposed to be 7 times 64 bits
                                                            // consists of [a1,b1,b2,c1,y1,y2,y3]
                                 uint64_t *out,             // output buffer: will contain 3 times 64 bits
                                 const uint64_t *pc_data,   // data: for the precomputation; consits of b_1,c_1,c_2 in that order
                                 uint64_t *pc_dep_mult)     // precompute the next dependant mutliplication for 
                                                            // next preprocessing: c_1 + (b_1 * c_2)
{
    uint64_t y1_y2;
    uint64_t y1_y3;
    uint64_t y2_y3;

    // the multiplications
    F64_mult_PARA_4(
            in[0],in[1],&y1_y2,
            in[0],in[2],&y1_y3,
            in[1],in[2],&y2_y3,
            pc_data[0],pc_data[2],pc_dep_mult);
    *pc_dep_mult = _F_add_(*pc_dep_mult,pc_data[1]);

    // apply the matrix
    STD_Post_apply_matrix(in, y2_y3, y1_y3, y1_y2, out);

}

inline static void FIN_Post_Proc(const uint64_t *in,      // input: supposed to be 7 times 64 bits
                                                          // consists of [a1,b1,b2,c1,y1,y2,y3]
                                 uint64_t *out)           // output buffer: will contain 3 times 64 bits
{
    uint64_t y1_y2;
    uint64_t y1_y3;
    uint64_t y2_y3;

    // the multiplications
    F64_mult_PARA_3(
            in[0],in[1],&y1_y2,
            in[0],in[2],&y1_y3,
            in[1],in[2],&y2_y3);

    // apply the matrix
    STD_Post_apply_matrix(in, y2_y3, y1_y3, y1_y2, out);

}

#define FILL_AES_in(AES_in_1,AES_in_2,AES_in_3,PRE_out)\
    AES_in_1[0] = PRE_out[0]; AES_in_1[1] = 0;\
    AES_in_2[0] = PRE_out[3]; AES_in_2[1] = 0;\
    AES_in_3[0] = PRE_out[7]; AES_in_3[1] = 0;

#define FILL_POST_in(POST_in, a1, b1, b2, c1, y1, y2, y3)\
    POST_in[0] = a1;\
    POST_in[1] = b1;\
    POST_in[2] = b2;\
    POST_in[3] = c1;\
    POST_in[4] = y1;\
    POST_in[5] = y2;\
    POST_in[6] = y3;

static inline void FIRST_Step(const uint64_t *in,       // input of first step compr func (6 * 64 bits)
                                                        // in = [a1,b1,b2,c1,c2,c3]
                              uint64_t *out,            // output buffer (3 * 64 bits)
                              const uint64_t *pc_data,  // part of the next input of the next compr func call
                                                        // needed to precompute mult (b1,c1,c2)
                              uint64_t *pc_dep_mult)    // precomputed mult c1 + b1 * c2 
{
    uint64_t PRE_out [PRE_OUT_LEN];
    uint64_t AES_in_1 [2];
    uint64_t AES_in_2 [2];
    uint64_t AES_in_3 [2];
    uint64_t AES_out_1 [2];
    uint64_t AES_out_2 [2];
    uint64_t AES_out_3 [2];

    FIRST_Pre_Proc(in,PRE_out);

    FILL_AES_in(AES_in_1,AES_in_2,AES_in_3,PRE_out);
    
    AES_Proc(PRE_out, AES_in_1, AES_in_2, AES_in_3,
                      AES_out_1, AES_out_2, AES_out_3);
    //LOG_STRING(AES_out_1,16);
    
    uint64_t POST_in [POST_IN_LEN];
    FILL_POST_in(POST_in,
            in[0], in[1], in[2], in[3],
            AES_out_1[0], AES_out_2[0], AES_out_3[0]);// we only take the first 64 bits of AES
    STD_Post_Proc(POST_in, out, pc_data, pc_dep_mult);
}

static inline void STD_Step(const uint64_t *in,
                            uint64_t *out,
                            const uint64_t apc_mult,
                            const uint64_t *pc_data,
                            uint64_t *pc_dep_mult)
{
    uint64_t PRE_out [PRE_OUT_LEN];
    uint64_t AES_in_1 [2];
    uint64_t AES_in_2 [2];
    uint64_t AES_in_3 [2];
    uint64_t AES_out_1 [2];
    uint64_t AES_out_2 [2];
    uint64_t AES_out_3 [2];

    STD_Pre_Proc(in, apc_mult, PRE_out);
    
    FILL_AES_in(AES_in_1,AES_in_2,AES_in_3,PRE_out);
    
    AES_Proc(PRE_out, AES_in_1, AES_in_2, AES_in_3,
                      AES_out_1, AES_out_2, AES_out_3);
    
    uint64_t POST_in [POST_IN_LEN];
    FILL_POST_in(POST_in,
            in[0], in[1], in[2], in[3],
            AES_out_1[0], AES_out_2[0], AES_out_3[0]);// we only take the first 64 bits of AES
    STD_Post_Proc(POST_in, out, pc_data, pc_dep_mult);
}

static inline void FIN_Step(const uint64_t *in,
                            uint64_t *out,
                            const uint64_t apc_mult) // the already precomputed multiplication
{
    uint64_t PRE_out [PRE_OUT_LEN];
    uint64_t AES_in_1 [2];
    uint64_t AES_in_2 [2];
    uint64_t AES_in_3 [2];
    uint64_t AES_out_1 [2];
    uint64_t AES_out_2 [2];
    uint64_t AES_out_3 [2];

    STD_Pre_Proc(in, apc_mult, PRE_out);
    
    FILL_AES_in(AES_in_1,AES_in_2,AES_in_3,PRE_out);
    
    AES_Proc(PRE_out, AES_in_1, AES_in_2, AES_in_3,
                      AES_out_1, AES_out_2, AES_out_3);
    
    uint64_t POST_in [POST_IN_LEN];
    FILL_POST_in(POST_in,
            in[0], in[1], in[2], in[3],
            AES_out_1[0], AES_out_2[0], AES_out_3[0]);// we only take the first 64 bits of AES
    FIN_Post_Proc(POST_in, out);
}

// in: array to fill in; past_comp_func: output of the previous compression function; msg: block of the msg
// past_comp will fill  [a1,b2,c3]
// msg will fill [b1,c1,c2]
#define FILL_Comp_Func_in(in, past_comp, msg)\
    in[0] = past_comp[0]; in[2] = past_comp[1]; in[5] = past_comp[2];\
    in[1] = msg[0]; in[3] = msg[1]; in[4] = msg[2];

#define Set_hash(hash,out)\
    ((uint64_t *) hash)[0] = out[0];\
    ((uint64_t *) hash)[1] = out[1];\
    ((uint64_t *) hash)[2] = out[2];

int merkle_damgard(const uint8_t *msg,        // message to hash
                   const size_t mlen,         // size of the message to hash
                   uint8_t *hash)             // buffer for the hash value
{ 
    int i;
    // preliminaries: var initialization and padding
    uint64_t *tmp_msg = (uint64_t *) msg;
    uint8_t *pmsg;
    size_t pmlen;
    if (!(i = padding(msg,mlen,&pmsg,&pmlen))){
        fprintf(stderr,"error while padding the msg\n");
        return i;
    }

    size_t block_n = pmlen / HASH_BLOCK_BYTE_LEN; 

    uint64_t pc_dep_mult;
    uint64_t in [PRE_IN_LEN];
    uint64_t out [POST_OUT_LEN];

    FILL_Comp_Func_in(in,IV,tmp_msg);
    //first block msg // different
    FIRST_Step(in, out, tmp_msg + BLOCK_LEN_B64, &pc_dep_mult);

    // loop until end msg
    for(i = 1; i < block_n - 1; ++i, tmp_msg += 3){
        FILL_Comp_Func_in(in,out,tmp_msg);
        STD_Step(in, out, pc_dep_mult, tmp_msg + BLOCK_LEN_B64, &pc_dep_mult); 
    }
    // final step
    tmp_msg += 3;
    
    FILL_Comp_Func_in(in, out, tmp_msg);
    FIN_Step(in, out, pc_dep_mult);

    free(pmsg);

    Set_hash(hash, out);
    return 0;
}
