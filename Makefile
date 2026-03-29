.PHONY: build run format all link

build:
	g++ -c thirdparty/leopard/leopard.cpp thirdparty/leopard/LeopardCommon.cpp thirdparty/leopard/LeopardFF8.cpp thirdparty/leopard/LeopardFF16.cpp wrapper/rs_wrapper.cpp && gcc -c thirdparty/cjson/cJSON.c thirdparty/b64/buffer.c thirdparty/b64/decode.c thirdparty/b64/encode.c && gcc -c src/main.c src/shred.c src/thread_pool.c && g++ main.o shred.o thread_pool.o cJSON.o buffer.o decode.o encode.o leopard.o LeopardCommon.o LeopardFF8.o LeopardFF16.o rs_wrapper.o -o main -lz -lpthread

run:
	./main

format:
	clang-format -i src/*.c src/*.h && clang-format -i thirdparty/leopard/*.cpp thirdparty/leopard/*.h thirdparty/cjson/*.c thirdparty/cjson/*.h thirdparty/b64/*.c thirdparty/b64/*.h thirdparty/cjson/*.h && clang-format -i wrapper/*.cpp wrapper/*.h

clean:
	rm -f *.o main


all:
	make format && make build && make run && make clean

link:
	bear -- make build
