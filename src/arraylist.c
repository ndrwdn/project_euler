#include "../include/arraylist.h"
#include <stdlib.h>

#define DEFAULT_ARRAYLIST_SIZE 10

arraylist *create_arraylist() {
    arraylist *list = malloc(sizeof(arraylist));
    list->index = 0;
    list->max_size = DEFAULT_ARRAYLIST_SIZE;
    list->list = malloc(sizeof(int) * list->max_size);
    return list;
}

void expand_arraylist(arraylist *list) {
    list->max_size *= 2;
    int *new_list = malloc(sizeof(int) * list->max_size);

    int i;
    for (i = 0; i < list->index; i++) {
        new_list[i] = list->list[i];
    }

    free(list->list);
    list->list = new_list;
}

void add_arraylist(arraylist *list, int value) {
    if (list->index >= list->max_size) {
        expand_arraylist(list);
    }

    list->list[list->index] = value;
    list->index++;
}

void destroy_arraylist(arraylist *list) {
    free(list->list);
    free(list);
}
