/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @file csnap/util.h
 * @ingroup csnap
 */

#define STR_TO_UINT32_OVERFLOW_ERROR 1;
#define STR_TO_UINT32_MIN_LIMIT_ERROR 2;
#define STR_TO_UINT32_MAX_LIMIT_ERROR 3;

extern bool str_to_uint32(const char *value, uint32_t min, uint32_t max, uint32_t *result);

extern void milli_sleep(unsigned long milliseconds);

extern bool is_dir(const char *path);
size_t read_file(FILE *fp, uint8_t **buf);

uint8_t* fread_file(FILE * stream, size_t *length);

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
