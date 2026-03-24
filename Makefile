.PHONY: build run format all link

build:
	g++ -c thirdparty/leopard.cpp thirdparty/LeopardCommon.cpp thirdparty/LeopardFF8.cpp thirdparty/LeopardFF16.cpp wrapper/rs_wrapper.cpp && gcc -c src/main.c src/shred.c src/thread_pool.c && g++ main.o shred.o leopard.o LeopardCommon.o LeopardFF8.o LeopardFF16.o rs_wrapper.o -o main -lz

run:
	./main

format:
	clang-format -i src/*.c src/*.h && clang-format -i thirdparty/*.cpp thirdparty/*.h && clang-format -i wrapper/*.cpp wrapper/*.h 


all:
	make format && make build && make run

link:
	bear -- make build
