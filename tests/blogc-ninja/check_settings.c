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
    assert_null(err);
    assert_non_null(s);
    assert_int_equal(bc_trie_size(s->env), 0);
    assert_int_equal(bc_trie_size(s->settings), 6);
    assert_string_equal(bc_trie_lookup(s->settings, "output_dir"), "_build");
    assert_string_equal(bc_trie_lookup(s->settings, "content_dir"), "content");
    assert_string_equal(bc_trie_lookup(s->settings, "assets_dir"), "assets");
    assert_string_equal(bc_trie_lookup(s->settings, "templates_dir"), "templates");
    assert_string_equal(bc_trie_lookup(s->settings, "main_template"), "main.tmpl");
    assert_string_equal(bc_trie_lookup(s->settings, "atom_template"), "atom.tmpl");
    blogc_ninja_settings_free(s);
}


static void
test_settings(void **state)
{
    const char *a =
        "[settings]\n"
        "output_dir = bola\n"
        "content_dir = guda\n"
        "assets_dir = chunda\n"
        "templates_dir = lol\n"
        "main_template = foo.tmpl\n"
        "atom_template = bar.tmpl\n"
        "\n"
        "[environment]\n"
        "BOLA = asd\n"
        "GUDA = qwe\n";
    bc_error_t *err = NULL;
    blogc_ninja_settings_t *s = blogc_ninja_settings_parse(a, strlen(a), &err);
    assert_null(err);
    assert_non_null(s);
    assert_int_equal(bc_trie_size(s->env), 2);
    assert_string_equal(bc_trie_lookup(s->env, "BOLA"), "asd");
    assert_string_equal(bc_trie_lookup(s->env, "GUDA"), "qwe");
    assert_int_equal(bc_trie_size(s->settings), 6);
    assert_string_equal(bc_trie_lookup(s->settings, "output_dir"), "bola");
    assert_string_equal(bc_trie_lookup(s->settings, "content_dir"), "guda");
    assert_string_equal(bc_trie_lookup(s->settings, "assets_dir"), "chunda");
    assert_string_equal(bc_trie_lookup(s->settings, "templates_dir"), "lol");
    assert_string_equal(bc_trie_lookup(s->settings, "main_template"), "foo.tmpl");
    assert_string_equal(bc_trie_lookup(s->settings, "atom_template"), "bar.tmpl");
    blogc_ninja_settings_free(s);
}


int
main(void)
{
    const UnitTest tests[] = {
        unit_test(test_settings_empty),
        unit_test(test_settings),
    };
    return run_tests(tests);
}
