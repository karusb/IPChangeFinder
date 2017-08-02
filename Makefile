CC=g++
CFLAGS=-g -Wall
TARGET=./IPChange/IPChange

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET).o -c $(TARGET).cpp
	$(CC) -o $(TARGET).exe $(TARGET).o
test:
	$(TARGET).exe
clean:
	$(RM) $(TARGET)