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

GHashTable* bb_post_parser_parse_dates(const gchar *post);

#endif /* _BALDE_BLOG_POST_PARSER_PRIVATE_H */
