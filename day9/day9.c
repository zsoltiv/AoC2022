#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define LINEBUFSIZ 7

typedef struct Coordinate {
    int x, y;
} Coordinate;

Coordinate s        = {0, 0},
           H        = {0, 0},
           T        = {0, 0},
           *touched =   NULL;
int ntouched = 0;

bool visited(Coordinate *coord)
{
    for(int i = 0; i < ntouched; i++)
        if(touched[i].x == coord->x &&
           touched[i].y == coord->y)
            return true;
    return false;
}

void new_touched(Coordinate *coord)
{
    ntouched++;
    touched = realloc(touched, sizeof(Coordinate) * ntouched);
    touched[ntouched - 1].x = coord->x;
    touched[ntouched - 1].y = coord->y;
}

static inline bool samerow()
{
    return !abs(H.y - T.y);
}

static inline bool samecol()
{
    return !abs(H.x - T.x);
}

static inline bool touching()
{
    return abs(H.x - T.x) < 2 && abs(H.y - T.y) < 2;
}

void movetail()
{
    if(!samerow() && !samecol()) {
        if(abs(H.x - T.x) > abs(H.y - T.y)) {
            T.y = H.y;
        } else {
            T.x = H.x;
        }
    }
    if(!samecol())
        T.x = T.x < H.x ? H.x - 1 : H.x + 1;
    else if(!samerow())
        T.y = T.y < H.y ? H.y - 1 : H.y + 1;
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    char buf[LINEBUFSIZ] = {0};
    new_touched(&s);
    while(fgets(buf, LINEBUFSIZ - 1, fp)) {
        char *ptr = buf;
        while(!isdigit(*ptr)) ptr++;
        int steps = (int) strtoul(ptr, NULL, 10);
        printf("INSTRUCTION %c %d\n", *buf, steps);
        switch(*buf) {
            case 'U':
                for(int i = 0; i < steps; i++) {
                    H.y++;
                    if(!touching())
                        movetail();
                    if(!visited(&T))
                        new_touched(&T);
                }
                break;
            case 'D':
                for(int i = 0; i < steps; i++) {
                    H.y--;
                    if(!touching())
                        movetail();
                    if(!visited(&T))
                        new_touched(&T);
                }
                break;
            case 'L':
                for(int i = 0; i < steps; i++) {
                    H.x--;
                    if(!touching())
                        movetail();
                    if(!visited(&T))
                        new_touched(&T);
                }
                break;
            case 'R':
                for(int i = 0; i < steps; i++) {
                    H.x++;
                    if(!touching())
                        movetail();
                    if(!visited(&T))
                        new_touched(&T);
                }
                break;
        }
    }
    fclose(fp);
    printf("Positions visited at least once: %d\n", ntouched);
}
