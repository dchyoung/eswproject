CC = gcc
CFLAGS = -g -Wall
AR = ar
LIB = libdatapro.a
INC_DIR = ../inc
OBJS = parser.o procdata.o
INCS = $(INC_DIR)/parser.h $(INC_DIR)/procdata.h

$(LIB): $(OBJS)
	$(AR) rv $(LIB) $(OBJS)

%.o: %.c $(INCS)
	$(CC) $(CFLAGS) -c -I$(INC_DIR) $< -o $@

clean:
	rm *.o $(LIB)
