PROJECTROOT := ..
OUTDIR := $(PROJECTROOT)/out
OBJDIR := $(OUTDIR)/obj

OC := clang
OFLAGS := -std=c11 -O3 -fobjc-arc -fpie -fPIE -flto \
	-W -Wall -Wextra -Werror -pedantic -pedantic-errors
OXX := clang++
OXXFLAGS := -std=c++11 -stdlib=libc++ -O3 -fobjc-arc -fpic -fPIC -fvisibility=hidden -flto \
	-W -Wall -Wextra -Werror -pedantic -pedantic-errors
INCLUDEDIRS := -I$(PROJECTROOT)
LD := clang++
LDFLAGS := -stdlib=libc++ -flto
LINKLIBS := -framework Foundation -lobjc -L$(OUTDIR) -lcxxlib -lc++ -lclib -lc
AR := ar
ARFLAGS := rcs

$(OUTDIR)/libobjclib.a : $(OBJDIR)/api_objc.o
	$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)/api_objc.o : OLApi.mm OLApi.h
	$(OXX) -o $@ -c $< $(OXXFLAGS) $(INCLUDEDIRS)

$(OUTDIR)/test_objc : $(OBJDIR)/test_objc.o $(OUTDIR)/libobjclib.a
	$(LD) -o $@ $(LDFLAGS) $^ $(LINKLIBS)

$(OBJDIR)/test_objc.o : test/test.m
	$(OC) -o $@ -c $< $(OFLAGS) $(INCLUDEDIRS)

.PHONY : clean test runtest

clean :
	rm -f $(OBJDIR)/api_objc.o $(OUTDIR)/libobjclib.a
	rm -f $(OBJDIR)/api_objc.o $(OUTDIR)/test_objc

test : $(OUTDIR)/test_objc

runtest : test
	cd $(OUTDIR) && ./test_objc
	
