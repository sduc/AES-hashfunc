#ifndef __AES_H__
#define __AES_H__

#define AES_128_Encrypt(in,out,key) AES_Encrypt(in,out,key,10)
#define AES_192_Encrypt(in,out,key) AES_Encrypt(in,out,key,12)

#define AES_128_Decrypt(in,out,key) AES_Decrypt(in,out,key,10)
#define AES_192_Decrypt(in,out,key) AES_Decrypt(in,out,key,12)

void AES_128_Key_Expansion (const unsigned char *userkey, 
                                    unsigned char *key);
void AES_192_Key_Expansion (const unsigned char *userkey, 
                                    unsigned char *key);

void AES_Key_Expansion_PARA_3(
        const unsigned char *userkey1,
        const unsigned char *userkey2,
        const unsigned char *userkey3,
        unsigned char *key1,
        unsigned char *key2,
        unsigned char *key3);

void AES_Encrypt(const unsigned char *in,   // pointer to 128 bits PLAINTEXT block
                 unsigned char *out,        // pointer to 128 bits CIPHERTEXT buffer 
                 const char *key,           // pointer to expanded key schedule
                 int round_num);            // number of AES rounds 10,12 or 14

void AES_Encrypt_PARA_3(
        const unsigned char *in1, 
        const unsigned char *in2, 
        const unsigned char *in3,
        unsigned char *out1, 
        unsigned char *out2, 
        unsigned char *out3,
        const char *key1, 
        const char *key2, 
        const char *key3);

void AES_Decrypt(const unsigned char *in,   // pointer to 128 bits CIPHERTEXT block
                 unsigned char *out,        // pointer to 128 bits DECRYPTEDTEXT buffer
                 const char *key,           // pointer to expanded key schedule
                 int round_num);            // number of AES rounds 10,12 or 14

#endif/*__AES_H__*/
