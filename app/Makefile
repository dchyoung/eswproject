CC = gcc
OUTPUT = datamaster
LIB = datapro
LIB_DIR = ../lib
INC_DIR = ../inc
OBJS = main.o
INCS = $(INC_DIR)/procdata.h

$(OUTPUT): $(OBJS)
	$(CC) -L$(LIB_DIR) $(OBJS) -l$(LIB) -o $(OUTPUT)

%.o: %.c $(INCS)
	$(CC) -c -I$(INC_DIR) $< -o $@

clean:
	rm *.o $(OUTPUT)
