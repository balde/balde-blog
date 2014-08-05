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


void
test_parse_slug(void)
{
    gchar *slug = bb_post_loader_parse_slug("/bola/guda/asdqwe-tyu.md");
    g_assert_cmpstr(slug, ==, "asdqwe-tyu");
    g_free(slug);
    slug = bb_post_loader_parse_slug("/bola/guda/asdqwe-tyu.markdown");
    g_assert_cmpstr(slug, ==, "asdqwe-tyu");
    g_free(slug);
    g_assert(bb_post_loader_parse_slug(NULL) == NULL);
    g_assert(bb_post_loader_parse_slug("/bola/guda/asdqwe-tyu.txt") == NULL);
}


void
test_load_from_file(void)
{
    bb_item_t *t = bb_post_loader_load_from_file("foo/bar.md");
    g_assert(t != NULL);
    g_assert_cmpstr(t->slug, ==, "bar");
    g_assert(t->post != NULL);
    bb_free_item(t);
    t = bb_post_loader_load_from_file("foo/bola.md");
    g_assert(t->error != NULL);
    g_assert(t->post == NULL);
    g_assert_cmpstr(t->slug, ==, "bola");
    bb_free_item(t);
}


void
test_load_from_directory(void)
{
    GList *l = bb_post_loader_load_from_directory("foo");
    g_assert(l != NULL);
    bb_item_t *t;
    GDateTime *dt;

    t = l->data;
    g_assert(t != NULL);
    g_assert(t->error == NULL);
    g_assert_cmpstr(t->slug, ==, "lol");
    g_assert(t->post != NULL);
    g_assert_cmpstr(t->post->raw_content, ==,
        "<!-- title: foo asd -->\n"
        "<!-- author: arcoiro -->\n"
        "<!-- datetime: 2015-08-10 10:00:00 -->\n"
        "<!-- tags: bola, guda -->\n"
        "\n"
        "This is yet another test post!\n");
    g_assert(t->post->metadata != NULL);
    g_assert_cmpstr(t->post->metadata->title, ==, "foo asd");
    g_assert_cmpstr(t->post->metadata->author, ==, "arcoiro");
    dt = g_date_time_new_utc(2015, 8, 10, 10, 0, 0);
    g_assert_cmpint(g_date_time_compare(t->post->metadata->datetime, dt), ==, 0);
    g_date_time_unref(dt);
    g_assert(t->post->metadata->mdatetime == NULL);
    g_assert_cmpstr(t->post->metadata->tags[0], ==, "bola");
    g_assert_cmpstr(t->post->metadata->tags[1], ==, "guda");
    g_assert(t->post->metadata->tags[2] == NULL);

    t = l->next->data;
    g_assert(t != NULL);
    g_assert(t->error == NULL);
    g_assert_cmpstr(t->slug, ==, "bar");
    g_assert(t->post != NULL);
    g_assert_cmpstr(t->post->raw_content, ==,
        "<!-- title: foo asd -->\n"
        "<!-- author: arcoiro -->\n"
        "<!-- datetime: 2014-08-10 10:00:00 -->\n"
        "<!-- tags: bola, guda -->\n"
        "\n"
        "This is just a test post!\n");
    g_assert(t->post->metadata != NULL);
    g_assert_cmpstr(t->post->metadata->title, ==, "foo asd");
    g_assert_cmpstr(t->post->metadata->author, ==, "arcoiro");
    dt = g_date_time_new_utc(2014, 8, 10, 10, 0, 0);
    g_assert_cmpint(g_date_time_compare(t->post->metadata->datetime, dt), ==, 0);
    g_date_time_unref(dt);
    g_assert(t->post->metadata->mdatetime == NULL);
    g_assert_cmpstr(t->post->metadata->tags[0], ==, "bola");
    g_assert_cmpstr(t->post->metadata->tags[1], ==, "guda");
    g_assert(t->post->metadata->tags[2] == NULL);

    t = l->next->next->data;
    g_assert(t != NULL);
    g_assert(t->error == NULL);
    g_assert_cmpstr(t->slug, ==, "baz");
    g_assert(t->post != NULL);
    g_assert_cmpstr(t->post->raw_content, ==,
        "<!-- title: foo asd -->\n"
        "<!-- author: arcoiro -->\n"
        "<!-- datetime: 2014-08-09 10:00:00 -->\n"
        "<!-- tags: bola, guda -->\n"
        "\n"
        "This is just another test post!\n");
    g_assert(t->post->metadata != NULL);
    g_assert_cmpstr(t->post->metadata->title, ==, "foo asd");
    g_assert_cmpstr(t->post->metadata->author, ==, "arcoiro");
    dt = g_date_time_new_utc(2014, 8, 9, 10, 0, 0);
    g_assert_cmpint(g_date_time_compare(t->post->metadata->datetime, dt), ==, 0);
    g_date_time_unref(dt);
    g_assert(t->post->metadata->mdatetime == NULL);
    g_assert_cmpstr(t->post->metadata->tags[0], ==, "bola");
    g_assert_cmpstr(t->post->metadata->tags[1], ==, "guda");
    g_assert(t->post->metadata->tags[2] == NULL);

    g_assert(l->next->next->next == NULL);
    bb_free_items(l);
    g_assert(bb_post_loader_load_from_directory("kofola") == NULL);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/post_loader/parse_slug", test_parse_slug);
    g_test_add_func("/post_loader/load_from_file", test_load_from_file);
    g_test_add_func("/post_loader/load_from_directory", test_load_from_directory);
    return g_test_run();
}
