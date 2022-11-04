#ifndef __CXML_H__
#define __CXML_H__


typedef struct cXmlAttr{
	struct cXmlAttr *next;	//��һ������
	char *name;				//������
	char *val;				//����ֵ
} cXmlAttr;

typedef struct cXml{
	struct cXml *next;	//��һ���ڵ�
	struct cXml *child;	//�ӽڵ�
	char *name;			//xmlԪ����
	char *val;			//xmlԪ��ֵ
	cXmlAttr *attr;		//xmlԪ������
} cXml;					//һ��xmlԪ��

cXml *cXmlParse(const char *str);
void cXmlDelete(cXml *node);
void cXmlPrint(cXml *node);
cXml *cXmlGetItem(cXml *node, char *name);
cXml *cXmlGetNextSameItem(cXml *root);
char *cXmlGetAttr(cXml *root, char *name);



#endif
