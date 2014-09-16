PROJECTROOT := ../..
OUTDIR := $(PROJECTROOT)/out
OBJDIR := $(OUTDIR)/obj
JNI_INCLUDE_DIRS := \
	-I/Library/Java/JavaVirtualMachines/jdk1.8.0_20.jdk/Contents/Home/include \
	-I/Library/Java/JavaVirtualMachines/jdk1.8.0_20.jdk/Contents/Home/include/darwin

CXX := clang++
CXXFLAGS := -std=c++11 -stdlib=libc++ -O3 -fpic -fPIC -fvisibility=hidden -flto \
	-W -Wall -Wextra -Werror -pedantic -pedantic-errors
INCLUDEDIRS := $(JNI_INCLUDE_DIRS) -I$(PROJECTROOT)
LD := clang++
LDFLAGS := -stdlib=libc++ -shared -flto
LINKLIBS := -L$(OUTDIR) -lcxxlib -lclib -lc++ -lc

$(OUTDIR)/libjavalib.dylib : $(OBJDIR)/api_java.o
	$(LD) -o $@ $(LDFLAGS) $< $(LINKLIBS)

$(OBJDIR)/api_java.o : api_java.cxx api_java.hxx
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDEDIRS)

.PHONY : clean

clean :
	rm -f $(OBJDIR)/api_java.o $(OUTDIR)/libjavalib.dylib
