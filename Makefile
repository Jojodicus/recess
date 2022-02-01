CC = gcc
CFLAGS = -Wall -g -O2 -std=c11

SRC = recess
LIB = lib$(SRC).so
CFG = $(SRC).cfg

RM = rm
INSTALL_PATH = $(HOME)/.local/bin
CFG_PATH = $(HOME)/.config

all: $(LIB)

clean:
	$(RM) -f *.so

install: all
	mkdir -p $(INSTALL_PATH)
	mkdir -p $(CFG_PATH)
	cp $(SRC) $(INSTALL_PATH)/$(SRC)
	cp $(LIB) $(INSTALL_PATH)/$(LIB)
	chmod +x $(INSTALL_PATH)/$(SRC)

uninstall:
	rm -rf $(INSTALL_PATH)/$(SRC) $(INSTALL_PATH)/$(LIB) $(CFG_PATH)/$(CFG)

$(LIB): $(SRC).c
	$(CC) -shared -fPIC -ldl $< -o $@

.PHONY: all clean install uninstall