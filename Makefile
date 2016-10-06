all:
	gcc napoleon.c error.c event.c thread.c -lev -pthread

clean:
	rm a.out
	