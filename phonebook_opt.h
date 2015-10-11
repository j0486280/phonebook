#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define COMPRESS_LAST_NAME_SIZE 10
#define HASH_BUCKET_SIZE  39997
#define COMPRESS_METHOD
#define HASH_METHOD 3

/* original version */
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_DETAIL *_detail;
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

typedef struct __PHONE_BOOK_DETAIL {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} detail;

void compress(char *input,char *output);
entry *findName(char lastname[], entry *pHead[]);
void append(char lastName[], entry *e[]);

#endif
