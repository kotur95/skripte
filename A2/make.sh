#!/bin/bash

clear

echo " -- LaTeX Compilation Errors --"
echo ""


ARGUMENT="$1"
#determine aux name by stripping .tex suffix and adding .aux
AUXNAME="${ARGUMENT%.tex}.aux"
xelatex -shell-escape -interaction=nonstopmode -file-line-error "$ARGUMENT" | grep -i ".*:[0-9]*:.*\|warning" 
bibtex -terse "$AUXNAME"  
xelatex -shell-escape -interaction=nonstopmode -file-line-error "$ARGUMENT" | grep -i ".*:[0-9]*:.*\|warning" 

# ps axf | grep latexPDF | grep -v grep | awk '{print "kill -s SIGHUP " $1}' | bash

