#include "../include/problem_0003.h"
#include "../include/arraylist.h"
#include <stdlib.h>
#include <math.h>

typedef struct primes {
    arraylist *p;
    int last_candidate;
} primes;

primes *create_primes() {
    primes *ps = malloc(sizeof(primes));
    ps->p = create_arraylist();
    return ps;
}

void destroy_primes(primes *ps) {
    destroy_arraylist(ps->p);
    free(ps);
}

int next_prime(primes *ps) {
    if (ps->p->size == 0) {
        ps->last_candidate = 2;
        goto out;
    }

    if (ps->p->size == 1) {
        ps->last_candidate = 3;
        goto out;
    }

    int is_prime = 0, is_composite, i;
    while (!is_prime) {
        ps->last_candidate += 2;

        is_composite = 0;
        for (i = 0; i < ps->p->size; i++) {
            if (ps->last_candidate % ps->p->list[i] == 0) {
                is_composite = 1;
                break;
            }
        }
        if (!is_composite) {
            is_prime = 1;
        }
    }

out:
    add_arraylist(ps->p, ps->last_candidate);
    return ps->last_candidate;
}

arraylist *factor(long composite) {
    arraylist *factors = create_arraylist();

    primes *ps = create_primes();
    int p;

    while ((p = next_prime(ps)) <= sqrt(composite)) {
        if (composite % p == 0) {
            add_arraylist(factors, p);
        }
    }

    destroy_primes(ps);

    return factors;
}

int problem_0003(long composite) {
    arraylist *factors = factor(composite);

    int largest_factor = factors->list[factors->size - 1];

    destroy_arraylist(factors);

    return largest_factor;
}
