/*
 * blogc: A blog compiler.
 * Copyright (C) 2015 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file COPYING.
 */

#ifndef _SOURCE_PARSER_H
#define _SOURCE_PARSER_H

#include <stdlib.h>
#include "utils/utils.h"
#include "error.h"

typedef struct {
    b_trie_t *config;
    char *content;
} blogc_source_t;

blogc_source_t* blogc_source_parse(const char *src, size_t src_len,
    blogc_error_t **err);
void blogc_source_free(void *source);

#endif /* _SOURCE_PARSER_H */
