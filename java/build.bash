#!/bin/bash

CURRDIR=`pwd`
OUTDIR=$CURRDIR/../out

rm -fr $OUTDIR/net
rm -fr $OUTDIR/JavaLib.jar

javac -verbose -deprecation -cp $OUTDIR:$CURRDIR -d $OUTDIR -Werror net/dotslashzero/javalib/*.java
cd $OUTDIR
jar cvf JavaLib.jar net/
cd $CURRDIR

# Generate headers with following command
# javah -o $SRCDIR/japi_<class>.h -v -cp $OUTDIR/JavaLib.jar:. net.dotslashzero.javalib.<class>
