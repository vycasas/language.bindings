PROJECTROOT := ..
OUTDIR := $(PROJECTROOT)/out
OBJDIR := $(OUTDIR)/obj

CC := clang
CFLAGS := -std=c11 -O3 -fpic -fPIC -fvisibility=hidden -flto \
	-W -Wall -Wextra -Werror -pedantic -pedantic-errors
INCLUDEDIRS := -I$(PROJECTROOT)
LD := clang
LDFLAGS := -shared -flto
LINKLIBS := -lc

$(OUTDIR)/libclib.dylib : $(OBJDIR)/api_c.o
	$(LD) -o $@ $(LDFLAGS) $^ $(LINKLIBS)

$(OBJDIR)/api_c.o : api.c api.h
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDEDIRS)

.PHONY : clean test runtest

clean :
	rm -f $(OBJDIR)/api_c.o $(OUTDIR)/libclib.dylib
	cd test && make clean

test : $(OUTDIR)/libclib.dylib
	cd test && make

runtest : test
	cd test && make run
