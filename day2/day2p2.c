#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum {
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3,
};

enum {
    LOSE = 0,
    DRAW = 3,
    WON  = 6,
};

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
    char *startptr;
    char *line = strtok_r(buf, delim, &startptr);
    unsigned int score = 0;
    while(line) {
        char *round = strdup(line);
        char rounddelim[] = " ";

        char *tok = strtok(round, rounddelim);
        int elf = (*tok) - 'A' + 1;
        tok = strtok(NULL, rounddelim);
        int player;
        switch((*tok) - 'X') {
            case 0: /* LOSE */
                switch(elf) {
                    case ROCK:
                        player = SCISSORS;
                        break;
                    case PAPER:
                        player = ROCK;
                        break;
                    case SCISSORS:
                        player = PAPER;
                        break;
                }
                break;
            case 1: /* DRAW */
                player = elf;
                break;
            case 2: /* WIN */
                switch(elf) {
                    case ROCK:
                        player = PAPER;
                        break;
                    case PAPER:
                        player = SCISSORS;
                        break;
                    case SCISSORS:
                        player = ROCK;
                        break;
                }
                break;
        }

        free(round);

        if((player == ROCK && elf == SCISSORS) ||
           (player == PAPER && elf == ROCK)    ||
           (player == SCISSORS && elf == PAPER))
            score += WON;
        else if(player == elf)
            score += DRAW;
        score += player;
        line = strtok_r(NULL, delim, &startptr);
    }
    printf("Score: %u\n", score);
}
