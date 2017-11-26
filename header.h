#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;
//define cache size
#define maxCacheSize 10

class header{//header
/*Sample response header
Accept-Ranges	bytes
Cache-Control	max-age=60
Connection	Keep-Alive
Content-Encoding	gzip
Content-Length	19750
Content-Type	text/html; charset=UTF-8
Date	Tue, 21 Nov 2017 21:55:27 GMT
Expires	Tue, 21 Nov 2017 21:56:27 GMT
Keep-Alive	timeout=5, max=57
Server	Apache
VaryAccept-Encoding*/
public:
	//int cacheId;	
    //string contentEncoding;
    //string contentLength;
    //string contentType;
    string date;
    string expires;//
    string lastModified;
    
    time_t lastAccessed;//timestamp

public:
    header();
};

class cache:public header{
public:
	header* header;
  	string body;

  public:
  cache();

};

string getURL(char * recvbuffer);
cache* parseResponse(string response);
vector <string> parseString(string str, string delim);
cache* stamp(cache* newpage);
string receiveFromWebServer(string URL);
string receiveFromWebServerModified(string URL,string time);
void cacheInsert(string URL,cache* newpage);
cache* cacheMiss(string URL);
cache* cacheHit(string URL);









