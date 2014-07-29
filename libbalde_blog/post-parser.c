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


static void
bb_post_parser_free_datetime(GDateTime *dt)
{
    if (dt == NULL)
        return;
    g_date_time_unref(dt);
}


GHashTable*
bb_post_parser_parse_dates(const gchar *post)
{
    if (post == NULL)
        return NULL;
    GError *tmp_error = NULL;
    GRegex *re_date = g_regex_new(
        "<!-- +([a-z]+): +([0-9]{4})-([0-9]{2})-([0-9]{2}) "
        "([0-9]{2}):([0-9]{2}):([0-9]{2}) +-->", 0, 0, &tmp_error);
    if (tmp_error != NULL) {
        g_error_free(tmp_error);
        return NULL;
    }
    GHashTable *dates = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
        (GDestroyNotify) bb_post_parser_free_datetime);
    GMatchInfo *match;
    g_regex_match(re_date, post, 0, &match);
    while (g_match_info_matches(match)) {
        gchar *tmp;
        gchar *key = g_match_info_fetch(match, 1);
        tmp = g_match_info_fetch(match, 2);
        gint year = (gint) g_ascii_strtoll(tmp, NULL, 10);
        g_free(tmp);
        tmp = g_match_info_fetch(match, 3);
        gint month = (gint) g_ascii_strtoll(tmp, NULL, 10);
        g_free(tmp);
        tmp = g_match_info_fetch(match, 4);
        gint day = (gint) g_ascii_strtoll(tmp, NULL, 10);
        g_free(tmp);
        tmp = g_match_info_fetch(match, 5);
        gint hour = (gint) g_ascii_strtoll(tmp, NULL, 10);
        g_free(tmp);
        tmp = g_match_info_fetch(match, 6);
        gint minute = (gint) g_ascii_strtoll(tmp, NULL, 10);
        g_free(tmp);
        tmp = g_match_info_fetch(match, 7);
        gdouble seconds = (gdouble) g_ascii_strtoll(tmp, NULL, 10);
        g_free(tmp);
        GDateTime *tmp_dt = g_date_time_new_utc(year, month, day, hour, minute,
            seconds);
        g_hash_table_insert(dates, key, tmp_dt);
        g_match_info_next (match, NULL);
    }
    g_match_info_free(match);
    g_regex_unref(re_date);
    return dates;
}
