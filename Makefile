include conf.mk

W_FLAGS=$(addprefix -W, $(WARNINGS))
L_FLAGS=$(addprefix -l, $(LINKS))

C_FILES=$(shell find src -type f -name '*.c')
O_FILES=$(patsubst src/%.c,obj/%.o,$(C_FILES))
D_FILES=$(patsubst src/%.c,dep/src/%.d,$(C_FILES))

col_dim=\e[0;34m
col_bright=\e[1;95m
col_arg=\e[0;92m
col_end=\e[0m
log=echo -e "$(col_dim)[$(col_bright)i$(col_dim)]$(col_end) $(1) $(col_arg)$(2)$(col_end)"

$(PROJECT): $(O_FILES)
	$(CC) $(FLAGS) $(W_FLAGS) $(L_FLAGS) $(O_FILES) -o $(PROJECT)
	$(call log,"Created $(PROJECT).")

run: $(PROJECT)
	$(PROJECT)

debug: FLAGS=$(FLAGS-DBG)
debug: $(PROJECT)

test: debug
	valgrind $(FLAGS-VALGRIND) ./$(PROJECT)

install: $(PROJECT)
	cp $(PROJECT) $(PREFIX)

uninstall:
	rm $(PREFIX)/$(PROJECT)

clean:
	rm -fr dep
	rm -fr obj
	rm -f $(PROJECT)
	rm -f vgcore.*

dep/src/%.d: src/%.c
	mkdir -p $(@D)
	printf $(dir obj/$*) > $@
	gcc -MM $(W_FLAGS) $< -o - >> $@
	$(call log,"Created dep file",$@)

include $(D_FILES)

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $(FLAGS) $(W_FLAGS) $(L_FLAGS) --std=$(STD) $< -o $@
	$(call log,"Created object file",$@)

.PHONY: run debug test install uninstall clean
.SILENT:
