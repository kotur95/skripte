NEDELJA1=kk-ned-01
NEDELJA2=kk-ned-02
NEDELJA3=kk-ned-03

all: $(NEDELJA1).pdf $(NEDELJA2).pdf $(NEDELJA3).pdf

.PHONY: clean, cleanall

clean:
	\rm *.pdf

$(NEDELJA1).pdf:$(NEDELJA1).md
	./make.sh $(NEDELJA1)

$(NEDELJA2).pdf:$(NEDELJA2).md
	./make.sh $(NEDELJA2)

$(NEDELJA3).pdf:$(NEDELJA3).md
	./make.sh $(NEDELJA3)
