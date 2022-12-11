#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define WIDTH 40
#define HEIGHT 6

int X = 1;
int cycles = 1;
int strengthsum = 0;

static void docycle()
{
    {
       if(cycles % 40 == 0)
           putc('\n', stdout);
       else {
           int horizontalpos = cycles % 40;
           if(horizontalpos - 1 == X || horizontalpos == X || horizontalpos + 1 == X)
               putc('#', stdout);
           else
               putc('.', stdout);
       }
    }
    cycles++;
    if(cycles == 20  ||
       cycles == 60  ||
       cycles == 100 ||
       cycles == 140 ||
       cycles == 180 ||
       cycles == 220)
        strengthsum += cycles * X;
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    char buf[BUFSIZ] = {0};

    while(fgets(buf, BUFSIZ, fp)) {
        if(!strncmp("addx", buf, strlen("addx"))) {
            docycle();
            char *ptr = buf;
            while(*ptr != ' ') ptr++;
            ptr++;
            int V = (int) strtol(ptr, NULL, 10);
            X += V;
        }
        docycle();
    }
    printf("Sum of signal strengths is %d\n", strengthsum);
}
