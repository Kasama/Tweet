all: main.c tweet.o
	gcc -std=c99 -o main main.c tweet.o
tweet.o: tweet.c
	gcc -std=c99 -c -o tweet.o tweet.c
run:
	./main
clean:
	rm *.o main
