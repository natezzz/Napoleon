all:
	gcc napoleon.c error.c event.c thread.c -lev -pthread

debug:
	gcc napoleon.c error.c event.c thread.c -lev -pthread -g

clean:
	rm a.out
	
