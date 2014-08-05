/*
 * balde-blog: A really simple blog application built with balde.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BALDE_BLOG_POST_LOADER_PRIVATE_H
#define _BALDE_BLOG_POST_LOADER_PRIVATE_H

#include <glib.h>
#include <balde-blog/post-parser-private.h>

typedef struct {
    gchar *slug;
    bb_post_t *post;
    GError *error;
} bb_item_t;

void bb_free_item(bb_item_t *item);
void bb_free_items(GList *items);
gchar* bb_post_loader_parse_slug(const gchar *filepath);
bb_item_t* bb_post_loader_load_from_file(const gchar *filepath);
GList* bb_post_loader_load_from_directory(const gchar *dirpath);

#endif /* _BALDE_BLOG_POST_LOADER_PRIVATE_H */
