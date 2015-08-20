#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "hash.h"

#define MAX_MSG_LEN 18446744073709551615
#define BLOCK 100

int main(int argc, const char **argv)
{
    char *msg = NULL;
    size_t mlen = 0;

    if (getline(&msg,&mlen,stdin)<=0){
        fprintf(stderr,"failed to read msg\n");
        return EXIT_FAILURE;
    }

    char hashval[HASH_LEN+1];
    hash(msg,mlen,hashval);
    hashval[HASH_LEN] = '\0';
    printf("%s\n",hashval);
    
    free(msg);
}
