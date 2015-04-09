#include <sys/time.h>

#define FILESIZE(A) (fseek(A,0,SEEK_END),ftell(A));fseek(A,0,SEEK_SET);
#define MAX_LINE_LENGTH 1500//1231
#define TOTAL_ARTICLES 4000000
#define TOTAL_AUTEURS 2000000
#define MAX_ARTICLE_COLAB 500
#define F_LOOKUP 1<<0
#define F_STRING 1<<1
#define F_INCREM 1<<2
typedef unsigned u32;
typedef struct{char**table;u32 max;u32 len;} Table;

u32 articles_length=0;
char* titles  [TOTAL_ARTICLES];
short dates   [TOTAL_ARTICLES];
u32*  collab  [TOTAL_ARTICLES];

u32 auteurs_length =0;
char* auteurs [TOTAL_AUTEURS];
//u32*  collab  [TOTAL_ARTICLES];
int dblp_alive=1;

u32 lookup(void**table,u32 len,char*title,u32 size){
	u32 pos;
	for(pos=0;pos<len;pos++)
		if(!memcmp(table[pos],title,size))
			return pos;
	return ~0;
}
u32 push(void**table,u32*_pos,char*data,u32 size,u32 flag){
	u32 id,pos=*_pos,msize=(flag&F_STRING)?size+1:size;
	if((flag&F_STRING))data[size]=0;
	if((flag&F_LOOKUP) && ((id=lookup(table,pos,data,size))!=~0))return id;
	memcpy(table[pos]=malloc(msize),data,msize);
	if(flag&F_INCREM)*_pos=*_pos+1;
	return pos;
}
u32 art_col[1+MAX_ARTICLE_COLAB]={1};
u32 parseLine(char*line,u32 len,u32 l){
	if(!strsw(line,"<year>"))
		return dates[articles_length-1]=atoi(line+6);
	if(!strsw(line,"<author>")){
		return art_col[art_col[0]++]=push((void**)auteurs,&auteurs_length ,line+8,len-(8+9), F_LOOKUP | F_STRING | F_INCREM);
	}
	if(!strsw(line,"<title>")){
		u32 id=push((void**)titles ,&articles_length,line+7,len-(7+8),F_INCREM | F_STRING);
		push((void**)collab ,&id,(char*)&art_col,art_col[0]*sizeof(u32),0);
		art_col[0]=1;
		return id;
	}
	return -1;
}

void* parseFile(char*path){
	//printf("parse %s\n",path);
	//return NULL;
	u32 p,line;
	char c,buffer[MAX_LINE_LENGTH],percent=-1;
	FILE*f=fopen(path,"r");
	for(p=0,line=0;fread(&c,1,1,f) && dblp_alive;p++){
		if(c=='\n'||c=='\r'){
			line++;
			buffer[p]=0;
			parseLine(buffer,p,line);
			char pc=((float)line/(float)31923557)*100;
			if(pc!=percent)
				printf("%i%% Lines:%u\tArticles:%u\tAuteurs:%u\r",percent=pc,line,articles_length,auteurs_length);//31923557
			//if(articles_length>100)break;
			p=-1;
		}else{
			buffer[p]=c;
		}
	}
	fclose(f);
	return NULL;
}