CC = gcc
SRC = recess
RM = rm
INSTALL_PATH = $(HOME)/.local/bin

all: lib$(SRC).so

clean:
	$(RM) -f *.so

install: all
	mkdir -p $(INSTALL_PATH)
	cp $(SRC) $(INSTALL_PATH)/$(SRC)
	cp lib$(SRC).so $(INSTALL_PATH)/lib$(SRC).so
	chmod +x $(INSTALL_PATH)/$(SRC)

uninstall:
	rm -rf $(INSTALL_PATH)/$(SRC) $(INSTALL_PATH)/lib$(SRC).so

lib%.so: %.c
	$(CC) -shared -fPIC -ldl $< -o $@

.PHONY: all clean install uninstall