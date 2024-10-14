Dependences = util.cpp syscall.cpp
Encapsulations = Epoll.cpp Channel.cpp InetAddress.cpp Socket.cpp

server:
	g++ -g -std=c++20 $(Dependences) $(Encapsulations) server.cpp -o server
	g++ -g -std=c++20 $(Dependences) client.cpp -o client

clean:
	rm -f server
	rm -f client