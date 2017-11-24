#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//define cache size
#define maxCacheSize 10;

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
	header* header;
  	string body;

  public:
  cache();

}

string getURL(char * recvbuffer){
	string URL;
	string token = strtok(recvbuffer," ");
	char* tokens[3];
	int i=0  
    while(token !=NULL)  
    {  
         tokens[i++]=token; 
         token=strtok(NULL," ");  
    }
    return tokens[1];
}
cache* parseResponse(string response){
	header* =new header;
	cache* =new cache;
	string delim="\r\n\r\n"; //split header and body
    vector<string> recv = parseString(response, delim); 

  if(recv.size() == 1 || recv.size() > 2)
  {
    cout<<"Reponse error. Exiting..."<<endl;
    exit(1);
  }
  //parse Header
  string header = recv.at(0);
  delim2 = "\r\n";
  vector<string> headercontent = parseString(header, delim2);
  if(headercontent.size() == 0)
  {
    cout<<"Header error. Exiting..."<<endl;
    exit(1);
  }

  int size = headercontent.size();
  vector<string> contents;
  delim3 = ": ";
  for(int i=1; i<numFields; i++) 
  {
    contents = parseString(headercontent.at(i), delim3);

    if(contents.at(0)=="Date") header->date=contents.at(1);
    else if(contents.at(0) == "Expires")
      header->expires=contents.at(1);
    else if(contents.at(0) == "Last-Modified")
      header->lastModifies=contents.at(1);

  }

  //get current time
  
  time_t tt=time(NULL);
  struct tm* time;
  time=gmtime(&tt);
  time_t current = timegm(time); 
  header->lastAccessed=current;
  
  //add in the cache
  cache->header=header;
  cache->body=response;

  return cache;
}
vector <string> parseString(string str, string delim){
	vector<string> tokens;
	int i=0;
	int length=str.length();
	size_t pos=str.find(delim,i);
	while(i<length){
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
	header* header=newpage.header;
	time_t tt=time(NULL);
    struct tm* time;
    time=gmtime(&tt);
    time_t current = timegm(time); 
    header->lastAccessed=current;
	newpage.header=header;

	return newpage;
}
string receiveFromWebServer(string URL){
	string server,filename;
	char errorbuffer[256],recvbuffer[1000000];
	int point1,point2;
	point1=URL.find("/");
	//input http://or https://
	if(URL.at(point1)=="/"&&URL.at(point1+1)=="/"){
		point2=URL.substr(point1+2,URL.length()-1).find("/");
		server=URL.substr(point1,point2);
		filename=URL.substr(point1+point2+2,URL.length()-1);
	}
	else{//input no //
		server=URL.substr(0,point1);
		filename=URL.substr(point1,URL.length()-1);
	}
	//retrieve filename from webserver
	int httpsd;
	if((httpsd)=socket(AF_INET,SOCK_STREAM,0)==-1){
		fprintf(stderr, "%s\n", "HTTP socket creation error");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);
	}

	struct addrinfo *httpinfo;
	struct addrinfo hints;
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	if(getaddrinfo(server.c_str(),"http",&hints,&httpinfo)!=0){
		fprintf(stderr,"getaddrinfo error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);
	}
	if(connect(httpsd,httpinfo->ai_addr,httpinfo->ai_addrlen)==-1){
		fprintf(stderr,"connect error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);
	}
	string message="GET"+URL+"HTTP/1.0\r\n\r\n";
	cout<<message<<endl;
	int bytes=0;
	if((bytes=send(httpsd,message.c_str(),message.length(),0)<=0){
		fprintf(stderr,"send error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);		
	}
	int bytes2=0;
	if((bytes2=recv(httpsd,recvbuffer,1000000,0)<=0){
		fprintf(stderr,"recv error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);		
	}
	strncpy(str,recvbuffer,bytes2);
	return str;
}

string receiveFromWebServerModified(string URL,string time){
	string server,filename;
	char errorbuffer[256],recvbuffer[1000000];
	int point1,point2;
	point1=URL.find("/");
	//input http://or https://
	if(URL.at(point1)=="/"&&URL.at(point1+1)=="/"){
		point2=URL.substr(point1+2,URL.length()-1).find("/");
		server=URL.substr(point1,point2);
		filename=URL.substr(point1+point2+2,URL.length()-1);
	}
	else{//input no //
		server=URL.substr(0,point1);
		filename=URL.substr(point1,URL.length()-1);
	}
	//retrieve filename from webserver
	int httpsd;
	if((httpsd)=socket(AF_INET,SOCK_STREAM,0)==-1){
		fprintf(stderr, "%s\n", "HTTP socket creation error");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);
	}

	struct addrinfo *httpinfo;
	struct addrinfo hints;
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	if(getaddrinfo(server.c_str(),"http",&hints,&httpinfo)!=0){
		fprintf(stderr,"getaddrinfo error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);
	}
	if(connect(httpsd,httpinfo->ai_addr,httpinfo->ai_addrlen)==-1){
		fprintf(stderr,"connect error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);
	}
	string message="GET"+URL+"HTTP/1.0\r\n\r\n";
	cout<<message<<endl;
	int bytes=0;
	if((bytes=send(httpsd,message.c_str(),message.length(),0)<=0){
		fprintf(stderr,"send error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);		
	}
	int bytes2=0;
	if((bytes2=recv(httpsd,recvbuffer,1000000,0)<=0){
		fprintf(stderr,"recv error\n");
		char* errorMessage=strerror_r(errno,errorbuffer,256);
		exit(1);		
	}
	string str;
	strncpy(str,recvbuffer,bytes2);
	string temp=str;
	vector<string> contents=parseString(temp,"\r\n");
	string status=contents.at(0);
	vector<string> modify=parseString(status," ");
	if(modify.at(1)!="304")return str;
	else return 0;

}
void cacheInsert(string URL,cache* newpage){
	if(cacheEntry.size()<maxCacheSize){
		newpage=stamp(newpage);//find invalid cache entry
		cacheEntry.insert(make_pair(URL,newpage));
	}
	else{//cache is full, find a victim
		//int i=0;
		time_t tt=time(NULL);
    	struct tm* time;
    	time=gmtime(&tt);
    	time_t current = timegm(time);
    	map<string,cache*>::iterator victim;
    	double timediff,maxTime=INT_MIN;

    	map<string,cache*>::iterator vic=cacheEntry.begin();
    	for(;vic!=cacheEntry.end();vic++){
    		cache* newpage=vic->second;
    		time_t lastaccessed=newpage.header.lastAccessed;
    		if(timediff>maxTime){
    			maxTime=timediff;
    			victim=vic;
    		}

    	}

    	cacheEntry.erase(victim);
    	cacheEntry.insert(make_pair(URL,newpage));

	}

}
void cacheUpdate(string URL, cache* newpage){

}
cache* cacheMiss(string URL){
	cout<<"cache miss..."<<endl;
	string page=receiveFromWebServer(URL);
	cache* newpage=parseReponse(URL);
	cacheInsert(URL,newpage);

	return newpage;
}
cache* cacheHit(string URL){
	map<string,cache*>::iterator hit=cache.find(URL);
	cache* entry=hit->second;

	string expire=entry.header.expires;
	if(expire.empty()){
		expire=entry.header.lastModified;
		if(expire.empty()){
			expire=entry.header.date;
		}
	}
   struct tm tm1;
   char* ret = strptime(t.c_str(), format, &tm1);
  if(ret == NULL)
  {
    cout<<"The string "<<t<<" was not recognized as a date format"<<endl;
    exit(1);
  }
  time_t t1 = timegm(&tm1);
  int ex;
  if(t1<=0)ex=-1;
  else{
  	ex=int(t1);
  }
	time_t tt=time(NULL);
    struct tm* time;
    time=gmtime(&tt);
    time_t current = timegm(time);
    int curr=int(current);
    if(current>ex){
    	string modifypage=receiveFromWebServerModified(URL,expires);
    	if(modifypage.empty()){
    		entry=stamp(entry);
    		return entry;
    	}else{
    		cache* newpage=parseReponse(modifypage);
    		cacheEntry.insert(make_pair(URL,newpage));
    		return newpage;

    	}
    }else{
    	entry=stamp(entry);
    	return entry;
    }

}