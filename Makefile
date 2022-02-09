DEFAULT_FAIL_CHANCE = 10

CC = gcc
CFLAGS = -Wall -g -O2 -std=c11
RM = rm

# i know this is convoluted
SRC = recess
LIB = lib$(SRC).so
CFG = recess.cfg

INSTALL_PATH = $(HOME)/.local/bin
CFG_PATH = $(HOME)/.config

all: $(LIB)

clean:
	$(RM) -f *.so *.o

install: all
	mkdir -p $(INSTALL_PATH)
	mkdir -p $(CFG_PATH)
	cp $(SRC) $(INSTALL_PATH)/$(SRC)
	cp $(LIB) $(INSTALL_PATH)/$(LIB)
	chmod +x $(INSTALL_PATH)/$(SRC)
	cp $(CFG) $(CFG_PATH)/$(CFG)

uninstall:
	rm -rf $(INSTALL_PATH)/$(SRC) $(INSTALL_PATH)/$(LIB) $(CFG)

$(LIB): $(SRC).c
	$(CC) -shared -fPIC -ldl -DDEFAULT_FAIL_CHANCE=$(DEFAULT_FAIL_CHANCE) $< -o $@

.PHONY: all clean install uninstall