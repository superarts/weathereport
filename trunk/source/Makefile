all: cli_weathereport

cli_weathereport: weathereport.o httpget.o
	gcc weathereport.c httpget.c -Wall -o cli_weathereport

weathereport.o: weathereport.c
	gcc -c weathereport.c

httpget.o: httpget.c
	gcc -c httpget.c

clean:
	rm -rf *.o cli_weathereport
