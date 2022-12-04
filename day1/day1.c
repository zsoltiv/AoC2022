#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

int compare_calories(const void *a, const void *b)
{
    return ((*(int *) a) - (*(int *) b));
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

    int elf = 0;
    unsigned long *elves = malloc(sizeof(unsigned long) * (elf + 1));
    elves[0] = 0;
    const char delim[] = "\n";
    char *line = strsep(&buf, delim);
    while(line) {
        unsigned long calories = strtoul(line, NULL, 10);
        if(!calories) {
            elf++;
            elves = realloc(elves, sizeof(unsigned long) * (elf + 1));
            elves[elf] = 0;
        } else {
            elves[elf] += calories;
        }
        line = strsep(&buf, delim);
    }
    qsort(elves, elf + 1, sizeof(unsigned long), compare_calories);
    unsigned long top_three = 0;
    for(int i = elf; i > elf - 3; i--)
        top_three += elves[i];
    printf("Max: %lu\n", elves[elf]);
    printf("Top three: %lu\n", top_three);
}
