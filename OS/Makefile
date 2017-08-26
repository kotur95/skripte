FILE=os

all: $(FILE).pdf

.PHONY: clean, cleanall
clean:
	\rm *.aux *.blg *.out *.bbl *.log *.toc

cleanall:
	\rm *.pdf
	make clean

$(FILE).pdf:$(FILE).tex
	./make.sh $(FILE).tex
