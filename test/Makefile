# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make"
#
# The compilation rules are defined in $(TOPDIR)/Makefile.user
# Please don't update them
#
# To add generate a new program, just update the PROGRAMS target below

PROGRAMS = kevin ryan halt hello shell matmult sort simple-semaphore testall testnewpss

all: $(PROGRAMS)

TOPDIR = ../
include $(TOPDIR)/Makefile.user

# Dependances
$(PROGRAMS): % : $(USERLIB)/sys.o $(USERLIB)/libnachos.o %.o

