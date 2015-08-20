/**
 * Author: Sebastien Duc
 *
 * Hash function using Merkle-Damgard Scheme with a block cipher based compression function
 * The block cipher used is AES.
 *
 * The Hash function hash to a domaine of size 192 bits
 *
 */

#ifndef __HASH_H__
#define __HASH_H__
#include <string.h>
#include "config.h"

#define HASH_LEN 24 //in bytes

/**
 * Function used to hash a message.
 *
 * @param msg message to hash
 * @param mlen length of msg in bytes
 * @param hash buffer that will contain the hash of 24 bytes
 * 
 * @return 0 if everything went correctly, 1 if someting went wrong 
 */
int merkle_damgard(const uint8_t *msg,
                   const size_t mlen,
                   uint8_t *hash);

#define hash(msg,mlen,hash) merkle_damgard(msg,mlen,hash)

#endif//__HASH_H__
