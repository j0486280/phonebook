#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_orig.h"

/* original version */
entry *findName(char lastname[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

void initialize()
{
    pHead = (entry *) malloc(sizeof(entry));
    e = pHead;
    pHead->pNext = NULL;
}

void release()
{
    free(pHead);
}

void moveNode()
{
    e = pHead;
}

void get_cmp_buf( char *input, char *output )
{
    strcpy(output,input);
}

void addNode( char lastName[], entry *node )
{
    e = append(lastName,node);
}

struct PHONEBOOK PHONEBOOK = {
    .initialize = initialize,
    .release = release,
    .moveNode = moveNode,
    .get_cmp_buf = get_cmp_buf,
    .findName = findName,
    .addNode = addNode
};
