#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "phonebook_opt.h"

static int ASCII_ENG_CHAR_START = 97 - 1;
static int COMPRESS_CHAR_BITS  = 5 - 1;
static int ORIGINAL_CHAR_BITS = 8 - 1;
static int SOUNDEX_OUTPUT_LEN = 5;

static inline void Soundex (char* input, char* output)
{
    int i = 1,index = 1;
    output[0] = input[0];

    while(input[i] !='\0' && index < SOUNDEX_OUTPUT_LEN) {
        if( input[i] == 'b' || input[i] == 'p' || input[i] == 'f' ||input[i] == 'v' )
            output[index] = '1';
        else if( input[i] == 'c' || input[i] == 's' || input[i] == 'k'||
                 input[i] == 'g' || input[i] == 'j' || input[i] == 'q' ||
                 input[i] == 'x' || input[i] == 'z' )
            output[index] = '2';
        else if( input[i] == 'd' || input[i] == 't' )
            output[index] = '3';
        else if( input[i] == 'l' )
            output[index] = '4';
        else if( input[i] == 'm' || input[i] == 'n' )
            output[index] = '5';
        else if ( input[i] == 'r' )
            output[index] = '6';
        else
            output[index] = '0';
        if ( output[index-1] != output[index] )
            index++;
        i++;
    }
    while ( index < SOUNDEX_OUTPUT_LEN ) output[index++] = '0';
}

static inline long hash_Soundex(char *lastName)
{
    char output[SOUNDEX_OUTPUT_LEN] ;
    Soundex(lastName,output);
    unsigned long key = 0;
    key += (output[0] - 97)*10000;
    key += (output[1] - 48)*1000;
    key += (output[2] - 48)*100;
    key += (output[3] - 48)*10;
    key += (output[4] - 48);
    return key;
}

static inline unsigned long hash_djb2(char* lastName)
{
    unsigned long key = 5381;
    int length = strlen( lastName );
    int i;
    for(i = 0 ; i < length ; i++ )
        key = ( (key << 5 ) + key ) + lastName[i];
    return key;
}

static inline unsigned long hash_1bucket(char* lastName)
{
    return 0;
}

static inline unsigned long hash_lose(char* lastName)
{
    unsigned long key = 0;
    int length = strlen( lastName );
    int i;
    for(i = 0 ; i < length ; i++ )
        key += lastName[i];
    return key;
}

static inline unsigned long hash_sdbm(char* lastName)
{
    unsigned long key = 0;
    int length = strlen( lastName );
    int i;
    for(i = 0 ; i < length ; i++ )
        key = ( ( key<<6 ) + ( key << 16 ) ) - key + lastName[i];
    return key;
}

static inline unsigned long hash_singhtly(char* lastName)
{
    unsigned long key = 0;
    int length = strlen( lastName );
    int i;
    for(i = 0 ; i < length ; i++ )
        key = ( (key << 5 ) + key ) + lastName[i];
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

void decompress(char *input,char *output)
{
    bool b[80];
    int i ,j;
    int input_size = strlen(input);
    char c = 0;
    int count = 0;
    int index = 0;

    for( i = 0; i < input_size; i++ ) {
        if( input[i] == '\0' ) break;
        for( j = ORIGINAL_CHAR_BITS; j >= 0; j-- )
            b[count++] = input[i]&(1 << j);
    }

    i = 0;
    while(i < count) {
        for( j = COMPRESS_CHAR_BITS; j >= 0 && i < count; j-- )
            c += (b[i++] << j);
        if(c != 0)
            output[index++] = (c+ASCII_ENG_CHAR_START);
        c = 0;
    }
    output[index] = '\0';
}

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastname[], entry *pHead[])
{
    char cmp_buffer[COMPRESS_LAST_NAME_SIZE];
    compress(lastname,cmp_buffer);
    unsigned int key=hashFunc(lastname)%HASH_BUCKET_SIZE;
    entry *node=pHead[key];
    while (node != NULL) {
        if (strcasecmp(cmp_buffer, node->lastName) == 0)
            return node;
        node = node->pNext;
    }
    return pHead[key];
}

void append(char lastName[], entry *e[])
{
    char cmp_buffer[COMPRESS_LAST_NAME_SIZE];
    compress(lastName,cmp_buffer);
    unsigned int key=hashFunc(lastName)%HASH_BUCKET_SIZE;

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
