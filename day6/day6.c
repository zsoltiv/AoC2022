#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int marker(char *data, int markersize)
{
    char buffer[markersize + 1];
    size_t datalen = strlen(data);
    int i;
    for(i = 0; i < datalen; i++) {
        if(datalen - i < markersize)
            break;
        strncpy(buffer, &data[i], markersize);
        char unique[markersize]; // not NULL terminated
        memset(unique, 0, markersize);
        int lastunique = 0;
        for(int j = 0; j < strlen(buffer); j++) {
            bool present = false;
            for(int k = 0; k < lastunique; k++)
                if(buffer[j] == unique[k])
                    present = true;
            if(!present)
                unique[lastunique++] = buffer[j];
        }

        if(lastunique == markersize)
            break;
        memset(buffer, 0, markersize + 1);
    }

    return i + markersize;
}

int main(void)
{
    // I should really make this into a function already
    FILE *fp = fopen("input.txt", "r");
    fseek(fp, 0, SEEK_END);
    size_t bufsize = ftell(fp) + 1;
    fseek(fp, 0, SEEK_SET);
    char *buf = calloc(bufsize, 1);
    fread(buf, 1, bufsize - 1, fp);
    fclose(fp);

    printf("start-of-packet: %d\n", marker(buf, 4));
    printf("start-of-message: %d\n", marker(buf, 14));
}
