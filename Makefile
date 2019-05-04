CC = gcc
CFLAGS = -Wall -g
OBJECT1 = chatclient.o
PROG1 = chatclient

all: $(PROG1)

$(PROG1): $(OBJECT1)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c  $(CFLAGS) $^

clean:
	rm -rf $(OBJECT1) $(PROG1) 
