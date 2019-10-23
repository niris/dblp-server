#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINE_LENGTH 2500//1231
#define MAX_AUTHOR 500

#define eq(A,B) (!memcmp(A,B,sizeof(B)-1))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
unsigned is_article=0;
unsigned num_autors=0;
unsigned num_article=0;

char col_author[MAX_AUTHOR][MAX_LINE_LENGTH];
char col_title[MAX_LINE_LENGTH];
char col_year[MAX_LINE_LENGTH];
int line_num=0;FILE*f;

unsigned parseLine(char*line,unsigned len){
	line_num++;
	char*end=NULL;
	//fprintf(stderr,"\t%s <%li>\n",line,ftell(f));

	if(eq(line,"<article "  )
	|| eq(line,"<inproceedings "  )
	|| eq(line,"<proceedings "  )
	|| eq(line,"<book "  )
	|| eq(line,"<incollection "  )
	|| eq(line,"<phdthesis "  )
	|| eq(line,"<mastersthesis "  )
	|| eq(line,"<www "  )
	){
		int i;for(i=0;i<num_autors;i++)col_author[i][0]='\0';
		num_article++;
		num_autors=0;
		col_title[0]=col_year[0]='\0';
		is_article=1;
	}
	if(eq(line,"</article"  )
	|| eq(line,"</inproceedings"  )
	|| eq(line,"</proceedings"  )
	|| eq(line,"</book"  )
	|| eq(line,"</incollection"  )
	|| eq(line,"</phdthesis"  )
	|| eq(line,"</mastersthesis"  )
	|| eq(line,"</www"  )
	){
		if(!num_autors && line[3]!='w')fprintf(stderr,"vide  : %s %s line:%i @<%li>\n",line,col_title,line_num,ftell(f));
		int i;for(i=0;i<num_autors;i++)
			fprintf(stdout,"%u\t%s\t%s\t%s\n",num_article,col_year,col_title,col_author[i]);
		is_article=0;
		//special thank to the DBLP team for having multiple tag on the same line !!!
		end=strchr(line+1,'<');//another (opening) tag on the same line ?!
		if(end)return parseLine(end,len-(end-line));
		return 0;//avoid the rest of the parser (is_article=0)
	}
	if(!is_article && line[0])fprintf(stderr,"skip  : %s @%i\n",line,line_num);
	if(eq(line,"<author>") || eq(line,"<editor>")){
		//fprintf(stderr,"author: %s\n",line);
		end        =strstr(line,"</editor>");
		if(!end)end=strstr(line,"</author>");
		if(!end)return fprintf(stderr,"miss  : </author>\n");
		num_autors++;
		sprintf(col_author[MIN(num_autors,MAX_AUTHOR)-1],"%.*s",(int)(end-(line+8)),line+8);
		end+=9;
	}
	if(eq(line,"<title>" )){
		end=strstr(line,"</title>");
		if(!end)return fprintf(stderr,"miss  : </title>\n");
		sprintf(col_title ,"%.*s",(int)(end-(line+7)),line+7);
		end+=8;
	}
	if(eq(line,"<year>"  )){
		end=strstr(line,"</year>");
		if(!end)return fprintf(stderr,"miss  : </year>\n");
		sprintf(col_year  ,"%.*s",(int)(end-(line+6)),line+6);
		end+=7;
	}
	//subparsing !
	if(end && end[0]){
		//skip to the next tag (because YES sometime there is fuckton of space between tags)
		if((end=strchr(end,'<')))return parseLine(end,len-(end-line));
		//TODO : in recursion, non parsed tag, break the chain
	}
	return 0;
}

void* parseFile(char*path){
	char c,buffer[MAX_LINE_LENGTH];
	f=fopen(path,"r");
	fseek(f, 0, SEEK_END);
	unsigned p,pos,total = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	for(pos=p=0;fread(&c,1,1,f)==1;pos++,p++){
		//if(pos>1000)break;//REMOVE
		if((pos&0xFFFFFF)==0)fprintf(stderr,"%.0f%%\n",((float)pos/(float)total)*100);
		if(c=='\n'||c=='\r'){
			buffer[p]=0;
			parseLine(buffer,p);
			p=-1;
		}else{
			if(p<=MAX_LINE_LENGTH)
				buffer[p]=c;
		}
	}
	fclose(f);
	fprintf(stderr,"parsed %u\n",num_article);
	return NULL;
}

int main(int argc,char**argv){
	if(argc!=2)return fprintf(stderr,"USAGE: %s in.xml\n",argv[0]);
	parseFile(argv[1]);
	return 0;
}
