/*
	cXml Parse Demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cxml.h"

char *Fread(char *path)
{
    FILE *fp=NULL;
    char *buf=NULL;
    size_t size;
    
    fp=fopen(path, "r");
    if(!fp){
        printf("fopen %s error:%s\n",path,strerror(errno));
        return NULL;
    }

    if(fseek(fp,0,SEEK_END)){
        printf("fseek %s error:%s\n",path,strerror(errno));
        goto out;
    }

    size=ftell(fp);
    if(size<=0){
        printf("ftell %s error:%s\n",path,strerror(errno));
        goto out;
    }

    if(fseek(fp,0,SEEK_SET)){
        printf("fseek %s error:%s\n",path,strerror(errno));
        goto out;
    }

    buf=malloc(size);
    if(!buf){
        printf("malloc %lu error:%s\n",size,strerror(errno));
        goto out;
    }

    if(1!=fread(buf,size,1,fp)){
        printf("fread %s error:%s\n",path,strerror(errno));
        goto out;
    }

    fclose(fp);
    return buf;
out:    
    if(fp){
        fclose(fp);
    }
    if(buf){
        free(buf);
    }
    return NULL;
}

int main(int argc, char **argv)
{
	cXml *root=NULL;
    char *buf=NULL;
    long size;

    if(argc<2){
        printf("Usage:\n");
        printf("\t%s <xmlFile>\n",argv[0]);
        return -1;
    }

    buf=Fread(argv[1]);
    if(!buf){
        goto out;
    }

	root=cXmlParse(buf);
	if(!root){
		printf("cXmlParse failed\n");
		return -1;
	}

    cXmlPrint(root, 0);

out:
    if(root){
        cXmlDelete(root);
    }
    if(buf){
        free(buf);
    }
	return 0;
}

