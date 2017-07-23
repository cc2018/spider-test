//============================================================================
// Name        : mongo-test.cpp
// Author      : caojian
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "comdef.h"
#include "db-client.h"
#include "spider-mgr.h"

#ifndef lint
static const char rcsid[] =
		"$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include <fcgi_config.h>

#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include <fcgi_stdio.h>

static void PrintEnv(char *label, char **envp) {
	printf("%s:<br>\n<pre>\n", label);
	for (; *envp != NULL; envp++) {
		printf("%s\n", *envp);
	}
	printf("</pre><p>\n");
}

void cgi_test() {
	char **initialEnv = environ;
	int count = 0;

	while (FCGI_Accept() >= 0) {
		char *contentLength = getenv("CONTENT_LENGTH");
		int len;

		//printf("Content-type: text/html\r\n\r\n"
						// "{\"name\": \"caojian\", \"gender\": \"man\"}");
		printf("Content-type: text/html\r\n"
				"\r\n"
				"<title>FastCGI echo</title>"
				"<h1>FastCGI echo</h1>\n"
				"Request number %d,  Process ID: %d<p>\n", ++count, getpid());

		if (contentLength != NULL) {
			len = strtol(contentLength, NULL, 10);
		} else {
			len = 0;
		}

		if (len <= 0) {
			printf("No data from standard input.<p>\n");
		} else {
			int i, ch;

			printf("Standard input:<br>\n<pre>\n");
			for (i = 0; i < len; i++) {
				if ((ch = getchar()) < 0) {
					printf(
							"Error: Not enough bytes received on standard input<p>\n");
					break;
				}
				putchar(ch);
			}
			printf("\n</pre><p>\n");
		}

		PrintEnv("Request environment", environ);
		PrintEnv("Initial environment", initialEnv);
	} /* while */
}

int main(int argc, char *argv[]) {
	printf("main begin \n");
	int test = DBMgr()->GetArticleCount("");
	printf("main begin %d\n", test);

	SpiderMgr* spider = new SpiderMgr();
	spider->Init();
	spider->Start();

	// delete spider;
	// delete client;
	// cgi_test();

	printf("exit, bye!");

	return 0;
}
