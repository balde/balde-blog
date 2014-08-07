/*
 * balde-blog: A really simple blog application built with balde.
 * Copyright (C) 2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BALDE_BLOG_HELPERS_PRIVATE_H
#define _BALDE_BLOG_HELPERS_PRIVATE_H

#include <glib.h>
#include <balde-blog/post-parser-private.h>

bb_post_t* bb_get_post_by_slug(balde_app_t *app, const gchar *slug);
GList* bb_get_posts_by_page(balde_app_t *app, guint page, guint posts_per_page);

#endif /* _BALDE_BLOG_HELPERS_PRIVATE_H */
