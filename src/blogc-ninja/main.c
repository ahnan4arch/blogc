/*
 * blogc: A blog compiler.
 * Copyright (C) 2015-2016 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>


static void
print_help(void)
{
    printf(
        "usage:\n"
        "    blogc-ninja [-h] [-v]\n"
        "                - A simple build tool for blogc.\n"
        "\n"
        "optional arguments:\n"
        "    -h            show this help message and exit\n"
        "    -v            show version and exit\n");
}


static void
print_usage(void)
{
    printf("usage: blogc-ninja [-h] [-v]\n");
}


int
main(int argc, char **argv)
{
    int rv = 0;

    for (unsigned int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    print_help();
                    goto cleanup;
                case 'v':
                    printf("%s\n", PACKAGE_STRING);
                    goto cleanup;
                default:
                    print_usage();
                    fprintf(stderr, "blogc-ninja: error: invalid argument: "
                        "-%c\n", argv[i][1]);
                    rv = 2;
                    goto cleanup;
            }
        }
    }

    printf("Hello World!\n");

cleanup:

    return rv;
}
