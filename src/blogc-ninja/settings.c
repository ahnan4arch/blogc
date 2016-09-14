/*
 * blogc: A blog compiler.
 * Copyright (C) 2015-2016 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include "../common/config-parser.h"
#include "../common/error.h"
#include "../common/utils.h"
#include "settings.h"


blogc_ninja_settings_t*
blogc_ninja_settings_parse(const char *content, size_t content_len,
    bc_error_t **err)
{
    if (err != NULL && *err != NULL)
        return NULL;

    if (content == NULL)
        return NULL;

    bc_config_t *config = bc_config_parse(content, content_len, err);
    if (config == NULL || (err != NULL && *err != NULL))
        return NULL;

    blogc_ninja_settings_t *rv = bc_malloc(sizeof(blogc_ninja_settings_t));
    rv->env = bc_trie_new(free);
    rv->settings = bc_trie_new(free);

    char **env = bc_config_list_keys(config, "environment");
    if (env != NULL) {
        for (size_t i = 0; env[i] != NULL; i++) {
            // FIXME: validate keys
            bc_trie_insert(rv->env, env[i],
                bc_strdup(bc_config_get(config, "environment", env[i])));
        }
    }
    bc_strv_free(env);

    bc_trie_insert(rv->settings, "output_dir", bc_strdup(
        bc_config_get_with_default(
            config, "settings", "output_dir", "_build")));

    bc_trie_insert(rv->settings, "content_dir", bc_strdup(
        bc_config_get_with_default(
            config, "settings", "content_dir", "content")));

    bc_trie_insert(rv->settings, "assets_dir", bc_strdup(
        bc_config_get_with_default(
            config, "settings", "assets_dir", "assets")));

    bc_trie_insert(rv->settings, "templates_dir", bc_strdup(
        bc_config_get_with_default(
            config, "settings", "templates_dir", "templates")));

    bc_trie_insert(rv->settings, "main_template", bc_strdup(
        bc_config_get_with_default(
            config, "settings", "main_template", "main.tmpl")));

    bc_trie_insert(rv->settings, "atom_template", bc_strdup(
        bc_config_get_with_default(
            config, "settings", "atom_template", "atom.tmpl")));

    bc_config_free(config);

    return rv;
}


void
blogc_ninja_settings_free(blogc_ninja_settings_t *settings)
{
    if (settings == NULL)
        return;
    bc_trie_free(settings->env);
    bc_trie_free(settings->settings);
    free(settings);
}
