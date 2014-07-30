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
#include <balde-markdown.h>
#include <balde-blog/post-parser-private.h>


void
bb_free_post_metadata(bb_post_metadata_t *m)
{
    if (m == NULL)
        return;
    g_free(m->title);
    g_free(m->author);
    g_strfreev(m->tags);
    if (m->datetime != NULL)
        g_date_time_unref(m->datetime);
    if (m->mdatetime != NULL)
        g_date_time_unref(m->mdatetime);
    g_free(m);
}


void
bb_free_post(bb_post_t *p)
{
    g_free(p->raw_content);
    g_free(p->parsed_content);
    bb_free_post_metadata(p->metadata);
    g_free(p);
}


GHashTable*
bb_post_parser_parse_metadata(const gchar *post)
{
    if (post == NULL)
        return NULL;
    GError *tmp_error = NULL;
    GRegex *re_metadata = g_regex_new("^<!-- *([a-z]+): *(.*\\S) *-->$",
        G_REGEX_MULTILINE, 0, &tmp_error);
    if (tmp_error != NULL) {
        g_error_free(tmp_error);
        return NULL;
    }
    GHashTable *metadata = g_hash_table_new_full(g_str_hash, g_str_equal,
        g_free, g_free);
    GMatchInfo *match;
    g_regex_match(re_metadata, post, 0, &match);
    while (g_match_info_matches(match)) {
        g_hash_table_insert(metadata, g_match_info_fetch(match, 1),
            g_match_info_fetch(match, 2));
        g_match_info_next (match, NULL);
    }
    g_match_info_free(match);
    g_regex_unref(re_metadata);
    return metadata;
}


GDateTime*
bb_post_parser_parse_datetime(const gchar *dt)
{
    if (dt == NULL)
        return NULL;
    GError *tmp_error = NULL;
    GRegex *re_datetime = g_regex_new(
        "([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})",
        0, 0, &tmp_error);
    if (tmp_error != NULL) {
        g_error_free(tmp_error);
        return NULL;
    }
    GDateTime *tmp_dt = NULL;
    GMatchInfo *match;
    g_regex_match(re_datetime, dt, 0, &match);
    if (!g_match_info_matches(match))
        goto err;
    gchar *tmp = g_match_info_fetch(match, 1);
    gint year = (gint) g_ascii_strtoll(tmp, NULL, 10);
    g_free(tmp);
    tmp = g_match_info_fetch(match, 2);
    gint month = (gint) g_ascii_strtoll(tmp, NULL, 10);
    g_free(tmp);
    tmp = g_match_info_fetch(match, 3);
    gint day = (gint) g_ascii_strtoll(tmp, NULL, 10);
    g_free(tmp);
    tmp = g_match_info_fetch(match, 4);
    gint hour = (gint) g_ascii_strtoll(tmp, NULL, 10);
    g_free(tmp);
    tmp = g_match_info_fetch(match, 5);
    gint minute = (gint) g_ascii_strtoll(tmp, NULL, 10);
    g_free(tmp);
    tmp = g_match_info_fetch(match, 6);
    gdouble seconds = (gdouble) g_ascii_strtoll(tmp, NULL, 10);
    g_free(tmp);
    tmp_dt = g_date_time_new_utc(year, month, day, hour, minute, seconds);
err:
    g_match_info_free(match);
    g_regex_unref(re_datetime);
    return tmp_dt;
}


gchar**
bb_post_parser_parse_tags(const gchar *tags_str)
{
    if (tags_str == NULL)
        return NULL;
    gchar **tags = g_strsplit(tags_str, ",", 0);
    for (guint i = 0; tags[i] != NULL; i++)
        g_strstrip(tags[i]);
    return tags;
}


bb_post_t*
bb_post_parser_parse_post(balde_app_t *app, const gchar *post)
{
    GHashTable *metadata = bb_post_parser_parse_metadata(post);
    if (metadata == NULL)  // probably a bad post file
        return NULL;
    if (g_hash_table_size(metadata) == 0) {  // same as above
        g_hash_table_destroy(metadata);
        return NULL;
    }
    bb_post_metadata_t *m = g_new(bb_post_metadata_t, 1);
    m->title = g_strdup(g_hash_table_lookup(metadata, "title"));
    m->author = g_strdup(g_hash_table_lookup(metadata, "author"));
    m->tags = bb_post_parser_parse_tags(g_hash_table_lookup(metadata, "tags"));
    m->datetime = bb_post_parser_parse_datetime(g_hash_table_lookup(metadata,
        "datetime"));
    m->mdatetime = bb_post_parser_parse_datetime(g_hash_table_lookup(metadata,
        "mdatetime"));
    bb_post_t *p = g_new(bb_post_t, 1);
    p->raw_content = g_strdup(post);
    p->parsed_content = balde_markdown_parse(app, p->raw_content);
    p->metadata = m;
    return p;
}
