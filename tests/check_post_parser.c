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
#include <balde-blog/post-parser-private.h>


void
test_parse_metadata(void)
{
    GHashTable *metadata = bb_post_parser_parse_metadata(
        "some random content here\n"
        "<!-- date: 2014-10-20 20:40:50 -->\n"
        "\n"
        "<!-- mdate: 2034-11-16 12:34:45 -->\n"
        "<!-- mdate: 2034-12-16 12:34:45 -->\n"
        "asdasd\n"
        "<!-- author: joao do caminhão     -->\n"
        "more random content here\n"
        "<!--tags:bola,guda-->");
    g_assert(metadata != NULL);
    g_assert_cmpint(g_hash_table_size(metadata), ==, 4);
    g_assert_cmpstr(g_hash_table_lookup(metadata, "date"), ==, "2014-10-20 20:40:50");
    g_assert_cmpstr(g_hash_table_lookup(metadata, "mdate"), ==, "2034-12-16 12:34:45");
    g_assert_cmpstr(g_hash_table_lookup(metadata, "author"), ==, "joao do caminhão");
    g_assert_cmpstr(g_hash_table_lookup(metadata, "tags"), ==, "bola,guda");
    g_hash_table_destroy(metadata);
}


void
test_parse_datetime(void)
{
    GDateTime *dt = bb_post_parser_parse_datetime("2014-10-20 20:40:50");
    g_assert(dt != NULL);
    g_assert_cmpint(g_date_time_get_year(dt), ==, 2014);
    g_assert_cmpint(g_date_time_get_month(dt), ==, 10);
    g_assert_cmpint(g_date_time_get_day_of_month(dt), ==, 20);
    g_assert_cmpint(g_date_time_get_hour(dt), ==, 20);
    g_assert_cmpint(g_date_time_get_minute(dt), ==, 40);
    g_assert_cmpint(g_date_time_get_seconds(dt), ==, 50);
    g_date_time_unref(dt);
    dt = bb_post_parser_parse_datetime("2014-10-20 20:40");
    g_assert(dt == NULL);
    dt = bb_post_parser_parse_datetime("2014-10-20 54:40:50");
    g_assert(dt == NULL);
    dt = bb_post_parser_parse_datetime(NULL);
    g_assert(dt == NULL);
}


void
test_parse_tags(void)
{
    gchar **tags = bb_post_parser_parse_tags("bola, guda , arcoiro,asd");
    g_assert(tags != NULL);
    g_assert_cmpstr(tags[0], ==, "bola");
    g_assert_cmpstr(tags[1], ==, "guda");
    g_assert_cmpstr(tags[2], ==, "arcoiro");
    g_assert_cmpstr(tags[3], ==, "asd");
    g_assert(tags[4] == NULL);
    g_strfreev(tags);
    g_assert(bb_post_parser_parse_tags(NULL) == NULL);
}


void
test_parse_post(void)
{
    balde_app_t *app = balde_app_init();
    bb_post_t *p = bb_post_parser_parse_post(app,
        "<!-- title: Moises e o Guda -->\n"
        "<!-- author: moises -->\n"
        "<!-- datetime: 2014-10-20 20:40:50 -->\n"
        "<!-- mdatetime: 2014-10-20 22:40:45 -->\n"
        "<!-- tags: bola, guda, arcoiro -->\n"
        "\n"
        "some random content here\n"
        "\n"
        "more random content here\n");
    g_assert(p != NULL);
    g_assert(p->metadata != NULL);
    g_assert(p->metadata->datetime != NULL);
    g_assert_cmpint(g_date_time_get_year(p->metadata->datetime), ==, 2014);
    g_assert_cmpint(g_date_time_get_month(p->metadata->datetime), ==, 10);
    g_assert_cmpint(g_date_time_get_day_of_month(p->metadata->datetime), ==, 20);
    g_assert_cmpint(g_date_time_get_hour(p->metadata->datetime), ==, 20);
    g_assert_cmpint(g_date_time_get_minute(p->metadata->datetime), ==, 40);
    g_assert_cmpint(g_date_time_get_seconds(p->metadata->datetime), ==, 50);
    g_assert(p->metadata->mdatetime != NULL);
    g_assert_cmpint(g_date_time_get_year(p->metadata->mdatetime), ==, 2014);
    g_assert_cmpint(g_date_time_get_month(p->metadata->mdatetime), ==, 10);
    g_assert_cmpint(g_date_time_get_day_of_month(p->metadata->mdatetime), ==, 20);
    g_assert_cmpint(g_date_time_get_hour(p->metadata->mdatetime), ==, 22);
    g_assert_cmpint(g_date_time_get_minute(p->metadata->mdatetime), ==, 40);
    g_assert_cmpint(g_date_time_get_seconds(p->metadata->mdatetime), ==, 45);
    g_assert_cmpstr(p->metadata->title, ==, "Moises e o Guda");
    g_assert_cmpstr(p->metadata->author, ==, "moises");
    g_assert(p->metadata->tags != NULL);
    g_assert_cmpstr(p->metadata->tags[0], ==, "bola");
    g_assert_cmpstr(p->metadata->tags[1], ==, "guda");
    g_assert_cmpstr(p->metadata->tags[2], ==, "arcoiro");
    g_assert(p->metadata->tags[3] == NULL);
    g_assert_cmpstr(p->raw_content, ==,
        "<!-- title: Moises e o Guda -->\n"
        "<!-- author: moises -->\n"
        "<!-- datetime: 2014-10-20 20:40:50 -->\n"
        "<!-- mdatetime: 2014-10-20 22:40:45 -->\n"
        "<!-- tags: bola, guda, arcoiro -->\n"
        "\n"
        "some random content here\n"
        "\n"
        "more random content here\n");
    g_assert_cmpstr(p->parsed_content, ==,
        "<!-- title: Moises e o Guda -->\n"
        "\n"
        "\n"
        "<!-- author: moises -->\n"
        "\n"
        "\n"
        "<!-- datetime: 2014-10-20 20:40:50 -->\n"
        "\n"
        "\n"
        "<!-- mdatetime: 2014-10-20 22:40:45 -->\n"
        "\n"
        "\n"
        "<!-- tags: bola, guda, arcoiro -->\n"
        "\n"
        "\n"
        "<p>some random content here</p>\n"
        "\n"
        "<p>more random content here</p>");
    bb_free_post(p);
    balde_app_free(app);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/post_parser/parse_metadata", test_parse_metadata);
    g_test_add_func("/post_parser/parse_datetime", test_parse_datetime);
    g_test_add_func("/post_parser/parse_tags", test_parse_tags);
    g_test_add_func("/post_parser/parse_post", test_parse_post);
    return g_test_run();
}
