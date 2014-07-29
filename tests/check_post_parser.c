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

#include <balde-blog/post-parser-private.h>


void
test_parse_dates(void)
{
    GHashTable *dates = bb_post_parser_parse_dates(
        "some random content here\n"
        "<!-- date: 2014-10-20 20:40:50 -->\n"
        "\n"
        "<!-- mdate: 2034-11-16 12:34:45 -->\n"
        "more random content here\n");
    g_assert(dates != NULL);
    g_assert(g_hash_table_size(dates) == 2);
    GDateTime *date = g_hash_table_lookup(dates, "date");
    GDateTime *mdate = g_hash_table_lookup(dates, "mdate");
    g_date_time_ref(date);
    g_date_time_ref(mdate);
    g_hash_table_destroy(dates);
    g_assert(date != NULL);
    g_assert(g_date_time_get_year(date) == 2014);
    g_assert(g_date_time_get_month(date) == 10);
    g_assert(g_date_time_get_day_of_month(date) == 20);
    g_assert(g_date_time_get_hour(date) == 20);
    g_assert(g_date_time_get_minute(date) == 40);
    g_assert(g_date_time_get_seconds(date) == 50);
    g_date_time_unref(date);
    g_assert(mdate != NULL);
    g_assert(g_date_time_get_year(mdate) == 2034);
    g_assert(g_date_time_get_month(mdate) == 11);
    g_assert(g_date_time_get_day_of_month(mdate) == 16);
    g_assert(g_date_time_get_hour(mdate) == 12);
    g_assert(g_date_time_get_minute(mdate) == 34);
    g_assert(g_date_time_get_seconds(mdate) == 45);
    g_date_time_unref(mdate);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/post-parser/parse_dates", test_parse_dates);
    return g_test_run();
}
