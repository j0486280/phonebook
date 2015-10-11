#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "phonebook_opt.h"

int bucket_count[HASH_BUCKET_SIZE]={0};
int ASCII_ENG_CHAR_START = 97 - 1;
int COMPRESS_CHAR_BITS  = 5 - 1;
int ORIGINAL_CHAR_BITS = 8 - 1;

 unsigned long hash (char* lastName)
{
    unsigned long key = 0;
    int length = strlen( lastName );
     int i;
    switch(HASH_METHOD)
    {
        case 0:
             key = 5381;
            for(i = 0 ; i < length ; i++ )
                key = ( (key << 5 ) + key ) + lastName[i]; /* hash * 33 + c */
        break;
        case 1:
            for(i = 0 ; i < length ; i++ )
                key += ( lastName[i] << i );
        break;
        case 2:
            key=0;
        break;
        case 3:
             for(i = 0 ; i < length ; i++ )
                key += lastName[i];
        break;
        case 4:
            for(i = 0 ; i < length ; i++ )
                key = ( ( key<<6 ) + ( key << 16 ) ) - key + lastName[i];
        break;
    }
    return key;
}

void compress(char *input,char *output)
{
    bool b[80];
    int i ,j  ,t0;
    int input_size = strlen(input);
    char c = 0;
    int count = 0;
    int index = 0;
    int len = ( input_size << 2 ) + input_size;

    for( i = 0 ; i < input_size ; i++ )
    {
        if( input[i] == '\0' ) break;
        t0 = input[i] - ASCII_ENG_CHAR_START ;
        for(j = COMPRESS_CHAR_BITS ; j >= 0 ; j-- )
            b[count++] = t0 & (1<< j ) ;
    }

    count = ORIGINAL_CHAR_BITS;
    
    for( i = 0; i < len ; i++ )
    {
        c += b[i] << (count--);
        if( count < 0 )
        {
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
    int count = 0;
    char cmp_buffer[COMPRESS_LAST_NAME_SIZE];
    compress(lastname,cmp_buffer);
    unsigned int key=hash(lastname)%HASH_BUCKET_SIZE;
    entry *node=pHead[key];
    while (node != NULL) {
        count++;
        if (strcasecmp(cmp_buffer, node->lastName) == 0)
        {
            printf("Find %s is search by %d times\n",lastname,count);
            return node;
        }
        node = node->pNext;
    }
    printf("%s is not found",lastname);
    return NULL;
}

void printCount()
{
    for(int i=0;i<HASH_BUCKET_SIZE;i++)
        printf("%d\t",bucket_count[i]);
}

void append(char lastName[], entry *e[])
{
    char cmp_buffer[COMPRESS_LAST_NAME_SIZE];
    compress(lastName,cmp_buffer);
    unsigned int key=hash(lastName)%HASH_BUCKET_SIZE;
    bucket_count[key]++;
    
    /* allocate memory for the new entry and put lastName */
    entry *node = (entry *)malloc(sizeof(entry));
    node -> pNext = e[key]->pNext;
    e[key] -> pNext = node;
    strcpy( node->lastName,cmp_buffer );
}