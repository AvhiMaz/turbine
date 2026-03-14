.PHONY: build run format all

build:
	gcc src/main.c -o main

run:
	./main

format:
	clang-format -i src/*.c src/*.h

all:
	make format && make build && make run
