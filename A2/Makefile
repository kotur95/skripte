FILE=integrali

all: $(FILE).pdf

.PHONY: clean, cleanall
clean:
	\rm *.aux *.blg *.out *.bbl *.log *.toc *.pdf

$(FILE).pdf:$(FILE).tex
	./make.sh $(FILE).tex
