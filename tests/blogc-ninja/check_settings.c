/*
 * blogc: A blog compiler.
 * Copyright (C) 2015-2016 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>
#include <string.h>

#include "../../src/blogc-ninja/settings.h"
#include "../../src/common/error.h"
#include "../../src/common/utils.h"


static void
test_settings_empty(void **state)
{
    const char *a = "";
    bc_error_t *err = NULL;
    blogc_ninja_settings_t *s = blogc_ninja_settings_parse(a, strlen(a), &err);
    assert_non_null(err);
    assert_null(s);
    assert_int_equal(err->type, BLOGC_NINJA_ERROR_SETTINGS);
    assert_string_equal(err->msg,
        "[environment] key required but not found or empty: AUTHOR_NAME");
    bc_error_free(err);
}


static void
test_settings(void **state)
{
    const char *a =
        "[settings]\n"
        "output_dir = bola\n"
        "content_dir = guda\n"
        "assets_dir = chunda\n"
        "main_template = foo.tmpl\n"
        "\n"
        "[environment]\n"
        "BOLA = asd\n"
        "GUDA = qwe\n";
    bc_error_t *err = NULL;
    blogc_ninja_settings_t *s = blogc_ninja_settings_parse(a, strlen(a), &err);
    assert_non_null(err);
    assert_null(s);
    assert_int_equal(err->type, BLOGC_NINJA_ERROR_SETTINGS);
    assert_string_equal(err->msg,
        "[environment] key required but not found or empty: AUTHOR_NAME");
    bc_error_free(err);
}


static void
test_settings2(void **state)
{
    const char *a =
        "[settings]\n"
        "output_dir = bola\n"
        "content_dir = guda\n"
        "assets_dir = chunda\n"
        "main_template = foo.tmpl\n"
        "\n"
        "[environment]\n"
        "BOLA = asd\n"
        "GUDA = qwe\n"
        "AUTHOR_NAME = chunda\n"
        "AUTHOR_EMAIL = chunda@example.com\n"
        "SITE_TITLE = Fuuuuuuuuu\n"
        "SITE_TAGLINE = My cool tagline\n";
    bc_error_t *err = NULL;
    blogc_ninja_settings_t *s = blogc_ninja_settings_parse(a, strlen(a), &err);
    assert_null(err);
    assert_non_null(s);
    assert_int_equal(bc_trie_size(s->env), 6);
    assert_string_equal(bc_trie_lookup(s->env, "BOLA"), "asd");
    assert_string_equal(bc_trie_lookup(s->env, "GUDA"), "qwe");
    assert_string_equal(bc_trie_lookup(s->env, "AUTHOR_NAME"), "chunda");
    assert_string_equal(bc_trie_lookup(s->env, "AUTHOR_EMAIL"), "chunda@example.com");
    assert_string_equal(bc_trie_lookup(s->env, "SITE_TITLE"), "Fuuuuuuuuu");
    assert_string_equal(bc_trie_lookup(s->env, "SITE_TAGLINE"), "My cool tagline");
    assert_int_equal(bc_trie_size(s->settings), 5);
    assert_string_equal(bc_trie_lookup(s->settings, "output_dir"), "bola");
    assert_string_equal(bc_trie_lookup(s->settings, "content_dir"), "guda");
    assert_string_equal(bc_trie_lookup(s->settings, "assets_dir"), "chunda");
    assert_string_equal(bc_trie_lookup(s->settings, "template_dir"), "templates");
    assert_string_equal(bc_trie_lookup(s->settings, "main_template"), "foo.tmpl");
    blogc_ninja_settings_free(s);
}


int
main(void)
{
    const UnitTest tests[] = {
        unit_test(test_settings_empty),
        unit_test(test_settings),
        unit_test(test_settings2),
    };
    return run_tests(tests);
}
