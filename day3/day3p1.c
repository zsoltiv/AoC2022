#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

int get_common_priority(char *rucksack)
{
    size_t len = strlen(rucksack);
    char c = rucksack[len / 2];
    rucksack[len / 2] = '\0';
    char *first = strdup(rucksack);
    rucksack[len / 2] = c;
    char *second = strdup(&rucksack[len / 2]);
    assert(strlen(first) == strlen(second));
    int priority;
    for(char *firstptr = first; *firstptr; firstptr++) {
        for(char *secondptr = second; *secondptr; secondptr++) {
            if(*firstptr == *secondptr) {
                priority = *firstptr;
                // a-z ->  1-26
                // A-Z -> 27-52
                priority = islower(priority) ? priority - 96 : priority - 38;
                break;
            }
        }
    }
    printf("%d\n", priority);
    return priority;
}

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
    int priority = 0;
    int *priorities = malloc(sizeof(int) * (priority + 1));
    while(rucksack) {
        priorities[priority++] = get_common_priority(rucksack);
        priorities = realloc(priorities, sizeof(int) * (priority + 1));
        rucksack = strtok(NULL, delim);
    }
    int sum = 0;
    for(int i = 0; i <= priority; i++)
        sum += priorities[i];
    printf("Sum: %d\n", sum);
}
