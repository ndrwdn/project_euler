#include "../include/problem_0002.h"
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_ARRAYLIST_SIZE 10

typedef struct arraylist {
    int index;
    int max_size;
    int *list;
} arraylist;

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

arraylist *fibonacci_up_to(int maximum) {
    arraylist *list = create_arraylist();
    add_arraylist(list, 1);
    add_arraylist(list, 2);

    int current_term = 2;
    while (current_term <= maximum) {
        current_term = list->list[list->index - 2] + list->list[list->index - 1];
        add_arraylist(list, current_term);
    }

    return list;
}

int problem_0002(int maximum) {
    arraylist *fibs = fibonacci_up_to(maximum);

    int sum = 0;
    int i;

    for (i = 0; i < fibs->index; i++) {
        int fib = fibs->list[i];
        if (fib % 2 == 0) {
            sum += fib;
        }
    }

    destroy_arraylist(fibs);

    return sum;
}
