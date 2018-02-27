NEDELJA1=kk-ned-01
NEDELJA2=kk-ned-02

all: $(NEDELJA1).pdf $(NEDELJA2).pdf

.PHONY: clean, cleanall

clean:
	\rm *.pdf

$(NEDELJA1).pdf:$(NEDELJA1).md
	./make.sh $(NEDELJA1)

$(NEDELJA2).pdf:$(NEDELJA2).md
	./make.sh $(NEDELJA2)
