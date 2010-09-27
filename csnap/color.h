/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
 *
 * based on code from Git
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
#ifndef COLOR_H
#define COLOR_H

/**
 * @file csnap/color.h
 * @ingroup csnap
 */

#include <stdio.h>
#include <stdbool.h>

#define COLOR_NORMAL        ""
#define COLOR_RESET         "\033[m"
#define COLOR_BOLD          "\033[1m"
#define COLOR_RED           "\033[31m"
#define COLOR_GREEN         "\033[32m"
#define COLOR_YELLOW        "\033[33m"
#define COLOR_BLUE          "\033[34m"
#define COLOR_MAGENTA       "\033[35m"
#define COLOR_CYAN          "\033[36m"
#define COLOR_BOLD_RED      "\033[1;31m"
#define COLOR_BOLD_GREEN    "\033[1;32m"
#define COLOR_BOLD_YELLOW   "\033[1;33m"
#define COLOR_BOLD_BLUE     "\033[1;34m"
#define COLOR_BOLD_MAGENTA  "\033[1;35m"
#define COLOR_BOLD_CYAN     "\033[1;36m"
#define COLOR_BG_RED        "\033[41m"
#define COLOR_BG_GREEN      "\033[42m"
#define COLOR_BG_YELLOW     "\033[43m"
#define COLOR_BG_BLUE       "\033[44m"
#define COLOR_BG_MAGENTA    "\033[45m"
#define COLOR_BG_CYAN       "\033[46m"

extern bool color_print_enable_colors;

__attribute__((format (printf, 2, 3)))
extern int color_printf(const char *color, const char *fmt, ...);

__attribute__((format (printf, 3, 4)))
extern int color_fprintf(FILE *fp, const char *color, const char *fmt, ...);

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
