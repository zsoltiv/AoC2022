#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

int priority(char c)
{
    return islower(c) ? c - 96 : c - 38;
}

char *common(char *a, char *b)
{
    int commoncount = 0;
    for(char *aptr = a; *aptr; aptr++) { for(char *bptr = b; *bptr; bptr++) {
            if(*aptr == *bptr) {
                commoncount++;
            }
        }
    }

    char *commonchars = calloc(commoncount + 1, 1);
    int i = 0;
    for(char *aptr = a; *aptr; aptr++) {
        for(char *bptr = b; *bptr; bptr++) {
            if(*aptr == *bptr)
                commonchars[i++] = *aptr;
        }
    }

    return commonchars;
}

typedef char *group[3];

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    fseek(fp, 0, SEEK_END);
    size_t bufsize = ftell(fp) + 1;
    fseek(fp, 0, SEEK_SET);
    char *buf = calloc(bufsize, 1);
    fread(buf, 1, bufsize - 1, fp);
    fclose(fp);

    char delim[] = "\n";
    char *rucksack = strtok(buf, delim);
    int last = 0;
    int i = 0;
    group *groups = malloc(sizeof(group));
    while(rucksack) {
        if(i == 3) {
            i = 0;
            last++;
            groups = realloc(groups, sizeof(group) * (last + 1));
        }
        groups[last][i++] = strdup(rucksack);
        rucksack = strtok(NULL, delim);
    }

    int *badges = malloc(sizeof(int) * (last + 1));
    for(int i = 0; i <= last; i++) {
        char *commonchars = common(groups[i][0], groups[i][1]);
        char *morecommonchars = common(commonchars, groups[i][2]);
        badges[i] = priority(*morecommonchars);
        free(commonchars);
        free(morecommonchars);
        free(groups[i][0]);
        free(groups[i][1]);
        free(groups[i][2]);
    }
    free(groups);
    int sum = 0;
    for(int i = 0; i <= last; i++)
        sum += badges[i];
    printf("%d\n", sum);
}
