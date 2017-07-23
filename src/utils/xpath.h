/*
 * xpath.h
 *
 *  Created on: 2017年3月29日
 *      Author: caojian02
 */

#ifndef UTILS_XPATH_H_
#define UTILS_XPATH_H_

#include<libxml/tree.h>
#include<libxml/parser.h>
#include<libxml/xpath.h>
#include<libxml/xpathInternals.h>
#include<libxml/HTMLparser.h>
#include<libxml/HTMLtree.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REGEX_ALL 0
#define REGEX_NO_ALL 1

int regexAll(const char *regex, char *str, char **res, int num, int flag);
int match(char *regex, char *str);
//xpath.c
int xpath(char *xml, const char *path, char **get, int num);
int  execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, char** get, int num);
int print_xpath_nodes(xmlNodeSetPtr nodes, char **get, int num);

void xpath_free_str(char **get, int size);


#ifdef __cplusplus
}
#endif

#endif /* UTILS_XPATH_H_ */
