#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 99

typedef struct Tree {
    int height;
    bool isvisible;
} Tree;

Tree forest[SIZE][SIZE];

bool visible_from_left(int j, int k)
{
    if(k == 0)
        return true;
    for(int i = 0; i < k; i++)
        if(forest[j][i].height >= forest[j][k].height)
            return false;
    return true;
}

bool visible_from_right(int j, int k)
{
    if(k == SIZE - 1)
        return true;
    for(int i = SIZE - 1; i > k; i--)
        if(forest[j][i].height >= forest[j][k].height)
            return false;
    return true;
}

bool visible_from_above(int j, int k)
{
    if(j == 0)
        return true;
    for(int i = 0; i < j; i++)
        if(forest[i][k].height >= forest[j][k].height)
            return false;
    return true;
}

bool visible_from_below(int j, int k)
{
    if(j == SIZE - 1)
        return true;
    for(int i = SIZE - 1; i > j; i--)
        if(forest[i][k].height >= forest[j][k].height)
            return false;
    return true;
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    char buf[BUFSIZ];
    int row = 0;
    while(fgets(buf, BUFSIZ, fp)) {
        for(int col = 0; col < SIZE; col++) {
            forest[row][col].height = buf[col] - '0';
            forest[row][col].isvisible = false;
        }
        row++;
    }
    fclose(fp);

    int visible = 0;
    for(int j = 0; j < SIZE; j++) {
        for(int k = 0; k < SIZE; k++) {
            visible += visible_from_left(j,  k) ||
                                     visible_from_right(j, k) ||
                                     visible_from_above(j, k) ||
                                     visible_from_below(j, k);
        }
    }
    printf("Visible trees: %d\n", visible);
}
