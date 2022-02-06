FAIL_CHANCE = 10 # @todo: make this an environment variable

CC = gcc
CFLAGS = -Wall -g -O2 -std=c11

SRC = recess
LIB = lib$(SRC).so

RM = rm
INSTALL_PATH = $(HOME)/.local/bin

all: $(LIB)

clean:
	$(RM) -f *.so *.o

install: all
	mkdir -p $(INSTALL_PATH)
	cp $(SRC) $(INSTALL_PATH)/$(SRC)
	cp $(LIB) $(INSTALL_PATH)/$(LIB)
	chmod +x $(INSTALL_PATH)/$(SRC)

uninstall:
	rm -rf $(INSTALL_PATH)/$(SRC) $(INSTALL_PATH)/$(LIB)

$(LIB): $(SRC).c
	$(CC) -shared -fPIC -ldl -DFAIL_CHANCE=$(FAIL_CHANCE) $< -o $@

.PHONY: all clean install uninstall