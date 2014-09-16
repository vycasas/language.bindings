PROJECTROOT := ..
OUTDIR := $(PROJECTROOT)/out
OBJDIR := $(OUTDIR)/obj

CXX := clang++
CXXFLAGS := -std=c++11 -stdlib=libc++ -O3 -fpic -fPIC -fvisibility=hidden -flto \
	-W -Wall -Wextra -Werror -pedantic -pedantic-errors
INCLUDEDIRS := -I$(PROJECTROOT)
LD := clang++
LDFLAGS := -stdlib=libc++ -shared -flto
LINKLIBS := -lc++ -L$(OUTDIR) -lclib -lc

$(OUTDIR)/libcxxlib.dylib : $(OBJDIR)/api_cxx.o
	$(LD) -o $@ $(LDFLAGS) $< $(LINKLIBS)

$(OBJDIR)/api_cxx.o : api.cxx api.hxx
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDEDIRS)

.PHONY : clean test runtest

clean :
	rm -f $(OBJDIR)/api_cxx.o $(OUTDIR)/libcxxlib.dylib
	cd test && make clean

test : $(OUTDIR)/libcxxlib.dylib
	cd test && make

runtest : test
	cd test && make run
