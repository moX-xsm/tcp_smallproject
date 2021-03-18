all:
	gcc ./src/server.c ./src/tcp_server.c -o ./bin/server -lpthread
	gcc ./src/client.c ./src/tcp_client.c -o ./bin/client -lpthread
clean:
	rm ./bin/*
server:
	./bin/server 8888
client:
	./bin/client 192.168.226.138 8888
