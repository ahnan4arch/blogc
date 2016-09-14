/*
 * blogc: A blog compiler.
 * Copyright (C) 2015-2016 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#ifndef _NINJA_SETTINGS_H
#define _NINJA_SETTINGS_H

#include "../common/error.h"
#include "../common/utils.h"

typedef struct {
    bc_trie_t *env;
    bc_trie_t *settings;
} blogc_ninja_settings_t;

blogc_ninja_settings_t* blogc_ninja_settings_parse(const char *content,
    size_t content_len, bc_error_t **err);
void blogc_ninja_settings_free(blogc_ninja_settings_t *settings);

#endif /* _NINJA_SETTINGS_H */
