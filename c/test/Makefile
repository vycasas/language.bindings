PROJECTROOT := ../..
OUTDIR := $(PROJECTROOT)/out
OBJDIR := $(OUTDIR)/obj

CC := clang
CFLAGS := -std=c11 -O3 -fpie -fPIE -flto \
	-W -Wall -Wextra -Werror -pedantic -pedantic-errors
INCLUDEDIRS := -I$(PROJECTROOT)
LD := clang
LDFLAGS := -flto
LINKLIBS := -L$(OUTDIR) -lclib -lc

$(OUTDIR)/test_c : $(OBJDIR)/test_c.o
	$(LD) -o $@ $(LDFLAGS) $< $(LINKLIBS)

$(OBJDIR)/test_c.o : test.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDEDIRS)

.PHONY : clean run

clean :
	rm -f $(OBJDIR)/test_c.o $(OUTDIR)/test_c

run : $(OUTDIR)/test_c
	cd $(OUTDIR) && ./test_c
