#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define SIZE 99

int forest[SIZE][SIZE];

bool visible_from_left(int j, int k)
{
    if(k == 0)
        return true;
    for(int i = 0; i < k; i++)
        if(forest[j][i] >= forest[j][k])
            return false;
    return true;
}

bool visible_from_right(int j, int k)
{
    if(k == SIZE - 1)
        return true;
    for(int i = SIZE - 1; i > k; i--)
        if(forest[j][i] >= forest[j][k])
            return false;
    return true;
}

bool visible_from_above(int j, int k)
{
    if(j == 0)
        return true;
    for(int i = 0; i < j; i++)
        if(forest[i][k] >= forest[j][k])
            return false;
    return true;
}

bool visible_from_below(int j, int k)
{
    if(j == SIZE - 1)
        return true;
    for(int i = SIZE - 1; i > j; i--)
        if(forest[i][k] >= forest[j][k])
            return false;
    return true;
}

int scenic_score(int j, int k)
{
    int scenic_left = 0;
    for(int i = k - 1; i >= 0; --i) {
        scenic_left++;
        if(forest[j][i] >= forest[j][k])
            break;
    }
    int scenic_right = 0;
    for(int i = k + 1; i < SIZE; ++i) {
        scenic_right++;
        if(forest[j][i] >= forest[j][k])
            break;
    }
    int scenic_up = 0;
    for(int i = j - 1; i >= 0; --i) {
        scenic_up++;
        if(forest[i][k] >= forest[j][k])
            break;
    }
    int scenic_down = 0;
    for(int i = j + 1; i < SIZE; ++i) {
        scenic_down++;
        if(forest[i][k] >= forest[j][k])
            break;
    }
    return scenic_left * scenic_right * scenic_up * scenic_down;
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    char buf[BUFSIZ];
    int row = 0;
    while(fgets(buf, BUFSIZ, fp)) {
        for(int col = 0; col < SIZE; col++) {
            forest[row][col] = buf[col] - '0';
        }
        row++;
    }
    fclose(fp);

    int visible = 0;
    int scenicmax = INT_MIN;
    for(int j = 0; j < SIZE; j++) {
        for(int k = 0; k < SIZE; k++) {
            visible += visible_from_left(j,  k) ||
                                     visible_from_right(j, k) ||
                                     visible_from_above(j, k) ||
                                     visible_from_below(j, k);
            int scenic_current = scenic_score(j, k);
            scenicmax = scenic_current > scenicmax ? scenic_current : scenicmax;
        }
    }
    printf("Visible trees: %d\nHighest scenic score: %d\n", visible, scenicmax);
}
