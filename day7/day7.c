#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXDIRSIZE 100000

typedef struct FileSystemNode {
    struct FileSystemNode *parent;
    char *name;
    int ncontents;
    struct FileSystemNode **contents;
    unsigned long size;
    bool isdir;
} FileSystemNode;

void print_node(const FileSystemNode *restrict node)
{
    printf("{\n");
    printf("\tparent: %p\n\tname: %s\n\tncontents: %d\n\tcontents: %p\n\tsize: %lu\n",
           node->parent,
           node->name,
           node->ncontents,
           node->contents,
           node->size);
    printf("}\n");
}


FileSystemNode *new_node(FileSystemNode *parent,
                         char *name,
                         size_t size,
                         bool isdir)
{
    FileSystemNode *node = malloc(sizeof(FileSystemNode));
    if(parent) {
        parent->ncontents++;
        parent->contents = realloc(parent->contents,
                                   sizeof(FileSystemNode *) * parent->ncontents);
        parent->contents[parent->ncontents - 1] = node;
    }
    node->parent = parent;

    node->name = name ? strdup(name) : NULL;
    node->size = size;
    node->ncontents = 0;
    node->contents = NULL;
    node->isdir = isdir;

    if(!node->parent) {
        fprintf(stderr, "WARN: %s has no parent\n", node->name);
    }

    return node;
}

void cd(FileSystemNode **cwd, char *dir)
{
    size_t len = strlen(dir);
    if(!strncmp(dir, "..", strlen(dir))) {
        *cwd = (*cwd)->parent;
    } else {
        for(int i = 0; i < (*cwd)->ncontents; i++)
            if(strncmp(dir, (*cwd)->contents[i]->name, len)) {
                *cwd = (*cwd)->contents[i];
                break;
            }
    }
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");

    char line[BUFSIZ] = {0};
    const char delims[] = " \n";
    FileSystemNode *cwd = new_node(NULL, NULL, 0, true);
    int ndirs = 0;
    while(fgets(line, BUFSIZ - 1, fp)) {
        if(!strncmp("$ ls", line, strlen("$ ls")))
            continue;
        if(!strncmp("$ cd", line, strlen("$ cd"))) {
            char *token = strtok(line, delims);
            puts(token);
            token = strtok(NULL, delims);
            puts(token);
            token = strtok(NULL, delims);
            puts(token);
            if(strncmp(token, "/", strlen(token))) { // ignore the first line
                cd(&cwd, token);
            }
        } else if(!strncmp("dir", line, strlen("dir"))) {
            ndirs++;
            char *name = strtok(line, delims);
            name = strtok(NULL, delims);
            FileSystemNode *node = new_node(cwd, name, 0, true);
            //print_node(node);
        } else {
            char *tok = strtok(line, delims);
            unsigned long size = strtoul(tok, NULL, 10);
            tok = strtok(NULL, delims);
            FileSystemNode *node = new_node(cwd, tok, size, false);
            //print_node(node);
        }
    }

    FileSystemNode *root = cwd;
    while(root->parent)
        root = root->parent;

    unsigned long sz;
    dirsize(root, &sz);
    printf("%lu\n", sz);
    fclose(fp);
}
