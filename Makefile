DEFAULT_FAIL_CHANCE = 10


CC = gcc
CFLAGS = -fPIC -Wall -g -O2 -std=c11
LDFLAGS = -shared -ldl
RM = rm

# @TODO: XDG path
INSTALL_PATH = $(HOME)/.local/bin
CFG_PATH = $(HOME)/.config


all: librecess.so

%.o:
	$(CC) $(CFLAGS) -c $< -o $@


OBJS = recess.o

# depenencies
recess.o: src/recess.c src/recess.h

librecess.so: $(OBJS)
	$(CC) $(LDFLAGS) -DDEFAULT_FAIL_CHANCE=$(DEFAULT_FAIL_CHANCE) $^ -o $@


clean:
	$(RM) -f *.so *.o


install: librecess.so recess.sh recess.cfg
	mkdir -p $(INSTALL_PATH)
	mkdir -p $(CFG_PATH)
	cp recess.sh $(INSTALL_PATH)/recess
	cp librecess.so $(INSTALL_PATH)/librecess.so
	chmod +x $(INSTALL_PATH)/recess
	cp recess.cfg $(CFG_PATH)/recess.cfg

uninstall:
	rm -rf $(INSTALL_PATH)/recess $(INSTALL_PATH)/librecess.so $(CFG_PATH)/recess.cfg


.PHONY: all clean install uninstall

