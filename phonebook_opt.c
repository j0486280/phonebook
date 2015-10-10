#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "phonebook_opt.h"

static void encrypt(char *input,char *output)
{
    bool b[80];
    int i ,j  ,t0;
    int input_size=strlen(input);
    char c=0;
    int count=0;
    int index=0;
    int len=input_size*5;

    for(i=0; i<input_size; i++) {
        if(input[i]=='\0') break;
        t0=input[i]-96;
        for(j=0; j<5; j++)
            b[count++]=t0&(1<<j);
    }
    count=7;
    for(i=0; i<len; i++) {
        c+=b[i]<<(count--);
        if(count<0) {
            output[index++]=c;
            c=0;
            count=7;
        }
    }
    if(count!=7) output[index++]=c;
    output[index]='\0';
}

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastname[], entry *pHead)
{
    char _encrypt[LIMIT_LAST_NAME_SIZE];
    encrypt(lastname,_encrypt);
    while (pHead != NULL) {
        if (strcasecmp(_encrypt, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    char _encrypt[LIMIT_LAST_NAME_SIZE];
    encrypt(lastName,_encrypt);
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, _encrypt);
    e->pNext = NULL;

    return e;
}
