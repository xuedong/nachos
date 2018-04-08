GCC = gcc -Wall -Werror -lpthread

SOURCES = $(wildcard src/*.c)
DEPS = $(wildcard src/*.cc)
TARGETS = $(SOURCES:src/%.c=bin/%)
OBJECTS = $(DEPS:src/%.cc=bin/%.o)

all: $(TARGETS)

TOPDIR = ../../
include $(TOPDIR)/Makefile.user

bin/%:src/%.c
	$(GCC) $(USERLIB)/sys.o $(USERLIB)/libnachos.o $^ -o $@
