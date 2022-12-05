#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum {
    PARSING_STACKS = 0,
    PARSING_MOVES,
} ParsingState;

typedef struct {
    int height;
    char *stack; // not NULL terminated
} Stack;

Stack *new_stack()
{
    Stack *s = malloc(sizeof(Stack));
    s->height =    0;
    s->stack  = NULL;
    return s;
}

void add_to_stack(Stack *s, char c)
{
    s->height++;
    s->stack = realloc(s->stack, s->height);
    s->stack[s->height - 1] = c;
}

char pop_from_stack(Stack *s)
{
    char c = s->stack[s->height - 1];
    s->height--;
    s->stack = realloc(s->stack, s->height);
    return c;
}

void fill_stacks(Stack **stacks, int linecount, char **data, char *indicesline)
{
    int last = strlen(data[0]);
    for(int line = linecount - 1; line >= 0; line--) {
        int stack = 0;
        for(int i = 0; i < last; i++) {
            char c = data[line][i];
            if(isdigit(indicesline[i])) {
                if(isupper(c))
                    add_to_stack(stacks[stack], c);
                stack++;
            }
        }
    }
}

void move_stack(Stack *from, Stack *to, int amount)
{
    for(int move = amount - 1; move >= 0; move--)
        add_to_stack(to, pop_from_stack(from));
}

void dump_stack(Stack *s)
{
    for(int i = 0; i < s->height; i++)
        printf("%c ", s->stack[i]);
    printf("\n");
}

bool isstackline(const char *line)
{
    for(const char *ptr = line; *ptr; ptr++)
        if(*ptr == '[')
            return true;
    return false;
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");

    // stack strings
    int linecount = 0;
    char **lines = NULL;
    // more stack related stuff
    int stackcount = 0;
    Stack **stacks = NULL;
    //parsing related variables
    ParsingState parsing_state = PARSING_STACKS;
    char *line = NULL;
    size_t n;
    while(getline(&line, &n, fp) != -1) {
        switch(parsing_state) {
            case PARSING_STACKS:
                if(isstackline(line)) {
                    linecount++;
                    lines = realloc(lines, sizeof(char *) * linecount);
                    lines[linecount - 1] = strdup(line);
                } else {
                    // parsing the line with the stack indices starting at 1
                    for(size_t i = strlen(line) - 1; i >= 0; i--) {
                        stackcount = (int) strtoul(&line[i], NULL, 10);
                        if(stackcount)
                            break;
                    }

                    stacks = malloc(sizeof(Stack) * stackcount);
                    for(int i = 0; i < stackcount; i++)
                        stacks[i] = new_stack();
                    fill_stacks(stacks, linecount, lines, line);

                    for(int i = 0; i < linecount; i++)
                        free(lines[i]);

                    parsing_state = PARSING_MOVES;
                }
                break;
            case PARSING_MOVES:
                if(!strncmp("move", line, strlen("move"))) {
                    int from, amount, to;
                    if(sscanf(line, "move %d from %d to %d",
                              &amount, &from, &to) != 3) {
                        printf("bajos a dolog haverda\n");
                    }
                    // 0-based index
                    from--;
                    to--;
                    
                    move_stack(stacks[from], stacks[to], amount);
                }
                break;
        }
    }

    for(int i = 0; i < stackcount; i++)
        dump_stack(stacks[i]);

    size_t topssize = stackcount + 1;
    char tops[topssize];
    memset(tops, 0, topssize);
    for(int i = 0; i < stackcount; i++) {
        Stack *s = stacks[i];
        tops[i] = s->stack[s->height - 1];
    }
    printf("Tops: %s\n", tops);
    free(line);
    fclose(fp);
}
