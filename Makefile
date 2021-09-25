
include include.mk

.PHONY: clean

BUILD_MODE=

debug: BUILD_MODE += debug
debug: bin
release: BUILD_MODE += release
release: bin

bin: mybuild
	mkdir -p bin
	cp game/malaga bin
	cp -r data/* bin/

mybuild:
	make -C rlu $(BUILD_MODE)
	make -C game $(BUILD_MODE)

clean:
	rm -rf bin
	make -C rlu clean
	make -C game clean
