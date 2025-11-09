CC = gcc
CFLAGS = -Wall -Wextra -pthread

SERVER_SRCS = server.c admin.c manager.c employee.c customer.c loan.c user.c feedback.c
CLIENT_SRCS = client.c

all: server client

server: $(SERVER_SRCS)
	$(CC) $(CFLAGS) -o server $(SERVER_SRCS)

client: $(CLIENT_SRCS)
	$(CC) $(CFLAGS) -o client $(CLIENT_SRCS)

clean:
	rm -f server client *.o
