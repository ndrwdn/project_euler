UNITY_PATH=../../../Downloads/code/Unity
UNITY_SRC=$(UNITY_PATH)/src/unity.c
UNITY_RUNNER=$(UNITY_PATH)/auto/generate_test_runner.rb

CC=gcc
CFLAGS=-Wall -I$(UNITY_PATH)/src

define generate-test-runner
ruby $(UNITY_RUNNER) $< $@
endef

define cc-command
$(CC) $(CFLAGS) $(UNITY_SRC) $^ -o $@
endef

clean:
	rm -f build/*
	rm -f test/*_runner.c


test/problem_0001_runner.c: test/problem_0001_test.c
	$(generate-test-runner)

build/problem_0001_test: include/problem_0001.h src/problem_0001.c test/problem_0001_test.c test/problem_0001_runner.c
	$(cc-command)
	$@

problem_0001: build/problem_0001_test
