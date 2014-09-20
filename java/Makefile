PROJECTROOT := ..
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

JAVA := java
CLASSPATH := $(OUTDIR)
JAVAC := javac
JAVACFLAGS := -verbose -deprecation -Werror 
JAR := jar

 $(OUTDIR)/JavaLib.jar : $(OUTDIR)/libjavalib.dylib
	$(JAVAC) -d $(OBJDIR) $(JAVACFLAGS) -cp $(CLASSPATH) net/dotslashzero/javalib/*.java
	cd $(OBJDIR) && $(JAR) cvf ../JavaLib.jar net/

$(OUTDIR)/libjavalib.dylib : $(OBJDIR)/api_java.o
	$(LD) -o $@ $(LDFLAGS) $< $(LINKLIBS)

$(OBJDIR)/api_java.o : jni/api_java.cxx jni/api_java.hxx
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDEDIRS)

.PHONY : clean test runtest

clean :
	rm -fr $(OBJDIR)/api_java.o $(OUTDIR)/libjavalib.dylib $(OBJDIR)/net $(OUTDIR)/JavaLib.jar $(OUTDIR)/Test.class

test : $(OUTDIR)/JavaLib.jar
	$(JAVAC) -d $(OUTDIR) $(JAVACFLAGS) -cp $(OUTDIR)/JavaLib.jar:$(OUTDIR) test/Test.java

runtest : test
	$(JAVA) -Djava.library.path=$(OUTDIR) -cp $(OUTDIR)/JavaLib.jar:$(OUTDIR) Test
