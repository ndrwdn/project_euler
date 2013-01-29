#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFF 1024

int open_make_file(FILE **make_file) {
    *make_file = fopen("Makefile", "r");
    return *make_file == NULL ? 0 : 1;
}

char *get_problem_id(int problem_number) {
    if (problem_number < 1 || problem_number > 9999) {
        return NULL;
    }

    char *problem_id = malloc(5 * sizeof(char));
    snprintf(problem_id, 5, "%04d", problem_number);

    return problem_id;
}

int find_position(FILE *make_file, int problem_number) {
    char line_buffer[MAX_BUFF];
    int position = 0;
    char *next_problem = get_problem_id(problem_number + 1);

    while (fgets(line_buffer, MAX_BUFF, make_file) != NULL) {
        if (strstr(line_buffer, next_problem)) {
            position = ftell(make_file);
        }
    }
    free(next_problem);

    if (position == 0) {
        position = ftell(make_file);
    }

    return position;
}

void add_problem_to_make_file(FILE *make_file, int position, char *problem_id) {
    printf("Adding problem_%s to Makefile.\n", problem_id);

    fseek(make_file, position, SEEK_SET);

    fprintf(make_file, "\n\ntest/problem_%s_runner.c: test/problem_%s_test.c\n\t$(generate-test-runner)",
            problem_id, problem_id);

    fprintf(make_file, "\nbuild/problem_%s.o: src/problem_%s.c include/problem_%s.h\n\t$(cc-object-command)",
            problem_id, problem_id, problem_id);

    fprintf(make_file, "\nbuild/problem_%s_test: build/problem_%s.o test/problem_%s_test.c test/problem_%s_runner.c\n\t$(cc-test-command)",
            problem_id, problem_id, problem_id, problem_id);

    fprintf(make_file, "\nproblem_%s: build/problem_%s_test\n\t$<\n",
            problem_id, problem_id);
}

void add_problem(FILE *make_file, int problem_number) {
    int position = find_position(make_file, problem_number);
    char *problem_id = get_problem_id(problem_number);

    add_problem_to_make_file(make_file, position, problem_id);
    free(problem_id);
}

int main(int argc, char *argv[]) {
    FILE *make_file;

    if (!open_make_file(&make_file))
        return 1;

    int problem_number = 2;

    add_problem(make_file, problem_number);

    fclose(make_file);

    return 0;
}
