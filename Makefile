all: server client
server:server.cpp
	g++ -std=c++11 -o server server.cpp -pthread
	
client:client.cpp
	g++ -o client client.cpp
