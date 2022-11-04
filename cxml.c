/*
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License 
 *	as published by the Free Software Foundation.
 *
 *	Auther: QiaoWei <qwiaoei@163.com>
 */

/*XML Parser*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cxml.h"

#define cXmlFree(p) do{if(p) free(p);}while(0)

static const char *skipCommentary(const char *str)
{
	if(!strncmp(str, "<!--", 4)){
		str=strstr(str,"-->");str+=3;
	}
	return str;
}

static const char *skipSpacing(const char *str)
{
	while(str && *str && (unsigned char)*str<=32) str++;
	str=skipCommentary(str);
	while (str && *str && (unsigned char)*str<=32) str++;
	return str;
}

static cXml *cXmlNew()
{
	cXml *node=(cXml *)malloc(sizeof(cXml));
  	if(node) memset(node, 0, sizeof(cXml));
	return node;
}

static char *cXmlNewStr(int len)
{
	char *str=malloc(len+1);
	if(str) memset(str, 0, len+1);
	return str;
}


static cXmlAttr *cXmlAttrNew()
{
	cXmlAttr *attr=(cXmlAttr *)malloc(sizeof(cXmlAttr));
  	if(attr) memset(attr, 0, sizeof(cXmlAttr));
	return attr;
}

static const char *cXmlParseAttr(cXmlAttr *attr, const char *str)
{
	const char *start=skipSpacing(str);
	const char *end=NULL;
	int len=0;
	char c;

	if(*start=='>' || *start=='/') return start;
	str=start;
	while(*str && *str!='=' && *str!='>' && *str!=' ') str++;
	end=str;
	len=end-start;
	attr->name=cXmlNewStr(len);
	if(!attr->name) return NULL;
	strncpy(attr->name, start, len);
	str=skipSpacing(end);
	if(*str!='=') return NULL;
	str++;
	start=skipSpacing(str);

	if(*start!='\"' && *start!='\'') return NULL;
	c=*start;
	start++;
	str=start;
	while(*str && *str!=c && *str!='>') str++;
	if(*str!=c) return NULL;
	end=str;
	len=end-start;
	attr->val=cXmlNewStr(len);
	if(!attr->val) return NULL;
	strncpy(attr->val, start, len);
	return ++end;
}


static const char *cXmlParseNode(cXml *item, const char *str);
static const char *cXmlNodeEnd(cXml *item, const char *str)
{
	str++;
	str=skipSpacing(str);
	if(*str!='<') return NULL;
	
	if(*(str+1)=='/'){
		return str;
	}else{
		while(*str=='<' && *(str+1)!='/'){
			cXml *node=cXmlNew();
			if(!node) return NULL;
			item->next=node;
			str=cXmlParseNode(node, str);
			if(!str) return NULL;
			if(!*str) str=skipSpacing(str);
		}
		return str;
	}
}

static const char *cXmlParseNode(cXml *item, const char *str)
{
	int len=0;
	const char *start=NULL;
	const char *end=NULL;

	if(*str!='<') return NULL;

	start=++str;
	while(str && *str && *str!='>' && *str!=' ')str++;
	len=str-start;
	item->name=cXmlNewStr(len);
	if(!item->name) return NULL;
	strncpy(item->name, start, len);

	if(*str==' '){
		cXmlAttr *attr=NULL;
		attr=(cXmlAttr *)cXmlAttrNew();
		if(!attr) return NULL;
		item->attr=attr;
		str=cXmlParseAttr(attr,str);
		if(!str || !*str) return NULL;
		while(*str==' '){
 			attr->next=(cXmlAttr *)cXmlAttrNew();
			if(!attr->next) return NULL;
			str=cXmlParseAttr(attr->next,str);
			if(!str || !*str) return NULL;
			attr=attr->next;
		}
	}
	if(*str=='/' && *(str+1)=='>'){
		str++;
		return cXmlNodeEnd(item, str);
	}
	if(*str!='>') return NULL;
	
	str++;
	str=skipSpacing(str);
	
	if(*str=='<'){
		cXml *node=cXmlNew();
		if(!node) return NULL;
		item->child=node;
		str=cXmlParseNode(node, str);
		if(!str) return NULL;
		while(*str=='<' && *(str+1)!='/'){
			cXml *nnode=cXmlNew();
			if(!nnode) return NULL;
			node->next=nnode;
			node=nnode;
			str=cXmlParseNode(nnode, str);
			if(!str) return NULL;
		}
		if(*(str+1)!='/') return NULL;
		str+=2;
		if(strncmp(str, item->name, strlen(item->name))) return NULL;
		str+=strlen(item->name);
		str++;
		return skipSpacing(str);
	}else{
		start=str;
		end=strstr(str,"</");
		if(!end) return NULL;
		len=end-start;
		item->val=cXmlNewStr(len);
		if(!item->val)return NULL;
		strncpy(item->val, start, len);
		str=end+2;
		if(strncmp(str, item->name, strlen(item->name))) return NULL;
		str+=strlen(item->name);
		str=skipSpacing(str);
		if(*str!='>') return NULL;

		return cXmlNodeEnd(item, str);
	}
	return NULL;
}

static void cXmlDelAttr(cXmlAttr *attr)
{
	cXmlAttr *next=NULL;
	
	if(!attr) return ;
	next=attr->next;
	cXmlFree(attr->name);
	cXmlFree(attr->val);
	cXmlFree(attr);
	cXmlDelAttr(next);
}

/*释放XML解析使用的内存*/
void cXmlDelete(cXml *node)
{
	cXml *next=NULL;
	cXml *child=NULL;

	if(!node) return ;
	next=node->next;
	child=node->child;
	cXmlFree(node->name);
	cXmlFree(node->val);
	cXmlDelAttr(node->attr);
	cXmlFree(node);
	cXmlDelete(next);
	cXmlDelete(child);
}

/*解析XML文本,失败返回NULL*/
cXml *cXmlParse(const char *str)
{
	cXml *root=cXmlNew();

	if(!root || !str){
		return NULL;
	}
	str=skipSpacing(str);
	if(*str=='<' && *(str+1)=='?'){
		str+=2;
		while(*str && *str!='?' && *(str+1)!='>') str++;
		str+=2;
	}
	if(!cXmlParseNode(root,skipSpacing(str))){
		cXmlDelete(root);
		return NULL;
	}
	return root;
}

static void cXmlAttrPrint(cXmlAttr *node)
{
	if(!node) return ;
	if(node->name && node->val) printf("Attr: %s=%s\n",node->name,node->val);
	if(node->next) cXmlAttrPrint(node->next);
}

void cXmlPrint(cXml *node)
{
	if(!node) return ;
	if(node->name) printf("Name=%s\n",node->name);
	if(node->attr) cXmlAttrPrint(node->attr);
	if(node->val) printf("Value=%s\n",node->val);
	if(node->child){printf("Child:\n"); cXmlPrint(node->child);}
	if(node->next){printf("Next:\n"); cXmlPrint(node->next);}
}

/*取子节点*/
cXml *cXmlGetItem(cXml *root, char *name)
{
	cXml *node=root->child;
	while(node && strcmp(node->name, name)) node=node->next;
	return node;
}

/*取下一个节点,且name相同*/
cXml *cXmlGetNextSameItem(cXml *root)
{
	cXml *node=root->next;
	if(node && node->name && !strcmp(node->name, root->name)){
		return node;
	}
	return NULL;
}

/*取节点的属性值*/
char *cXmlGetAttr(cXml *root, char *name)
{
	cXmlAttr *attr=root->attr;
	while(attr && strcmp(attr->name, name)) attr=attr->next;
	if(attr) return attr->val;
	return NULL;
}

