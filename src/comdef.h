/*
 * comdef.h
 *
 *  Created on: 2017年3月23日
 *      Author: caojian02
 */

#ifndef COMDEF_H_
#define COMDEF_H_

#define _POSIX_C_SOURCE 200809L

#if defined(SOLARIS)		/* Solaris 10 */
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 700
#endif

#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>	/* for winsize */
#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */
#include <assert.h>

/* for c++ std */
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

/* for dmalloc memory check */
// #ifdef DMALLOC
// #include "dmalloc.h"
// #endif
// dmalloc -l logfile -i 100 low

#define PANIC(x) assert(x != NULL)
/*
the max number of the buffers
*/
#define BUFFER_MAX_NUMBER 1024
#define	MAXLINE	4096			/* max line length */

/*
 * Default file access permissions for new files.
 */
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * Default permissions for new directories.
 */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef	void	Sigfunc(int);	/* for signal handlers */

#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))



#endif /* COMDEF_H_ */
