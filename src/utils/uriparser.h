/*
 * uriparser.h
 *
 *  Created on: 2017年3月29日
 *      Author: caojian02
 */

#ifndef UTILS_URIPARSER_H_
#define UTILS_URIPARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  URI_SCHEME = 0,
  URI_AUTH,
  URI_HOST,
  URI_PORT,
  URI_PATH,
  URI_QUERY,
  URI_FRAGMENT
} URIACTION;

void
cspider_joinall(const char *, char **, int);

char *
cspider_join(const char *, char*);

char *
cspider_uri_component(char *, URIACTION);

void
cspider_rstrip(char *);

void
cspider_lstrip(char *);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_URIPARSER_H_ */
