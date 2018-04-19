my_server: my_server.c
	gcc my_server.c -o my_server -lpthread
clean:
	-rm -f my_server
