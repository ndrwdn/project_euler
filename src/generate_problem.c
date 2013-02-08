#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFF 1024

typedef struct file_object {
    char *name;
    char *mode;
    FILE *fp;
} file_object;

file_object *create_file(char *name, char *mode) {
    file_object *file = malloc(sizeof(file_object));
    file->name = name;
    file->mode = mode;
    return file;
}

int open_file(file_object *file) {
    file->fp = fopen(file->name, file->mode);
    return file->fp == NULL ? 0 : 1;
}

void close_file(file_object *file) {
    fclose(file->fp);
}

void destroy_file(file_object *file) {
    free(file);
}

char *get_problem_id(int problem_number) {
    if (problem_number < 1 || problem_number > 9999) {
        return NULL;
    }

    char *problem_id = malloc(5 * sizeof(char));
    snprintf(problem_id, 5, "%04d", problem_number);

    return problem_id;
}

int get_position_from_line(FILE *make_file, char *line) {
    int position_of_line = ftell(make_file);
    int length_of_line = strlen(line);
    int new_line = 1;

    return position_of_line - length_of_line - new_line;
}

int find_position(file_object *make_file, int problem_number) {
    char line_buffer[MAX_BUFF];
    int position = 0;
    char *next_problem = get_problem_id(problem_number + 1);

    while (fgets(line_buffer, MAX_BUFF, make_file->fp) != NULL) {
        if (strstr(line_buffer, next_problem)) {
            position = get_position_from_line(make_file->fp, line_buffer);
            break;
        }
    }
    free(next_problem);

    if (position == 0) {
        position = ftell(make_file->fp);
    }

    return position;
}

file_object *create_new_make_file() {
    file_object *file = create_file("Makefile.new", "w+");
    open_file(file);
    return file;
}

int is_new_problem_position(file_object *make_file, int position, int *added) {
    if (*added == 0 && ftell(make_file->fp) >= position) {
        *added = 1;
        return 1;
    }
    return 0;
}

void add_new_problem(FILE *new_make_file, char *problem_id) {
    fprintf(new_make_file, "\n\ntest/problem_%s_runner.c: test/problem_%s_test.c\n\t$(generate-test-runner)\n",
            problem_id, problem_id);

    fprintf(new_make_file, "\nbuild/problem_%s.o: src/problem_%s.c include/problem_%s.h\n\t$(cc-object-command)\n",
            problem_id, problem_id, problem_id);

    fprintf(new_make_file, "\nbuild/problem_%s_test: build/problem_%s.o test/problem_%s_test.c test/problem_%s_runner.c\n\t$(cc-test-command)\n",
            problem_id, problem_id, problem_id, problem_id);

    fprintf(new_make_file, "\nproblem_%s: build/problem_%s_test\n\t$<\n",
            problem_id, problem_id);
}

void move_file(file_object *from, file_object *to) {
    rewind(to->fp);
    ftruncate(fileno(to->fp), 0);

    rewind(from->fp);
    unsigned char buffer[MAX_BUFF];
    size_t item_size = sizeof(unsigned char);
    size_t items_read = 0;

    while ((items_read = fread(buffer, item_size, MAX_BUFF, from->fp)) > 0) {
        if (ferror(from->fp)) {
            break;
        }

        fwrite(buffer, item_size, items_read, to->fp);

        if (feof(from->fp)) {
            break;
        }
    }

    close_file(from);
    remove(from->name);
}

void add_problem_to_make_file(file_object *make_file, int position, char *problem_id) {
    printf("Adding problem_%s to Makefile.\n", problem_id);

    file_object *new_make_file = create_new_make_file();

    char line_buffer[MAX_BUFF];
    int added = 0;
    rewind(make_file->fp);
    while (fgets(line_buffer, MAX_BUFF, make_file->fp) != NULL) {
        if (is_new_problem_position(make_file, position, &added)) {
            add_new_problem(new_make_file->fp, problem_id);
        }
        fprintf(new_make_file->fp, "%s", line_buffer);
    }

    move_file(new_make_file, make_file);
}

void add_problem(file_object *make_file, int problem_number) {
    int position = find_position(make_file, problem_number);
    char *problem_id = get_problem_id(problem_number);

    add_problem_to_make_file(make_file, position, problem_id);
    free(problem_id);
}

int main(int argc, char *argv[]) {
    file_object *make_file = create_file("Makefile", "r+");

    if (!open_file(make_file))
        return 1;

    int problem_number = 2;

    add_problem(make_file, problem_number);

    close_file(make_file);
    destroy_file(make_file);

    return 0;
}
