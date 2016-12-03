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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common/error.h"
#include "../common/file.h"
#include "../common/utils.h"
#include "renderer.h"
#include "settings.h"


static void
print_help(void)
{
    printf(
        "usage:\n"
        "    blogc-ninja [-h] [-v] [-f FILE] [-o FILE]\n"
        "                - A simple build tool for blogc.\n"
        "\n"
        "optional arguments:\n"
        "    -h            show this help message and exit\n"
        "    -v            show version and exit\n"
        "    -f FILE       settings file (default: settings.ini)\n"
        "    -o FILE       output file (default: build.ninja)\n");
}


static void
print_usage(void)
{
    printf("usage: blogc-ninja [-h] [-v] [-f FILE] [-o FILE]\n");
}


int
main(int argc, char **argv)
{
    int rv = 0;
    bc_error_t *err = NULL;

    char *settings_file = NULL;
    char *output_file = NULL;
    blogc_ninja_settings_t *settings = NULL;

    for (unsigned int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    print_help();
                    goto cleanup;
                case 'v':
                    printf("%s\n", PACKAGE_STRING);
                    goto cleanup;
                case 'f':
                    if (argv[i][2] != '\0')
                        settings_file = bc_strdup(argv[i] + 2);
                    else if (i + 1 < argc)
                        settings_file = bc_strdup(argv[++i]);
                    break;
                case 'o':
                    if (argv[i][2] != '\0')
                        output_file = bc_strdup(argv[i] + 2);
                    else if (i + 1 < argc)
                        output_file = bc_strdup(argv[++i]);
                    break;
                default:
                    print_usage();
                    fprintf(stderr, "blogc-ninja: error: invalid argument: "
                        "-%c\n", argv[i][1]);
                    rv = 2;
                    goto cleanup;
            }
        }
    }

    const char *f = settings_file ? settings_file : "settings.ini";
    const char *o = output_file ? output_file : "build.ninja";

    size_t content_len;
    char *content = bc_file_get_contents(f, true, &content_len, &err);
    if (err != NULL) {
        bc_error_print(err, "blogc-ninja");
        rv = 2;
        goto cleanup;
    }

    settings = blogc_ninja_settings_parse(content, content_len, &err);
    if (err != NULL) {
        fprintf(stderr, "blogc-ninja: failed to parse settings (%s): %s\n",
            f, err->msg);
        rv = 2;
        goto cleanup;
    }

    char *output = blogc_ninja_render(settings);

    printf("%s", output);

cleanup:

    free(settings_file);
    free(output_file);
    blogc_ninja_settings_free(settings);
    bc_error_free(err);

    return rv;
}
