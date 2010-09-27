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
#include "message.h"

#include "color.h"
#include <libsnap/global.h>

#include <string.h>


void message_help(char *appname)
{
    color_printf( COLOR_NORMAL,      "Usage: " );
    color_printf( COLOR_BOLD,        "%s", appname );
    color_printf( COLOR_BOLD_YELLOW, " <options> " );
    color_printf( COLOR_BOLD_RED,    "<file>\n" );
    color_printf( COLOR_BOLD,   "\nEncode/decode a file in the S.N.A.P. format to the standard output.\n\n" );

    color_printf( COLOR_BOLD, "Options:\n" );
    color_printf( COLOR_BOLD_YELLOW, "  -e, --encode" );
    color_printf( COLOR_NORMAL,      "            \n" );
    color_printf( COLOR_BOLD_YELLOW, "  -d, --decode" );
    color_printf( COLOR_NORMAL,      "            \n" );
    color_printf( COLOR_BOLD_YELLOW, "  -a, --ack" );
    color_printf( COLOR_NORMAL,      "               enable acknowledgement flag\n" );
    color_printf( COLOR_BOLD_YELLOW, "  -A, --noack" );
    color_printf( COLOR_NORMAL,      "             disable acknowledgement flag (default)\n" );
    color_printf( COLOR_BOLD_YELLOW, "  -E, --edm=" );
    color_printf( COLOR_BOLD_GREEN,  "<action>" );
    color_printf( COLOR_NORMAL,      "      none|3times|checksum|crc8|crc16|crc32|fec\n" );
    color_printf( COLOR_BOLD_YELLOW, "  -s, --size=" );
    color_printf( COLOR_BOLD_GREEN,  "<value>" );
    color_printf( COLOR_NORMAL,      "      packet size (default 64)\n" );

    color_printf( COLOR_BOLD,        "Generic options:\n" );

    color_printf( COLOR_BOLD_YELLOW, "      --nocolor" );
    color_printf( COLOR_NORMAL,      "          disable colorized output\n" );
    color_printf( COLOR_BOLD_YELLOW, "  -h, --help" );
    color_printf( COLOR_NORMAL,      "             display this help and exit\n" );
    color_printf( COLOR_BOLD_YELLOW, "      --author" );
    color_printf( COLOR_NORMAL,      "           show author information and exit\n" );
    color_printf( COLOR_BOLD_YELLOW, "      --license" );
    color_printf( COLOR_NORMAL,      "          show license information and exit\n" );
    color_printf( COLOR_BOLD_YELLOW, "  -V, --version" );
    color_printf( COLOR_NORMAL,      "          show version information and exit\n\n" );

    color_printf( COLOR_BOLD,     "Arguments:\n" );
    color_printf( COLOR_BOLD_RED, "  <file>" );
    color_printf( COLOR_NORMAL,   "                 File to read from. Without FILE, or when FILE is -, read standard input.\n\n" );
}


void message_version(char *appname)
{
    printf( "%s version %s\n", appname, SNAP_VERSION );
}


void message_author(char *appname)
{
    printf(
        "%s was written by\n"
        "    Stefan Böhmann <kde@hilefoks.org>\n", appname
    );
}


void message_license(char *appname)
{
    SNAP_UNUSED( appname );

    printf(
        "Copyright (C) 2010 Stefan Böhmann.\n"
        "License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n"
    );
}


void message_spinner(char *appname)
{
    SNAP_UNUSED( appname );

    static int pos = 0;
    static char spinner[] = "/-\\|/-\\|/-\\|/-\\|\\-/|\\-/|\\-/|\\-/|";

    pos = ( pos + 1 ) % strlen( spinner );
    printf( "\b\b %c", spinner[ pos ] );
    fflush( stdout );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
