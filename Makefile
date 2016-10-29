all:
	gcc napoleon.c error.c event.c thread.c http.c picohttpparser.c -lev -pthread

debug:
	gcc napoleon.c error.c event.c thread.c http.c picohttpparser.c -lev -pthread -g

clean:
	rm a.out

