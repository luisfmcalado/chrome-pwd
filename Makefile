SOURCES=$(wildcard deps/*.c src/*.c)
T_SOURCES=$(wildcard deps/*.c test/*.c src/*.c)
T_SOURCES := $(filter-out src/main.c, $(T_SOURCES))
CC=gcc

INC_LIBS=-L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include
LIBS=-lcrypto -lssl
CFLAGS=-Wall

all: dir
	$(CC) -o build/cdr $(SOURCES) $(CFLAGS) $(LIBS) $(INC_LIBS) $(MACROS)

debug: dir
	$(CC) -o build/cdr -g $(SOURCES) $(CFLAGS) $(LIBS) $(INC_LIBS) -DDEBUG $(MACROS)

test: dir
	$(CC) -o build/cdr_test -g $(T_SOURCES) $(CFLAGS) $(LIBS) $(INC_LIBS) -DDEBUG $(MACROS)

dir:
	mkdir -p build

clean:
	rm -rf build
