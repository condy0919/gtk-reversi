CC=gcc
LIBS = `pkg-config --libs gtk+-2.0`
INC = `pkg-config --cflags gtk+-2.0`
#CFLAGS = -c -Wall -g #debug
#CFLAGS = -c -O2 -Wall -g #test and test
CFLAGS = -c -O2 -Wall #release
OBJS = gtk_main.o signal_callback.o gui.o interface.o engine.o list.o
BIN = gtk_main
RM = rm -rf

.PHONY: all , clean

all: $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LIBS)
	$(RM) $(OBJS)

gtk_main.o: gtk_main.c gui.h interface.h signal_callback.h
	$(CC) $(CFLAGS) gtk_main.c $(INC)

signal_callback.o: signal_callback.c gui.h commondefs.h interface.h
	$(CC) $(CFLAGS) signal_callback.c $(INC)

gui.o: gui.c signal_callback.h commondefs.h signal_callback.h
	$(CC) $(CFLAGS) gui.c $(INC)

interface.o: interface.c commondefs.h
	$(CC) $(CFLAGS) interface.c $(INC)

engine.o: engine.c list.h list.o
	$(CC) $(CFLAGS) engine.c $(INC)

list.o: list.c 
	$(CC) $(CFLAGS) list.c $(INC)

clean:
	$(RM) $(BIN) *.o *.*~ *~
