all: implementation.pdf 

%.pdf:%.dvi
	dvipdf $^
	mupdf $@

%.dvi:%.tex 
	latex $^
	bibtex bibliography.bib
	latex $^
	latex $^
