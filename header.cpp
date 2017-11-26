#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <limits.h>

#include "header.h"
using namespace std;

//char* format = "%a, %d %b %Y %H:%M:%S %Z";
//map<string, cache*> cacheEntry;
header::header(){}
cache::cache(){}
string getURL(char * recvbuffer){
	string URL;
	char* token = strtok(recvbuffer," ");
	char* tokens[3];
	int i=0;  
    while(token!=NULL)  
    {  
         tokens[i]=token; 
         token=strtok(NULL," "); 
         i++; 
    }
    return tokens[1];
}

cache* parseResponse(string response){
	header* httpheader=new header;
	cache* proxycache=new cache;
        string reresponse=response;
	string delim="\r\n\r\n"; //split header and body
    vector<string> rec = parseString(response, delim); 

  if(rec.size() == 1 || rec.size() > 2)
  {
    cout<<"Reponse error. Exiting..."<<endl;
    exit(1);
  }
  //parse Header
  string head = rec.at(0);
  string delim2 = "\r\n";
  vector<string> headercontent = parseString(head, delim2);
  if(headercontent.size() == 0)
  {
    cout<<"Header error. Exiting..."<<endl;
    exit(1);
  }

  int size = headercontent.size();
  vector<string> contents;
  string delim3 = ": ";
  for(int i=1; i<size; i++) 
  {
    contents = parseString(headercontent.at(i), delim3);

    if(contents.at(0)=="Date") httpheader->date=contents.at(1);
    else if(contents.at(0) == "Expires")
      httpheader->expires=contents.at(1);
    else if(contents.at(0) == "Last-Modified")
      httpheader->lastModified=contents.at(1);

  }

  //get current time
  
  time_t tt=time(NULL);
  struct tm* time;
  time=gmtime(&tt);
  time_t current = timegm(time); 
  httpheader->lastAccessed=current;
  
  //add in the cache
  proxycache->header=httpheader;
  proxycache->body=reresponse;

  return proxycache;
}
vector <string> parseString(string str, string delim){
	vector<string> tokens;
	int i=0;
	int length=str.length();
	size_t pos;
	while(i<length){
		pos=str.find(delim,i);
		if(pos==string::npos){//end of string
			tokens.push_back(str.substr(i));
			break;

		}
		tokens.push_back(str.substr(i,int(pos)-i));
		i=int(pos)+delim.length();
	}
	return tokens;
}
cache* stamp(cache* newpage){//stamp(cache is not full)
	header* header=newpage->header;
	time_t tt=time(NULL);
    struct tm* time;
    time=gmtime(&tt);
    time_t current = timegm(time); 
    header->lastAccessed=current;
	newpage->header=header;

	return newpage;
}

