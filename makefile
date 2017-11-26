default:
	g++ -c header.cpp
	g++ -c proxy.cpp
	g++ proxy.cpp header.cpp -o proxy
	g++ client.cpp -o client
