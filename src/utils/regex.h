/*
 * regex.h
 *
 *  Created on: 2017年3月29日
 *      Author: caojian02
 */

#ifndef UTILS_REGEX_H_
#define UTILS_REGEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#define REGEX_ALL 0
#define REGEX_NO_ALL 1

int regexAll(const char *regex, char *str, const char **res, int num, int flag);
int match(char *regex, char *str);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_REGEX_H_ */
