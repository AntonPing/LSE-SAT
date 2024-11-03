CC=gcc

SRCS=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c, obj/%.o, $(SRCS))

FLAG=-g -Wall -Wextra -pedantic
TARGET=bin/lse-sat

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(FLAG)

obj/%.o: src/%.c
	$(CC) -o $@ -c $< -g

build: $(TARGET)

run: $(TARGET)
	$(TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)