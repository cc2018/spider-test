/*
 * utfconf.h
 *
 *  Created on: 2017年4月1日
 *      Author: caojian02
 */

#ifndef UTILS_UTFCONV_H_
#define UTILS_UTFCONV_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UTF-8 to GBK
 * @param src [in]
 * @param dst [out]
 * @param len [in] The most bytes which starting at dst, will be written.
 *
 */
void utf8_to_gb(char* src, char* dst, int len);

/**
 * GBK to UTF-8
 *
 * @param src [in]
 * @param dst [out]
 * @param len [in] The most bytes which starting at dst, will be written.
 */
void gb_to_utf8(char* src, char* dst, int len);

#ifdef __cplusplus
}
#endif



#endif /* UTILS_UTFCONF_H_ */
