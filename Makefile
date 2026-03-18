.PHONY: build run format all link

build:
	gcc src/main.c src/shred.c -o main

run:
	./main

format:
	clang-format -i src/*.c src/*.h

all:
	make format && make build && make run

link:
	bear -- make build
