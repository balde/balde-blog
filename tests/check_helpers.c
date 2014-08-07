/*
 * balde-blog: A really simple blog application built with balde.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <glib.h>
#include <balde.h>
#include <balde-blog/post-loader-private.h>
#include <balde-blog/post-parser-private.h>
#include <balde-blog/helpers-private.h>


void
test_get_post_by_slug(void)
{
    balde_app_t *app = balde_app_init();
    balde_app_set_config(app, "POSTS_DIRECTORY", "foo");
    bb_post_loader_load_app(app);
    bb_post_t* t = bb_get_post_by_slug(app, "baz");
    g_assert(t != NULL);
    g_assert_cmpstr(t->metadata->title, ==, "foo asd baz");
    bb_post_loader_free_app(app);
    balde_app_free(app);
}


void
test_get_posts_by_page(void)
{
    balde_app_t *app = balde_app_init();
    balde_app_set_config(app, "POSTS_DIRECTORY", "foo");
    bb_post_loader_load_app(app);
    bb_item_t *t;

    GList *l = bb_get_posts_by_page(app, 1, 2);
    g_assert(l != NULL);

    t = l->data;
    g_assert(t != NULL);
    g_assert_cmpstr(t->slug, ==, "lol");

    t = l->next->data;
    g_assert(t != NULL);
    g_assert_cmpstr(t->slug, ==, "bar");

    g_assert(l->next->next == NULL);

    g_list_free(l);

    l = bb_get_posts_by_page(app, 2, 2);
    g_assert(l != NULL);

    t = l->data;
    g_assert(t != NULL);
    g_assert_cmpstr(t->slug, ==, "baz");

    g_assert(l->next == NULL);

    g_list_free(l);

    l = bb_get_posts_by_page(app, 3, 2);
    g_assert(l == NULL);

    g_list_free(l);

    bb_post_loader_free_app(app);
    balde_app_free(app);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/helpers/get_post_by_slug", test_get_post_by_slug);
    g_test_add_func("/helpers/get_posts_by_page", test_get_posts_by_page);
    return g_test_run();
}
