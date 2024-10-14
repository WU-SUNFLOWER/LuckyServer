Source_Path = src/

Dependences = util.cpp syscall.cpp
PrefixedDependences = $(patsubst %.cpp, $(Source_Path)%.cpp, $(Dependences))

Encapsulations = Epoll.cpp Channel.cpp InetAddress.cpp Socket.cpp
PrefixedEncapsulations = $(patsubst %.cpp, $(Source_Path)%.cpp, $(Encapsulations))

server:
	g++ -g -std=c++20 $(PrefixedDependences) $(PrefixedEncapsulations) server.cpp -o server
	g++ -g -std=c++20 $(PrefixedDependences) client.cpp -o client

clean:
	rm -f server
	rm -f client