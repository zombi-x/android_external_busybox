/* vi: set sw=4 ts=4: */
/*
 * strings implementation for busybox
 *
 * Copyright Tito Ragusa <farmatito@tiscali.it>
 * 
 * Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
 */

#include "busybox.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#define WHOLE_FILE		1
#define PRINT_NAME		2
#define PRINT_OFFSET	4
#define SIZE			8

int strings_main(int argc, char **argv)
{
	int n, c, i = 0, status = EXIT_SUCCESS;
	unsigned long opt;
	unsigned long count;
	FILE *file = stdin;
	char *string;
	const char *fmt = "%s: ";
	char *n_arg = "4";

	opt = bb_getopt_ulflags(argc, argv, "afon:", &n_arg);
	/* -a is our default behaviour */

	argc -= optind;
	argv += optind;

	n = bb_xgetlarg(n_arg, 10, 1, INT_MAX);
	string = xzalloc(n + 1);
	n--;

	if (argc == 0) {
		fmt = "{%s}: ";
		*argv = (char *)bb_msg_standard_input;
		goto PIPE;
	}

	do {
		if ((file = bb_wfopen(*argv, "r"))) {
PIPE:
			count = 0;
			do {
				c = fgetc(file);
				if (isprint(c) || c == '\t') {
					if (i <= n) {
						string[i] = c;
					} else {
						putchar(c);
					}
					if (i == n) {
						if (opt & PRINT_NAME) {
							printf(fmt, *argv);
						}
						if (opt & PRINT_OFFSET) {
							printf("%7lo ", count - n);
						}
						printf("%s", string);
					}
					i++;
				} else {
					if (i > n) {
						putchar('\n');
					}
					i = 0;
				}
				count++;
			} while (c != EOF);
			bb_fclose_nonstdin(file);
		} else {
			status = EXIT_FAILURE;
		}
	} while (--argc > 0);

	if (ENABLE_FEATURE_CLEAN_UP)
		free(string);

	bb_fflush_stdout_and_exit(status);
}
