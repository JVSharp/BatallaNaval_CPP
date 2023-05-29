CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
LFLAGS = -lws2_32

SERVER_SRC = servidor.cpp
CLIENT_SRC = cliente.cpp

SERVER_TARGET = servidor
CLIENT_TARGET = cliente

.PHONY: all clean

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_SRC) $(LFLAGS)

$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SRC) $(LFLAGS)

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
