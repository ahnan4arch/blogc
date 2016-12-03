/*
 * blogc: A blog compiler.
 * Copyright (C) 2015-2016 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#ifndef _ERROR_H
#define _ERROR_H

#include <stddef.h>

// error handling is centralized here for the sake of simplicity :/
typedef enum {

    // errors for src/common
    BC_ERROR_CONFIG_PARSER = 1,
    BC_ERROR_FILE,

    // errors for src/blogc
    BLOGC_ERROR_SOURCE_PARSER = 100,
    BLOGC_ERROR_TEMPLATE_PARSER,
    BLOGC_ERROR_LOADER,
    BLOGC_WARNING_DATETIME_PARSER,

    // errors for src/blogc-ninja
    BLOGC_NINJA_ERROR_SETTINGS = 200,
} bc_error_type_t;

typedef struct {
    char *msg;
    bc_error_type_t type;
} bc_error_t;

bc_error_t* bc_error_new(bc_error_type_t type, const char *msg);
bc_error_t* bc_error_new_printf(bc_error_type_t type, const char *format, ...);
bc_error_t* bc_error_parser(bc_error_type_t type, const char *src,
    size_t src_len, size_t current, const char *format, ...);
void bc_error_print(bc_error_t *err, const char *prefix);
void bc_error_free(bc_error_t *err);

#endif /* _ERROR_H */
