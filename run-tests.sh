#!/bin/sh

# usage: run-tests.sh --genall  to generate a new set of references
		 run-tests.sh			to verify the current code

if [ "$1" = '--gen' ]; then
	shift;
	debug/flashfilerdemo.exe $1 > $1.ref
	exit 0
fi

if [ "$1" = '--test' ]; then
	shift;
	debug/flashfilerdemo.exe $1 > $1.tmp
	diff $1.tmp $1.ref
	exit 0
fi

if [ "$1" = '--genall' ]; then
	ls data/*.[Ff][Ff]2 | xargs -n1 run-tests.sh --gen
	exit 0
fi

# by default, run all the tests. there should be NO output.
ls data/*.[Ff][Ff]2 | xargs -n1 run-tests.sh --test

