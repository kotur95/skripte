#!/bin/bash

clear

echo "Compiling $1 to PDF ..."
ARGUMENT="$1"
pandoc $ARGUMENT.md --template cpp.latex --listings --pdf-engine=xelatex -o $ARGUMENT.pdf

ps axf | grep epdfview | grep -v grep | awk '{print "kill -s SIGHUP " $1}' | bash
