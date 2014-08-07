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
#include <balde-blog/helpers-private.h>


bb_post_t*
bb_get_post_by_slug(balde_app_t *app, const gchar *slug)
{
    bb_user_data_t *ud = app->user_data;
    for (GList *tmp = ud->posts; tmp != NULL; tmp = g_list_next(tmp)) {
        bb_item_t *item = tmp->data;
        if(!g_strcmp0(slug, item->slug))
            return item->post;
    }
    return NULL;
}


GList*
bb_get_posts_by_page(balde_app_t *app, guint page, guint posts_per_page)
{
    GList *rv = NULL;
    guint index = (page - 1) * posts_per_page;
    guint offset = index + posts_per_page;
    bb_user_data_t *ud = app->user_data;
    guint c = 0;
    for (GList *tmp = ud->posts; tmp != NULL; tmp = g_list_next(tmp)) {
        if (c >= index && c < offset)
            rv = g_list_append(rv, tmp->data);
        c++;
    }
    return rv;
}
