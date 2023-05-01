#ifndef __CXML_H__
#define __CXML_H__


typedef struct cXmlAttr{
	struct cXmlAttr *next;	//下一个属性
	char *name;				//属性名
	char *val;				//属性值
} cXmlAttr;

typedef struct cXml{
	struct cXml *next;	//下一个节点
	struct cXml *child;	//子节点
	char *name;			//xml元素名
	char *val;			//xml元素值,可能为NULL
	cXmlAttr *attr;		//xml元素属性
} cXml;					//一个xml元素

cXml *cXmlParse(const char *str);
void cXmlDelete(cXml *node);
void cXmlPrint(cXml *node, int depth);
cXml *cXmlGetItem(cXml *node, char *name);
cXml *cXmlGetNextSameItem(cXml *root);
char *cXmlGetAttr(cXml *root, char *name);



#endif
