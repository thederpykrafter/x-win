# use @<command> to supress make echoing command
CC = gcc
LANG = c
CFLAGS = -lX11
OUT_FILE = x-win

default: build

all: build install

test: build run

build: src/main.$(LANG)
	@if [ ! -d bin ]; then mkdir bin; fi
	@if ! cat README.md | head -n1 | grep $(OUT_FILE); then sed -i "1s/.*/# $(OUT_FILE)/" README.md; fi
	@gcc src/main.$(LANG) $(CFLAGS) -o bin/$(OUT_FILE)
	@echo -e "\x1b[92mCompiled\x1b[m bin/$(OUT_FILE)"

install: bin/$(OUT_FILE)
	@echo -e "\x1b[94mInstalling\x1b[m bin/$(OUT_FILE)"
	@sudo cp bin/$(OUT_FILE) /usr/bin/
	@echo -e "\x1b[92mInstalled\x1b[m $(OUT_FILE)"

uninstall: bin/$(OUT_FILE)
	@sudo rm -f /usr/bin/$(OUT_FILE)
	@echo -e "\x1b[91mUninstalled \x1b[m$(OUT_FILE)"

run:
	@echo -e "\x1b[92mRunning\x1b[m $(OUT_FILE)"
	@./bin/$(OUT_FILE)

clean veryclean:
	@rm -f bin/$(OUT_FILE)
	@echo -e "\x1b[91mRemoved \x1b[mbin/$(OUT_FILE)"
