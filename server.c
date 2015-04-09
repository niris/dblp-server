#include <ctype.h>
#ifdef WIN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h> 
#define closesocket close
#endif
#define SEND(A) send(s,A,strlen(A),0)

#define X2C(c,i) ((c[i] >= 'A') ? (c[i]-'A')+10 : (c[i]-'0'))

char* unescape(char *url) {
	int x,y;
	for(x=0,y=0;url[x];x++,y++)
		if(url[y]=='%')url[x]=(X2C((url+y),1)<<4 | X2C((url+y),2)),y+=2;
		else if(url[y]=='+')url[x]=' ';
		else url[x]=url[y];
	url[x] = '\0';
	return url;
}

int server_alive=1;
int process(int s){
	char buf[4096],*meth,*path,*prot,*post;
	int plen=0,len=recv(s,buf,sizeof(buf),0);buf[len]=0;
	if(!(meth=strtok(buf," ")))return -__LINE__;
	if(!(path=strtok(NULL," ")))return -__LINE__;
	if(!(prot=strtok(NULL,"\n")))return -__LINE__;
	while(1){
		char*line=strtok(NULL, "\n");
		if(!line)return -__LINE__;
		if(!strchr(line,':')){//last header line
			if(strlen(line)!=1)return -__LINE__;//bad header
			plen=len-((line+2)-buf);
			if(plen)post=line+2;
			break;
		}
	}
	printf("%s %s",meth,path);
	SEND("HTTP/1.0 200 OK\nContent-Type:text/plain\n\n");
	if(!strcmp(path,"/kill"))
		return dblp_alive=server_alive=0;
	if(!strsw(path,"/year/")){//print article year from it ID
		return sprintf(buf,"%d",dates[atoi(path+6)]),SEND(buf);
	}
	if(!strsw(path,"/title/")){//print article title from it ID
		return SEND(titles[atoi(path+7)]);
	}
	if(!strsw(path,"/author/")){//print authors of an article
		u32 i,id=atoi(path+8);
		for(i=1;i<collab[id][0];i++)
			sprintf(buf,"%d\n",collab[id][i]),SEND(buf);
		return 0;
	}
	if(!strsw(path,"/name/")){//get authors name from it id
		return SEND(auteurs[atoi(path+6)]);
	}
	if(!strsw(path,"/byname/")){//get authors id from it name
		char*name=unescape(path+8);
		for(len=0;len<auteurs_length;len++){
			if(!strcmp(auteurs[len],name))return sprintf(buf,"%d\n",len),SEND(buf);
		}
		return SEND("no such name !");
	}
	if(!strsw(path,"/collab/")){//get every articles of an author
		u32 id=atoi(path+8);
		u32 i;for(i=0;i<articles_length;i++){
			u32 j;for(j=1;j<collab[i][0];j++){
				if(collab[i][j]!=id)continue;
				sprintf(buf,"%d\n",i),SEND(buf);
				break;
			}
		}
		return 0;
	}
	//info page
	return sprintf(buf,
		"Article[%i] API:\n"
		"============\n"
		"/year/#id  \tget article year\n"
		"/title/#id \tget article title\n"
		"/author/#id\tget article (co-)authors\n"
		"\nAuthor[%i] API:\n"
		"===========\n"
		"/byname/#name\tget author's id from it name\n"
		"/name/#id  \tget author's name\n"
		"/collab/#id\tget author's publication\n"
	,articles_length,auteurs_length),SEND(buf);
}


void*startServ(int*port){
	//printf("serve %i\n",*port);
	//return NULL;
#ifdef WIN
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData)){
		printf("WSAStartup failed\n");
		return NULL;
	}
#endif
	int in = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv.sin_port = htons(*port);
	bind(in, (struct sockaddr *) &serv, sizeof(serv));
	listen(in,5);
	printf("listening on port %i\n",*port);
	while (server_alive){
		int out;
		printf("\n>");
		if((out=accept(in, NULL, NULL))<0){
			printf("accept:%i,%i",in,out);
		}
		process(out);
		closesocket(out);
	}
	closesocket(in);
	return NULL;
}