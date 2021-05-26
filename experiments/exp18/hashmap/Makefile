CLANG_WARN=-Wall -Wextra -Wpedantic
CC_OPT=-O3
CC=clang $(CLANG_WARN) $(CC_OPT)
MAIN_C=main.c

all: main.c
	$(CC) $(MAIN_C)

run: all
	./a.out

clean:
	rm -f a.out

.PHONY: clean
