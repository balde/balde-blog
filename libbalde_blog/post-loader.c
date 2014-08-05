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


void
bb_free_item(bb_item_t *item)
{
    if (item == NULL)
        return;
    bb_free_post(item->post);
    g_free(item->slug);
    if (item->error != NULL)
        g_error_free(item->error);
    g_free(item);
}


void
bb_free_items(GList *items)
{
    g_list_free_full(items, (GDestroyNotify) bb_free_item);
}


gchar*
bb_post_loader_parse_slug(const gchar *filepath)
{
    if (filepath == NULL)
        return NULL;
    GError *tmp_error = NULL;
    // the slug does not accepts slashes for now. or at least until I find a
    // proper way to guess slugs from the full file path without ignoring slashes.
    GRegex *re_slug = g_regex_new("([^/]+).(md|markdown)$", 0, 0, &tmp_error);
    if (tmp_error != NULL) {
        g_error_free(tmp_error);
        return NULL;
    }
    GMatchInfo *match;
    g_regex_match(re_slug, filepath, 0, &match);
    gchar* rv = NULL;
    if (!g_match_info_matches(match))
        goto err;
    rv = g_match_info_fetch(match, 1);
err:
    g_match_info_free(match);
    g_regex_unref(re_slug);
    return rv;
}


bb_item_t*
bb_post_loader_load_from_file(const gchar *filepath)
{
    if (filepath == NULL)
        return NULL;
    gchar *slug = bb_post_loader_parse_slug(filepath);
    if (slug == NULL)
        return NULL;
    gchar* contents;
    bb_item_t *rv = g_new(bb_item_t, 1);
    rv->error = NULL;
    rv->slug = slug;
    rv->post = NULL;
    if (!g_file_get_contents(filepath, &contents, NULL, &(rv->error)))
        return rv;
    rv->post = bb_post_parser_parse_post(contents);
    g_free(contents);
    return rv;
}


static gint
bb_post_loader_compare_items(bb_item_t *a, bb_item_t *b)
{
    if (a == NULL || a->error != NULL || b == NULL || b->error != NULL)
        return -1;  // stack errors in the begin of the list.
    gint cmp = g_date_time_compare(a->post->metadata->datetime, b->post->metadata->datetime);
    return cmp * -1;
}


GList*
bb_post_loader_load_from_directory(const gchar *dirpath)
{
    if (dirpath == NULL)
        return NULL;
    GError *tmp_error = NULL;
    GDir *dir = g_dir_open(dirpath, 0, &tmp_error);
    if (tmp_error != NULL) {
        // buh! nothing to do if we can't open the dir :P
        g_error_free(tmp_error);
        return NULL;
    }
    GList *rv = NULL;
    const gchar *tmp;
    while ((tmp = g_dir_read_name(dir)) != NULL) {
        gchar *filepath = g_build_filename(dirpath, tmp, NULL);
        if (!g_file_test(filepath, G_FILE_TEST_IS_REGULAR))
            goto clean;
        bb_item_t *item = bb_post_loader_load_from_file(filepath);
        if (item == NULL)
            goto clean;
        rv = g_list_insert_sorted(rv, item,
            (GCompareFunc) bb_post_loader_compare_items);
clean:
        g_free(filepath);
    }
    g_dir_close(dir);
    return rv;
}
