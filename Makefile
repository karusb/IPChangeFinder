CC=g++
CFLAGS=-g -Wall
PATH=./IPChange/
TARGET=IPChange

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(PATH)$(TARGET).o -c $(PATH)$(TARGET).cpp
	$(CC) -o $(PATH)$(TARGET).exe $(PATH)$(TARGET).o
test:
	$(TARGET).exe
clean:
	$(RM) $(TARGET)