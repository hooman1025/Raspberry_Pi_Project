CC = gcc
LN = ln

TARGET = cafe
MAIN = RaspberryPi_server_code.c

SRCS = Data_parsing.c Menu_select.c DB_update.c clnt_handle.c
OBJS = Data_parsing.o Menu_select.o DB_update.o clnt_handle.o
SO = lib$(TARGET).so
LIB = $(SO).0.0.0
LIBS = -l$(TARGET) -lpthread -lmariadbclient
LIB_DIR = -L./

CFLAGS = -g 
SO_FLAGS = -shared -Wl,-soname,$(SO).0

TARGET_ALL = chapter1 chapter2

all : $(TARGET_ALL)

chapter1 : Cafe.h
	$(CC) -fPIC $(CFLAGS) -c $(SRCS)
	$(CC) $(SO_FLAGS) -o $(LIB) $(OBJS)
	$(LN) -s $(LIB) $(SO)
	$(LN) -s $(LIB) $(SO).0
	$(LN) -s $(LIB) $(SO).0.0
	sudo cp $(SO) /usr/lib/x86_64-linux-gnu/
	sudo ldconfig

chapter2 : $(LIBS) Cafe.h
	$(CC) $(CFLAGS) -o server $(MAIN) $(LIB_DIR) $^

clean :
	rm -f *.o lib* server
	sudo rm /usr/lib/x86_64-linux-gnu/$(SO)


