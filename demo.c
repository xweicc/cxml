/*
	cXml Parse Demo
*/

#include <stdio.h>
#include "cxml.h"

int main(void)
{
	char *str=\
	"<bookstore>\
		<book category=\"CHILDREN\">\
			<title>Harry Potter</title>\
			<author>J K. Rowling</author>\
			<year>2005</year>\
			<price>29.99</price>\
		</book>\
		<book category = \"WEB\">\
			<title>Learning XML</title>\
			<author>Erik T. Ray</author>\
			<year>2003</year>\
			<price>39.95</price>\
		</book>\
	</bookstore>";

	cXml *root,*book,*title;
	char *category;

	root=cXmlParse(str);
	if(!root){
		printf("cXmlParse failed\n");
		return -1;
	}

	printf("===============cXmlPrint Start====================\n");
	cXmlPrint(root);
	printf("===============cXmlPrint End======================\n\n");

	book=cXmlGetItem(root, "book");
	if(!book){
		printf("Not find book\n");
		goto out;
	}
	category=cXmlGetAttr(book, "category");
	if(!category){
		printf("Not find category\n");
		goto out;
	}
	
	title=cXmlGetItem(book, "title");
	if(!title){
		printf("Not find title\n");
		goto out;
	}
	printf("Book category:%s title:%s\n",category,title->val);

	book=cXmlGetNextSameItem(book);
	if(!book){
		printf("Not find Next book\n");
		goto out;
	}
	category=cXmlGetAttr(book, "category");
	if(!category){
		printf("Not find category\n");
		goto out;
	}
	
	title=cXmlGetItem(book, "title");
	if(!title){
		printf("Not find title\n");
		goto out;
	}
	printf("Book category:%s title:%s\n",category,title->val);

out:
	cXmlDelete(root);
	return 0;
}

