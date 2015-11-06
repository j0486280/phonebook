#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define COMPRESS_LAST_NAME_SIZE 10
#define HASH_BUCKET_SIZE  260000
#define STRING_COMPARISON_THRESHOLD 0.7
#define STRING_COMPARISON_LENGTH 3
#define COMPRESS_METHOD
#define hashFunc hash_Soundex
#define FUZZY_SEARCH
#define SHOW_RESULT

typedef struct __PHONE_BOOK_ENTRY {
    char lastName[COMPRESS_LAST_NAME_SIZE];
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

struct PHONEBOOK {
    void (*initialize)(void);
    void (*release)(void);
    void (*moveNode)(void);
    void (*get_cmp_buf)(char *input, char *output);
    void (*addNode)(char lastName[], entry *node[]);
    entry* (*findName)(char lastname[], entry *pHead[]);
};

void decompress(char *input,char *output);
int Levenshtein(char *a,char *b);
entry *pHead[HASH_BUCKET_SIZE];
entry *e[HASH_BUCKET_SIZE];
struct PHONEBOOK PHONEBOOK;

#endif
