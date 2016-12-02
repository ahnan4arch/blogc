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


static const struct default_settings_map {
    const char *key;
    const char *default_value;
} default_settings[] = {
    {"output_dir", "_build"},
    {"content_dir", "content"},
    {"assets_dir", "assets"},
    {"template_dir", "templates"},
    {"main_template", "main.tmpl"},
    {"locale", NULL},
    {NULL, NULL},
};


static const char* required_environment[] = {
    "AUTHOR_NAME",
    "AUTHOR_EMAIL",
    "SITE_TITLE",
    "SITE_TAGLINE",
    NULL,
};


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

    for (size_t i = 0; required_environment[i] != NULL; i++) {
        const char *value = bc_trie_lookup(rv->env, required_environment[i]);
        if (value == NULL || value[0] == '\0') {
            *err = bc_error_new_printf(BLOGC_NINJA_ERROR_SETTINGS,
                "[environment] key required but not found or empty: %s",
                required_environment[i]);
            blogc_ninja_settings_free(rv);
            rv = NULL;
            goto cleanup;
        }
    }

    for (size_t i = 0; default_settings[i].key != NULL; i++) {
        const char *value = bc_config_get_with_default(
            config, "settings", default_settings[i].key,
            default_settings[i].default_value);
        if (value != NULL) {
            bc_trie_insert(rv->settings, default_settings[i].key,
                bc_strdup(value));
        }
    }

cleanup:

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
