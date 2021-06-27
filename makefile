VPATH = ../src

CC = gcc
CFLAGS = -g -Wall -W -I../include

PROGRAM_NAME=chess

HEADERS = pgn-functions.h move.h board.h  
OBJS = main.o pgn-functions.o move.o board.o

$(PROGRAM_NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

-include $(OBJS:%.o=%.d)



clean:
	rm *.o $(PROGRAM_NAME)
