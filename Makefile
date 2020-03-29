all: link

install : link
	cp keyboardcolour ~/.local/bin/keyboardcolour

compile :
	tcc -c main.c
link : compile
	tcc main.o -lusb-1.0 -lcmmk -lm -o keyboardcolour
clean :
	-rm main.o keyboardcolour
