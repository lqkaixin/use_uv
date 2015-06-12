
src = $(shell ls src/*.c)

obj = $(src:src/%.c=$(OUT_PATH)%.o)

dobj = $(obj:%=%.d)

all:$(obj)

include $(dobj)

$(obj):$(OUT_PATH)%.o: src/%.c
	-$(CC) $(CFLAGS) -o $@ $< $(LIBS)

$(dobj):$(OUT_PATH)%.o.d: src/%.c
	-$(CC) $(CFLAGS) -MM -MT $(OUT_PATH)$*.o -MF $@ $<
	sed -i 's,\(.*\),.o/\1.o $@,g' $@

clean:
	-rm $(obj) $(dobj)


