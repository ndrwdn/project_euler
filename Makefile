UNITY_PATH=../../../Downloads/code/Unity
UNITY_SRC=$(UNITY_PATH)/src/unity.c
UNITY_RUNNER=$(UNITY_PATH)/auto/generate_test_runner.rb

CC=gcc
CFLAGS=-Wall -I$(UNITY_PATH)/src

define generate-test-runner
ruby $(UNITY_RUNNER) $< $@
endef

define cc-command
$(CC) $(CFLAGS)
endef

define cc-exec-command
$(cc-command) $^ -o $@
endef

define cc-object-command
$(cc-command) $< -c -o $@
endef

define cc-test-command
$(cc-exec-command) $(UNITY_SRC)
endef

clean:
	rm -f build/*
	rm -f test/*_runner.c

build/generate_problem: src/generate_problem.c
	$(cc-exec-command)

generate_problem: build/generate_problem
	$< $(NUM)


test/problem_0001_runner.c: test/problem_0001_test.c
	$(generate-test-runner)

build/problem_0001.o: src/problem_0001.c include/problem_0001.h
	$(cc-object-command)

build/problem_0001_test: build/problem_0001.o test/problem_0001_test.c test/problem_0001_runner.c
	$(cc-test-command)

problem_0001: build/problem_0001_test
	$<
