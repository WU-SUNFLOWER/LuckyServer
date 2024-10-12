Dependences = util.cpp syscall.cpp

server:
	g++ -std=c++20 $(Dependences) server.cpp -o server
	g++ -std=c++20 $(Dependences) client.cpp -o client

clean:
	rm -f server
	rm -f client