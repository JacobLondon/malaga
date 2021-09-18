
RM_F = rm -f

MK_LINUX = lua/linux.mk
MK_DARWIN = lua/darwin.mk
MK_WINDOWS = lua/windows.mk
MK_ALL = $(MK_LINUX) $(MK_DARWIN) $(MK_WINDOWS)

all: $(MK_ALL)

linux: $(MK_LINUX)
	make -C lua -f linux.mk

darwin: $(MK_DARWIN)
	make -C lua -f darwin.mk

windows: $(MK_WINDOWS)
	make -C lua -f windows.mk

$(MK_LINUX):
	cat lua/makefile | \
		sed 's/ -DLUA_USE_READLINE//g' | \
		sed 's/ -lreadline//g' \
		> $(MK_LINUX)

# Note: MinGW needs '-D__need___off64_t' for MYCFLAGS
$(MK_WINDOWS):
	cat lua/makefile | \
		sed 's/ -DLUA_USE_LINUX -DLUA_USE_READLINE//g' | \
		sed 's/,-E//g' | \
		sed 's/ -ldl -lreadline//g' | \
		sed 's/CC= gcc/CC= tcc/g' | \
		sed 's/ -lm//g' \
		> $(MK_WINDOWS)

$(MK_DARWIN):
	cat lua/makefile | \
		sed 's/ -DLUA_USE_LINUX -DLUA_USE_READLINE//g' | \
		sed 's/,-E//g' | \
		sed 's/ -ldl -lreadline//g' | \
		sed 's/(CWARNGCC)/(_myrmCWARNGCC)/g' | \
		sed 's/CC= gcc/CC= clang/g' \
		> $(MK_DARWIN)

coll:
	make -C collections release

clean:
	$(RM_F) $(MK_ALL)
	make -C collections clean
