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
#include "color.h"
#include <stdarg.h>


bool color_print_enable_colors = true;


int color_vfprintf(FILE *fp, const char *color, const char *fmt, va_list args, const char *trail)
{
    int r = 0;

    if( color_print_enable_colors && *color ) {
        r += fprintf( fp, "%s", color );
    }

    r += vfprintf( fp, fmt, args);

    if( color_print_enable_colors && *color ) {
        r += fprintf( fp, "%s", COLOR_RESET) ;
    }

    if( trail ) {
        r += fprintf( fp, "%s", trail );
    }

    return r;
}



int color_fprintf(FILE *fp, const char *color, const char *fmt, ...)
{
    va_list args;
    int r;

    va_start( args, fmt );
    r = color_vfprintf( fp, color, fmt, args, NULL );
    va_end( args );

    return r;
}


int color_printf(const char *color, const char *fmt, ...)
{
    va_list args;
    int r;

    va_start( args, fmt );
    r = color_vfprintf( stdout, color, fmt, args, NULL );
    va_end( args );

    return r;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
