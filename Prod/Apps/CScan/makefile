#CC current variable for compiler in C 
CC=gcc


#Source DIR
S_DIR=src
#Header DIR 
H_DIR=include

L_DIR=lib

CPPFLAGS = -I $(H_DIR)/ -I $(H_DIR)/"curl-7.64.0-win32-mingw"

#CFLAGS current variable for compilation options   
#-W et -Wall display front end compiler warnings 
#-ansi request ansi compilation 
#-pedantic display compiler warnings 
CFLAGS=-W -Wall -ansi -pedantic -std=c99 

#LDFLAGS current variable for link editing 
LDFLAGS = -L $(L_DIR)/"curl-7.64.0-win32-mingw"

LDLIBS = -lmingw32 -lcurl
# curent variable for executable 
EXEC=main

all: $(EXEC)

$(EXEC): main.o
	$(CC) $(CFLAGS) -o $(EXEC)  main.o $(LDFLAGS) $(CPPFLAGS)  $(LDLIBS)

main.o: $(S_DIR)/main.c $(H_DIR)/main.h
	$(CC)  -o main.o -c $(S_DIR)/main.c $(CFLAGS) $(CPPFLAGS) 
curlHandler.o: $(S_DIR)/curlHandler.c $(H_DIR)/curlHandler.h
	$(CC)  -o curlHandler.o -c $(S_DIR)/curlHandler.c $(CFLAGS) $(CPPFLAGS)
fileHandler.o: $(S_DIR)/fileHandler.c $(H_DIR)/fileHandler.h
	$(CC)  -o fileHandler.o -c $(S_DIR)/fileHandler.c $(CFLAGS) $(CPPFLAGS)
jsonHandler.o: $(S_DIR)/jsonHandler.c $(H_DIR)/jsonHandler.h
	$(CC)  -o jsonHandler.o -c $(S_DIR)/jsonHandler.c $(CFLAGS) $(CPPFLAGS)

clean:
	rm *.o core

delproper: clean
	rm main.exe


