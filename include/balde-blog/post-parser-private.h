/*
 * balde-blog: A really simple blog application built with balde.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BALDE_BLOG_POST_PARSER_PRIVATE_H
#define _BALDE_BLOG_POST_PARSER_PRIVATE_H

#include <glib.h>

typedef struct {
    gchar *title;
    gchar *author;
    gchar **tags;
    GDateTime *datetime;
    GDateTime *mdatetime;
} bb_post_metadata_t;

typedef struct {
    gchar *raw_content;
    bb_post_metadata_t *metadata;
} bb_post_t;

void bb_free_post_metadata(bb_post_metadata_t *m);
void bb_free_post(bb_post_t *p);
GHashTable* bb_post_parser_parse_metadata(const gchar *post);
GDateTime* bb_post_parser_parse_datetime(const gchar *dt);
gchar** bb_post_parser_parse_tags(const gchar *tags_str);
bb_post_t* bb_post_parser_parse_post(const gchar *post);

#endif /* _BALDE_BLOG_POST_PARSER_PRIVATE_H */
