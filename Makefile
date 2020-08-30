CFLAGS = -O
CC = g++ 

all: docdex

docdex: main.o html_document.o stemmer.o
	$(CC) $(CFLAGS) -o docdex main.o html_document.o stemmer.o -lsqlite3 -lcurl

main.o:
	$(CC) $(CFLAGS) -c ./html-docdex/main.cpp -lsqlite3

html_document.o:
	$(CC) $(CFLAGS) -c ./html-docdex/html_document.cpp ./html-docdex/html_document.hpp -lcurl

stemmer.o:
	$(CC) $(CFLAGS) -c ./html-docdex/stemmer.cpp ./html-docdex/stemmer.hpp


clean:
	@echo "Cleaning up..."
	rm -f core *.o html-docdex/*hpp.gch
	rm ./docdex
