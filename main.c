#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */

#ifdef HASH_METHOD
    entry *pHead[HASH_BUCKET_SIZE];
    entry  *e[HASH_BUCKET_SIZE];
    for(i=0;i<HASH_BUCKET_SIZE;i++)
    {
        pHead[i]= (entry *) malloc(sizeof(entry));
       pHead[i]->pNext=NULL;
       e[i]=pHead[i];
    }
 #else
      entry *pHead, *e;
      pHead = (entry *) malloc(sizeof(entry));
      e = pHead;
      pHead->pNext = NULL;
#endif
      i=0;
    printf("size of entry : %lu bytes\n", sizeof(entry));
    

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#ifdef HASH_METHOD
        append(line, e);
#else
        e = append(line, e);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

#ifdef HASH_METHOD
        for(i=0;i<HASH_BUCKET_SIZE;i++)
            e[i]=pHead[i];
#else
        e = pHead;
#endif
   
    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "brushbush";
   
#ifdef HASH_METHOD
        for(i=0;i<HASH_BUCKET_SIZE;i++)
            e[i]=pHead[i];
#else
        e = pHead;
#endif

    /*Get compress input value*/
    char compression[MAX_LAST_NAME_SIZE];
    strcpy(compression,input);
#ifdef COMPRESS_METHOD
    compress(input,compression);
#endif
    assert(findName(input, pHead) &&
           "Did you implement findName() in " IMPL "?");  
    assert(0 == strcmp(findName(input, pHead)->lastName, compression));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    /* FIXME: release all allocated entries */
#ifdef HASH_METHOD
    for(i=0;i<HASH_BUCKET_SIZE;i++)
        free(pHead[i]);
#else
    free(pHead);
#endif
    return 0;
}