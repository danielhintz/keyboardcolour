all: link

run: link
	sudo ./main

compile :
	tcc -c main.c
link : compile
	tcc main.o -lusb-1.0 -lcmmk -o main
clean :
	-rm main.o main
