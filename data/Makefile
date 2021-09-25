
include ../include.mk

.PHONY: clean

GENERATE = generate.mk

# convert 'maps/test.lua' -> '\t$(LUA) maps/test.lua > maps/test.mg' and put lines into generated makefile by rule 'generate'
$(GENERATE):
	echo 'generate:' > $(GENERATE)
	find maps | grep -v 'mg\.lua' | grep '\.lua' | sed 's/.lua//g' | xargs -I {} echo {} {} | xargs printf '\t$$(LUA) %s.lua > %s.mg\n' >> $(GENERATE)

mybuild: $(GENERATE)
	make -f $(GENERATE) generate LUA=$(LUA)

release: mybuild
debug: mybuild

clean:
	$(RM_F) $(GENERATE) maps/*.mg
