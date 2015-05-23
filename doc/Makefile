all: docs 


docs:
	doxygen ./conf/doxygen.config >> /dev/null

clean:
	rm -f refman.pdf brief_description.pdf
	rm -fR html/ latex/ rtf/ man/

