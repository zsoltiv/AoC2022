#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define TOTALSPACE 70000000
#define UPDATESIZE 30000000

typedef struct Node {
    struct Node *parent;
    char *name;
    int nchildren;
    struct Node **children;
    unsigned long size;
} Node;

unsigned long dirsizesum = 0;
unsigned long mustbefreed = 0;
unsigned long smallesttofree = ULONG_MAX;

void append_child(Node *parent, Node *child)
{
    if(!parent || !child)
        return;

    parent->nchildren++;
    parent->children = realloc(parent->children,
                               sizeof(Node *) * parent->nchildren);
    parent->children[parent->nchildren - 1] = child;
    child->parent = parent;
}

Node *new_node(Node *parent, char *name, size_t size)
{
    Node *node = malloc(sizeof(Node));

    node->name = strdup(name);
    node->size = size;
    append_child(parent, node);
    node->nchildren = 0;
    node->children = NULL;

    return node;
}

Node *cd(Node *cwd, char *name)
{
    if(!strncmp("..", name, strlen(".."))) {
        return cwd->parent;
    }
    for(int i = 0; i < cwd->nchildren; i++)
        if(!strncmp(name, cwd->children[i]->name, strlen(name)))
            return cwd->children[i];
    return new_node(cwd, name, 0);
}

void dump_node(Node *node)
{
    printf("{\n");
    printf("\tname: %s\n", node->name);
    printf("\tsize: %lu\n", node->size);
    printf("}\n");
}

void print_branch(Node *root)
{
    dump_node(root);
    for(int i = 0; i < root->nchildren; i++)
        print_branch(root->children[i]);
}

unsigned long dirsize(Node *dir)
{
    unsigned long size = 0;
    for(int i = 0; i < dir->nchildren; i++) {
        if(!dir->children[i]->size)
            size += dirsize(dir->children[i]);
        size += dir->children[i]->size;
    }

    if(size <= 100000)
        dirsizesum += size;
    else if(size >= UPDATESIZE - mustbefreed && size < smallesttofree) {
        printf("Potential good answer %lu\n", size);
        smallesttofree = size;
    }
    return size;
}

int main(void)
{
    FILE *fp = fopen("input.txt", "r");
    char buf[BUFSIZ] = {0};
    char delim[] = " ";

    Node *root = new_node(NULL, "/", 0);
    Node *cwd = root;

    while(fgets(buf, BUFSIZ - 1, fp)) {
        if(!strncmp("$", buf, strlen("$"))) {
            char *tok = strtok(buf, delim);
            tok = strtok(NULL, delim);
            if(!strncmp(tok, "cd", strlen(tok))) {
                tok = strtok(NULL, delim);
                if(strncmp("/", tok, strlen("/"))) {
                    cwd = cd(cwd, tok);
                }
            }
        } else if(!strncmp("dir", buf, strlen("dir"))) {
            char *tok = strtok(buf, delim);
            tok = strtok(NULL, delim);
            new_node(cwd, tok, 0);
        } else {
            char *tok = strtok(buf, delim);
            unsigned long size = strtoul(tok, NULL, 10);
            tok = strtok(NULL, delim);
            new_node(cwd, tok, size);
            if(!size) {
                fprintf(stderr, "WARN: Regular file %s has size 0!\n", tok);
            }
        }
    }
    fclose(fp);

    mustbefreed = TOTALSPACE - dirsize(root);
    printf("Sum of all directory sizes under 100000: %lu\n", dirsizesum);
    dirsize(root); // second pass
    printf("Size of the directory to be freed: %lu\n", smallesttofree);
}
