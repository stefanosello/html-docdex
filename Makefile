CFLAGS = -O
CC = g++ 

all: docdex

docdex: main.o html_document.o stemmer.o html_handler.o db_handler.o
	$(CC) $(CFLAGS) -o docdex main.o html_document.o stemmer.o html_handler.o db_handler.o -lsqlite3 -lcurl

main.o:
	$(CC) $(CFLAGS) -c ./html-docdex/main.cpp -lsqlite3

html_document.o:
	$(CC) $(CFLAGS) -c ./html-docdex/html_document.cpp ./html-docdex/html_document.hpp -lcurl

stemmer.o:
	$(CC) $(CFLAGS) -c ./html-docdex/stemmer.cpp ./html-docdex/stemmer.hpp

html_handler.o:
	$(CC) $(CFLAGS) -c ./html-docdex/html_handler.cpp ./html-docdex/html_handler.hpp

db_handler.o:
	$(CC) $(CFLAGS) -c ./html-docdex/db_handler.cpp ./html-docdex/db_handler.hpp


clean:
	@echo "Cleaning up..."
	rm -f core *.o html-docdex/*hpp.gch
	rm ./docdex
