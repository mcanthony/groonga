/* -*- c-basic-offset: 2; coding: utf-8 -*- */
/*
  Copyright (C) 2015  Kouhei Sutou <kou@clear-code.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <groonga.h>

#include <gcutter.h>
#include <glib/gstdio.h>

#include "../lib/grn-assertions.h"

static gchar *tmp_directory;
static gchar *path;
static grn_ctx context;
static grn_obj *database;

void test_set_and_get(void);
void test_get_nonexistent(void);

void
cut_startup(void)
{
  tmp_directory = g_build_filename(grn_test_get_tmp_dir(),
                                   "test-conf",
                                   NULL);
}

void
cut_shutdown(void)
{
  g_free(tmp_directory);
}

void
cut_setup(void)
{
  cut_remove_path(tmp_directory, NULL);
  g_mkdir_with_parents(tmp_directory, 0700);
  path = g_build_filename(tmp_directory, "text-conf", NULL);
  grn_ctx_init(&context, 0);
  database = grn_db_create(&context, path, NULL);
}

void
cut_teardown(void)
{
  grn_obj_close(&context, database);
  grn_ctx_fin(&context);
  cut_remove_path(tmp_directory, NULL);
  g_free(path);
}

void
test_set_and_get(void)
{
  const char *value;
  uint32_t value_size;

  grn_test_assert(grn_conf_set(&context, "key", -1, "value", -1));
  grn_test_assert(grn_conf_get(&context, "key", -1, &value, &value_size));

  cut_assert_equal_memory("value", strlen("value"),
                          value, value_size);
}

void
test_get_nonexistent(void)
{
  const char *value;
  uint32_t value_size;

  grn_test_assert(grn_conf_get(&context,
                               "noneixstent", -1,
                               &value, &value_size));

  cut_assert_equal_memory(NULL, 0,
                          value, value_size);
}

