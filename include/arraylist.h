#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct arraylist {
    int size;
    int max_size;
    int *list;
} arraylist;

arraylist *create_arraylist();
void add_arraylist(arraylist *list, int value);
void destroy_arraylist(arraylist *list);

#endif

