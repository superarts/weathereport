all: cli_weathereport

cli_weathereport: weathereport.o httpget.o utils.o cache.o yahoo.o
	gcc weathereport.c httpget.c utils.c cache.c yahoo.c -Wall -o cli_weathereport

weathereport.o: weathereport.c
	gcc -c weathereport.c

httpget.o: httpget.c
	gcc -c httpget.c

utils.o: utils.c
	gcc -c utils.c

cache.o: cache.c
	gcc -c cache.c

yahoo.o: yahoo.c
	gcc -c yahoo.c

clean:
	rm -rf *.o cli_weathereport
