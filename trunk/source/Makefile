all: cli_weathereport

cli_weathereport: weathereport.o curlget.o
	gcc weathereport.c curlget.c httpget.c -lcurl -Wall -o cli_weathereport

weathereport.o: weathereport.c
	gcc -c weathereport.c

curlget.o: curlget.c
	gcc -c curlget.c

httpget.o: httpget.c
	gcc -c httpget.c

clean:
	rm -rf *.o cli_weathereport
