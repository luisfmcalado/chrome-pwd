CC=gcc

all:
	mkdir -p build
	$(CC) -o build/chromedatareader src/main.c deps/sqlite3.c -lcrypt32
