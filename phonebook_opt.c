#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "phonebook_opt.h"

static int ASCII_ENG_CHAR_START = 97 - 1;
static int COMPRESS_CHAR_BITS  = 5 - 1;
static int ORIGINAL_CHAR_BITS = 8 - 1;

static inline unsigned long hash (char* lastName)
{
    unsigned long key = 0;
    int length = strlen( lastName );
    int i;

#ifdef HASH_FUNCTION_DJB2
    key = 5381;
    for(i = 0 ; i < length ; i++ )
        key = ( (key << 5 ) + key ) + lastName[i];
#endif

#ifdef HASH_FUNCTION_TEST
    for(i = 0 ; i < length ; i++ )
        key += ( lastName[i] << i );
#endif

#ifdef HASH_FUNCTION_ONEBUCKET
    key = 0;
#endif

#ifdef HASH_FUNCTION_LOSE
    for(i = 0 ; i < length ; i++ )
        key += lastName[i];
#endif

#ifdef HASH_FUNCTION_SDBM
    for(i = 0 ; i < length ; i++ )
        key = ( ( key<<6 ) + ( key << 16 ) ) - key + lastName[i];
#endif

#ifdef HASH_FUNCTION_SINGHTLY
    for(i = 0 ; i < length ; i++ )
        key = 31 * key + lastName[i];
#endif

    return key;
}

static inline void compress(char *input,char *output)
{
    bool b[80];
    int i ,j  ,t0;
    int input_size = strlen(input);
    char c = 0;
    int count = 0;
    int index = 0;
    int len = ( input_size << 2 ) + input_size;

    for( i = 0 ; i < input_size ; i++ ) {
        if( input[i] == '\0' ) break;
        t0 = input[i] - ASCII_ENG_CHAR_START ;
        for(j = COMPRESS_CHAR_BITS ; j >= 0 ; j-- )
            b[count++] = t0 & (1<< j ) ;
    }

    count = ORIGINAL_CHAR_BITS;

    for( i = 0; i < len ; i++ ) {
        c += b[i] << (count--);
        if( count < 0 ) {
            output[index++] = c;
            c = 0;
            count = ORIGINAL_CHAR_BITS;
        }
    }
    if( count != ORIGINAL_CHAR_BITS ) output[index++] = c;
    output[index] = '\0';
}

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastname[], entry *pHead[])
{
    char cmp_buffer[COMPRESS_LAST_NAME_SIZE];
    compress(lastname,cmp_buffer);
    unsigned int key=hash(lastname)%HASH_BUCKET_SIZE;
    entry *node=pHead[key];
    while (node != NULL) {
        if (strcasecmp(cmp_buffer, node->lastName) == 0)
            return node;
        node = node->pNext;
    }
    return NULL;
}

void append(char lastName[], entry *e[])
{
    char cmp_buffer[COMPRESS_LAST_NAME_SIZE];
    compress(lastName,cmp_buffer);
    unsigned int key=hash(lastName)%HASH_BUCKET_SIZE;

    /* allocate memory for the new entry and put lastName */
    e[key]->pNext = (entry *) malloc(sizeof(entry));
    e[key] = e[key]->pNext;
    strcpy(e[key]->lastName, cmp_buffer);
    e[key]->pNext = NULL;
}

void initialize()
{
    int i;
    for(i=0; i<HASH_BUCKET_SIZE; i++) {
        pHead[i]= (entry *) malloc(sizeof(entry));
        pHead[i]->pNext=NULL;
        e[i]=pHead[i];
    }
}

void release()
{
    int i;
    for(i=0; i<HASH_BUCKET_SIZE; i++)
        free(pHead[i]);
}

void moveNode()
{
    int i;
    for(i=0; i<HASH_BUCKET_SIZE; i++)
        e[i]=pHead[i];
}

void get_cmp_buf(char *input, char *output)
{
    compress(input,output);
}

void addNode(char lastName[], entry *node[])
{
    append(lastName,node);
}

struct PHONEBOOK PHONEBOOK = {
    .initialize = initialize,
    .release = release,
    .moveNode = moveNode,
    .get_cmp_buf = get_cmp_buf,
    .findName = findName,
    .addNode = addNode
};
