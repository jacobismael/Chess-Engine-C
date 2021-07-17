VPATH = ../src

CC = gcc
CFLAGS = -g -Wall -W -I../include --std=c11

PROGRAM_NAME=chess

HEADERS = pgn-functions.h move.h board.h game.h bot1.h bot2.h bot3.h standardlist.h
OBJS = main.o pgn-functions.o move.o board.o game.o bot1.o bot2.o bot3.o standardlist.o

$(PROGRAM_NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

-include $(OBJS:%.o=%.d)



clean:
	rm *.o $(PROGRAM_NAME)
