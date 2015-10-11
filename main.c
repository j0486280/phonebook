#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define INPUT_SIZE 8
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
    PHONEBOOK.initialize();

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
        PHONEBOOK.addNode(line,e);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[INPUT_SIZE][MAX_LAST_NAME_SIZE] = {"uninvolved","zyxel","whiteshank",
                                                  "odontomous", "pungoteague",
                                                  "reweighted", "xiphisternal", "yakattalo"
                                                 };

    for( i = 0; i < INPUT_SIZE; i++ ) {

        char in[MAX_LAST_NAME_SIZE];
        PHONEBOOK.get_cmp_buf(input[i],in);
        PHONEBOOK.moveNode();

        /*Get compress input value*/
        assert(PHONEBOOK.findName(input[i], e) &&
               "Did you implement findName() in " IMPL "?");
        assert(0 == strcmp(PHONEBOOK.findName(input[i], e)->lastName, in));

#if defined(__GNUC__)
        __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
        /* compute the execution time */
        clock_gettime(CLOCK_REALTIME, &start);
        PHONEBOOK.findName(input[i], e);
        clock_gettime(CLOCK_REALTIME, &end);
        cpu_time2 = diff_in_second(start, end);
    }
    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    /* FIXME: release all allocated entries */
    PHONEBOOK.release();

    return 0;
}
