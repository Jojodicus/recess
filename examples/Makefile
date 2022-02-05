CC = gcc
CFLAGS = -Wall -g -O2 -std=c11

RM = rm

clean:
	$(RM) -f *.out

%.out: %.c
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean