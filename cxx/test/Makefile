PROJECTROOT := ../..
OUTDIR := $(PROJECTROOT)/out
OBJDIR := $(OUTDIR)/obj

CXX := clang++
CXXFLAGS := -std=c++11 -stdlib=libc++ -O3 -fpie -fPIE -flto \
	-W -Wall -Wextra -Werror -pedantic -pedantic-errors
INCLUDEDIRS := -I$(PROJECTROOT)
LD := clang++
LDFLAGS := -stdlib=libc++ -flto
LINKLIBS := -L$(OUTDIR) -lcxxlib -lc++ -lclib -lc

$(OUTDIR)/test_cxx : $(OBJDIR)/test_cxx.o
	$(LD) -o $@ $(LDFLAGS) $< $(LINKLIBS)

$(OBJDIR)/test_cxx.o : test.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDEDIRS)

.PHONY : clean run

clean :
	rm -f $(OBJDIR)/test_cxx.o $(OUTDIR)/test_cxx

run : $(OUTDIR)/test_cxx
	cd $(OUTDIR) && ./test_cxx
