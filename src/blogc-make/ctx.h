/*
 * blogc: A blog compiler.
 * Copyright (C) 2016 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#ifndef _MAKE_CTX_H
#define _MAKE_CTX_H

#include <stdbool.h>
#include <time.h>
#include "settings.h"
#include "../common/error.h"
#include "../common/utils.h"

typedef struct {
    char *path;
    char *short_path;
    struct timespec timestamp;
    bool readable;
} bm_filectx_t;

typedef struct {
    bm_settings_t *settings;

    char *root_dir;
    char *output_dir;

    bm_filectx_t *main_template_fctx;
    bm_filectx_t *atom_template_fctx;
    bm_filectx_t *settings_fctx;

    bc_slist_t *posts_fctx;
    bc_slist_t *pages_fctx;
    bc_slist_t *copy_files_fctx;
} bm_ctx_t;

bm_filectx_t* bm_filectx_new(bm_ctx_t *ctx, const char *filename);
void bm_filectx_free(bm_filectx_t *fctx);
bm_ctx_t* bm_ctx_new(const char *filename, bc_error_t **err);
void bm_ctx_free(bm_ctx_t *ctx);

#endif /* _MAKE_CTX_H */
