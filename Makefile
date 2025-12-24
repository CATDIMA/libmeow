CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Oz -fshort-enums -ffunction-sections -fdata-sections -fno-unroll-loops

.PHONY: lib
lib: libmeow.a

libmeow.a: build/meow.o
	ar rcs build/libmeow.a build/meow.o

build/meow.o: src/meow.c
	mkdir -p build/
	$(CC) $(CFLAGS) -I inc/ -c src/meow.c -o build/meow.o

.PHONY: clean
clean:
	rm -rf build/
