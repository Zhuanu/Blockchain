CFLAGS = -g -ggdb -Wall -Wextra -pedantic

all: main

main: main.o exo1.o exo2.o exo3.o exo4.o exo5.o exo6.o exo7.o exo8.o exo9.o
	gcc -o main main.o exo1.o exo2.o exo3.o exo4.o exo5.o exo6.o exo7.o exo8.o exo9.o $(CFLAGS) -lssl -lcrypto 
main.o: main.c exo.h        
	gcc -c $(CFLAGS) main.c
exo1.o: exo1.c exo.h
	gcc -c $(CFLAGS) exo1.c
exo2.o: exo2.c exo.h 
	gcc -c $(CFLAGS) exo2.c
exo3.o: exo3.c exo.h
	gcc -c $(CFLAGS) exo3.c
exo4.o: exo4.c exo.h
	gcc -c $(CFLAGS) exo4.c
exo5.o: exo5.c exo.h
	gcc -c $(CFLAGS) exo5.c
exo6.o: exo6.c exo.h     
	gcc -c $(CFLAGS) exo6.c
exo7.o: exo7.c exo.h       
	gcc -c $(CFLAGS) exo7.c
exo8.o: exo8.c exo.h       
	gcc -c $(CFLAGS) exo8.c
exo9.o: exo9.c exo.h       
	gcc -c $(CFLAGS) exo9.c

clean:
	rm -f *.o
