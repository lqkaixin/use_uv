


#CFLAGS += -DDEBUG_UTILITY

#CFLAGS += -I$(shell pwd)/include/ -Wall

export CFLAGS = -Iinclude/
export LIBS = lib/libcommon.a
export OUT_PATH=out/

all:subsystem

subsystem:
	cd . && $(MAKE) -f src/makefile
	
	
clean:
	cd . && $(MAKE) clean -f src/makefile
