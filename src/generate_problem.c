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

void destroy_file_object(file_object *file) {
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

void destroy_problem_id(char *problem_id) {
    free(problem_id);
}

int get_position_from_line(FILE *make_file, char *line) {
    int position_of_line = ftell(make_file);
    int length_of_line = strlen(line);
    int new_line = 1;

    return position_of_line - length_of_line - new_line;
}

int find_position(file_object *make_file, int problem_number, char *problem_id) {
    char line_buffer[MAX_BUFF];
    int position = 0;
    char *next_problem = get_problem_id(problem_number + 1);

    while (fgets(line_buffer, MAX_BUFF, make_file->fp) != NULL) {
        if (strstr(line_buffer, problem_id)) {
            position = -1;
            break;
        }
        if (strstr(line_buffer, next_problem)) {
            position = get_position_from_line(make_file->fp, line_buffer);
            break;
        }
    }
    destroy_problem_id(next_problem);

    if (position == 0) {
        position = ftell(make_file->fp);
    } else if (position > 0) {
        position -= 1;
        fseek(make_file->fp, position, SEEK_SET);
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

void add_problem_to_make_file(int problem_number, char *problem_id) {
    file_object *make_file = create_file("Makefile", "r+");
    open_file(make_file);

    int position = find_position(make_file, problem_number, problem_id);

    if (position == -1) {
        printf("Problem problem_%s already exists in Makefile, not re-adding.\n", problem_id);
        return;
    }

    printf("Adding problem_%s to Makefile.\n", problem_id);

    file_object *new_make_file = create_new_make_file();

    char line_buffer[MAX_BUFF];
    int added = 0;
    rewind(make_file->fp);
    while (fgets(line_buffer, MAX_BUFF, make_file->fp) != NULL) {
        fprintf(new_make_file->fp, "%s", line_buffer);
        if (is_new_problem_position(make_file, position, &added)) {
            add_new_problem(new_make_file->fp, problem_id);
        }
    }

    move_file(new_make_file, make_file);
    destroy_file_object(new_make_file);

    close_file(make_file);
    destroy_file_object(make_file);
}

char *create_file_name(char *prefix, char *problem_id, char *extension) {
    int file_name_length = strlen(prefix) + strlen(problem_id) + strlen(extension) + 1;
    char *file_name = malloc(file_name_length);

    snprintf(file_name, file_name_length, "%s%s%s", prefix, problem_id, extension);
    return file_name;
}

void destroy_file_name(char *file_name) {
    free(file_name);
}

int file_already_exists(char *type, char *file_name) {
    if (access(file_name, F_OK) != 0) {
        return 0;
    }

    printf("%s file %s already exists, not re-creating.\n", type, file_name);

    return 1;
}

void create_include_file(char *problem_id) {
    char *include_file_name = create_file_name("include/problem_", problem_id, ".h");

    if (file_already_exists("Include", include_file_name)) {
        goto out1;
    }

    printf("Creating include file: %s.\n", include_file_name);

    file_object *include_file = create_file(include_file_name, "w");
    open_file(include_file);

    fprintf(include_file->fp, "#ifndef PROBLEM_%s_H\n", problem_id);
    fprintf(include_file->fp, "#define PROBLEM_%s_H\n\n", problem_id);
    fprintf(include_file->fp, "int problem_%s();\n\n", problem_id);
    fprintf(include_file->fp, "#endif\n");

    close_file(include_file);
    destroy_file_object(include_file);

out1:
    destroy_file_name(include_file_name);
}

void create_source_file(char *problem_id) {
    char *source_file_name = create_file_name("src/problem_", problem_id, ".c");

    if (file_already_exists("Source", source_file_name)) {
        goto out1;
    }

    printf("Creating source file: %s.\n", source_file_name);

    file_object *source_file = create_file(source_file_name, "w");
    open_file(source_file);

    fprintf(source_file->fp, "#include \"../include/problem_%s.h\"\n\n", problem_id);
    fprintf(source_file->fp, "int problem_%s() {\n", problem_id);
    fprintf(source_file->fp, "    return 0;\n");
    fprintf(source_file->fp, "}\n");

    close_file(source_file);
    destroy_file_object(source_file);

out1:
    destroy_file_name(source_file_name);
}

void create_test_file(char *problem_id) {
    char *test_file_name = create_file_name("test/problem_", problem_id, "_test.c");

    if (file_already_exists("Test", test_file_name)) {
        goto out1;
    }

    printf("Creating test file: %s.\n", test_file_name);

    file_object *test_file = create_file(test_file_name, "w");
    open_file(test_file);

    fprintf(test_file->fp, "#include \"unity.h\"\n");
    fprintf(test_file->fp, "#include \"../include/problem_%s.h\"\n\n", problem_id);
    fprintf(test_file->fp, "void setUp() {\n");
    fprintf(test_file->fp, "}\n\n");
    fprintf(test_file->fp, "void tearDown() {\n");
    fprintf(test_file->fp, "}\n");

    close_file(test_file);
    destroy_file_object(test_file);

out1:
    destroy_file_name(test_file_name);
}

void add_problem(int problem_number) {
    char *problem_id = get_problem_id(problem_number);

    add_problem_to_make_file(problem_number, problem_id);

    create_include_file(problem_id);
    create_source_file(problem_id);
    create_test_file(problem_id);

    destroy_problem_id(problem_id);
}

int main(int argc, char *argv[]) {
    int problem_number = 2;
    add_problem(problem_number);

    return 0;
}
