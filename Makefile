
include include.mk

.PHONY: clean

BUILD_MODE=

debug: BUILD_MODE += debug
debug: bin
release: BUILD_MODE += release
release: bin

bin: mybuild
	mkdir -p bin/
	cp game/malaga bin/
	mkdir -p bin/maps
	cp data/maps/* bin/maps/
	mkdir -p bin/assets
	cp data/assets/* bin/assets/

mybuild:
	make -C rlu $(BUILD_MODE)
	make -C lua $(BUILD_MODE)
	make -C rlua $(BUILD_MODE)
	make -C data $(BUILD_MODE)
	make -C game $(BUILD_MODE)

clean:
	rm -rf bin
	make -C lua clean
	make -C data clean
	make -C rlu clean
	make -C rlua clean
	make -C game clean
