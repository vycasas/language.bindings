#!/bin/bash

CURRDIR=`pwd`
OUTDIR=$CURRDIR/../../out

javac -verbose -deprecation -cp $OUTDIR/JavaLib.jar:$CURRDIR -d $OUTDIR -Werror Test.java

cd $OUTDIR
java -Djava.library.path=$OUTDIR -cp $OUTDIR/JavaLib.jar:$OUTDIR Test

cd $CURRDIR
