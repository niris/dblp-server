#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#define strsw(A,B) strncmp(A,B,strlen(B))
#include <unistd.h>
#include <fcntl.h>

#include "dblp.c"
#include "server.c"
#ifdef PTHREAD
#include <pthread.h>
#endif

int main(int argc,char** argv){
	if(argc<=2)return printf("USAGE:\n%s xml_file port",argv[0]);
	int port=atoi(argv[2]);
#ifdef PTHREAD
	pthread_t dblp,serv;
	pthread_create(&dblp, NULL, (void *) &parseFile, argv[1]);
	pthread_create(&serv, NULL, (void *) &startServ, &port);
	pthread_join(serv, NULL);
	pthread_join(dblp, NULL);
#else
	parseFile(argv[1]);// dblp.uni-trier.de/xml
	startServ(&port);
#endif
	return 0;
}
