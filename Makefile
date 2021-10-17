
include include.mk

.PHONY: clean

BUILD_MODE=
COMPRESS_TOOL=
COMPRESS_EXT=

help:
	@echo 'Options:'
	@echo '  debug'
	@echo '  release'
	@echo '  zip'
	@echo '  tar'
	@echo '  clean'
	@echo '  nopack'
	@echo '  clobber'

debug: BUILD_MODE += debug
debug: $(PROJECT)
release: BUILD_MODE += release
release: $(PROJECT)

$(PROJECT): mybuild
	mkdir -p $(PROJECT)
	cp game/malaga $(PROJECT)
	mkdir -p $(PROJECT)/maps
	cp data/maps/* $(PROJECT)/maps/
	mkdir -p $(PROJECT)/assets
	[ -d data/assets ] && cp data/assets/* $(PROJECT)/assets/ || true

zip: COMPRESS_TOOL = zip -r
zip: COMPRESS_EXT = zip
zip: release compress
tar: COMPRESS_TOOL = tar czf
tar: COMPRESS_EXT = tar.gz
tar: release compress
compress:
	$(COMPRESS_TOOL) $(PROJECT).$(COMPRESS_EXT) $(PROJECT)

mybuild:
	make -C collections $(BUILD_MODE)
	make -C rlu $(BUILD_MODE)
	make -C lua $(BUILD_MODE)
	make -C rlua $(BUILD_MODE)
	make -C game $(BUILD_MODE)
	make -C data $(BUILD_MODE)

play: $(PROJECT)
	$(shell cd $(PROJECT) && ./$(PROJECT) ; cd ..)

clean:
	$(RM_F) bin
	make -C rlu clean
	make -C rlua clean
	make -C game clean
	make -C data clean

nopack:
	$(RM_F) $(PROJECT).zip $(PROJECT).tar.gz

clobber: clean nopack
	make -C collections clean
	make -C lua clean
