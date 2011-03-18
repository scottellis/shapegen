
CC = gcc
CFLAGS = -Wall -Wno-unused-function

CXXFLAGS += `pkg-config opencv --cflags`

LDFLAGS += `pkg-config opencv --libs`


TARGET = shapegen


$(TARGET): main.c
	$(CC) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) main.c -o $(TARGET)
	
	
clean:
	rm shapegen

