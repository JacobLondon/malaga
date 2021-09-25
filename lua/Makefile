
include ../include.mk

D_T = lua/Darwin.mk
L_T = lua/Linux.mk
W_T = lua/Windows.mk

$(D_T):
	cp Darwin.mk lua
$(L_T):
	cp Linux.mk lua
$(W_T):
	cp Windows.mk lua

# UNAME_S is one of the following:
Darwin: $(D_T)
Linux: $(L_T)
Windows: $(W_T)

LINIT_C = lua/linit.c
$(LINIT_C):
	cp mylinit.c $(LINIT_C)

mybuild:
	make -C lua -f $(UNAME_S).mk MYEXT=$(MYEXT)

# include requirements
release: debug
debug: $(UNAME_S) $(LINIT_C) mybuild

clean:
	make -C lua clean
