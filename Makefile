lmc: lmc.h lmc.c parser.h parser.c main.c 
	$(CC) -Wall lmc.c parser.c main.c -o lmc

clean:
	rm ./lmc