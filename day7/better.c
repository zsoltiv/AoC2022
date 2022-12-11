#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node {
    struct Node *parent;
    char *name;
    unsigned long size;
    bool isdir;
    int nchildren;
    struct Node **children;
} Node;

Node *new_node(Node *parent, char *name, bool isdir, size_t size)
{
    Node *node = malloc(sizeof(Node));
    node->parent = parent;
    node->name = name;
    node->isdir = isdir;
    node->size = size;
    if(node->parent) {
        parent->nchildren++;
        parent->children = realloc(parent->children, sizeof(Node *) * parent->nchildren);
        parent->children[parent->nchildren - 1] = node;
    }

    return node;
}

static inline bool startswith(const char *str, const char *prefix)
{
    return !strncmp(prefix, str, strlen(prefix));
}

Node *getchild(Node *cwd, const char *dir)
{
    for(int i = 0; i < cwd->nchildren; i++)
        if(!strncmp(cwd->children[i]->name, dir, strlen(cwd->children[i]->name)))
            return cwd->children[i];
    return NULL;
}

void cd(Node **cwd, char *dir)
{
    if(startswith(dir, ".."))
        *cwd = (*cwd)->parent;
    else {
        Node *child = getchild(*cwd, dir);
        if(!child) {
            Node *node = new_node(*cwd, dir, true, 0);
            (*cwd)->nchildren++;
            (*cwd)->children = realloc((*cwd)->children, sizeof(Node *) * (*cwd)->nchildren);
            (*cwd)->children[(*cwd)->nchildren - 1] = node;
            *cwd = node;
        } else
            *cwd = child;
    }
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    char line[BUFSIZ] = {0};
    const char delims[] = " \n";
    Node *root = new_node(NULL, "", true, 0);
    Node *cwd = root;
    while(fgets(line, BUFSIZ - 1, fp)) {
        if(startswith(line, "$ ls"))
            continue;
        else if(startswith(line, "$ cd")) {
            char *dirname = strtok(line, delims);
            dirname = strtok(NULL, delims);
            dirname = strtok(NULL, delims);
            cd(&cwd, dirname);
        } else if(startswith(line, "dir ")) {
            char *dirname = strtok(line, delims);
            dirname = strtok(NULL, delims);
            Node *node = new_node(cwd, dirname, true, 0);
        } else {
            unsigned long size = strtoul(line, NULL, 10);
            char *filename = strtok(line, delims);
            filename = strtok(NULL, delims);
            Node *node = new_node(cwd, filename, false, size);
        }
    }
}
