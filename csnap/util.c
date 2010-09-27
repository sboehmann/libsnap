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
#include "util.h"

#include <string.h>
#include <libsnap/global.h>

#define __USE_XOPEN_EXTENDED
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>



bool str_to_uint32(const char *value, uint32_t min, uint32_t max, uint32_t *result)
{
    SNAP_CHECK_PTR( result );

    *result = 0;

    while( *value == ' ' || *value == '\t' ) {
        ++value;
    }

    if( strcmp( value, "max" ) == 0 ) {
        *result = max;
        return true;
    }

    if( strcmp( value, "min" ) == 0 ) {
        *result = min;
        return true;
    }

    do {
        // Don't overflow.
        if( *result > ( UINT32_MAX - 9 ) / 10 ) {
            *result = STR_TO_UINT32_OVERFLOW_ERROR;
            return false;
        }

        *result *= 10;
        *result += *value - '0';
        ++value;
    }
    while( *value >= '0' && *value <= '9' );

    if( *result < min ) {
        *result = STR_TO_UINT32_MIN_LIMIT_ERROR;
        return false;
    }

    if( *result > max ) {
        *result = STR_TO_UINT32_MAX_LIMIT_ERROR;
        return false;
    }

    return result;
}


void milli_sleep(unsigned long milliseconds)
{
    usleep( milliseconds * 1000 );
}


bool is_dir(const char *path)
{
  struct stat stats;
  return stat( path, &stats ) == 0 && S_ISDIR( stats.st_mode );
}


/* Read the contents of a file into a buffer.  Return the size of the file
 * and set buf to point to a buffer allocated with malloc that contains
 * the file contents.
 */
size_t read_file(FILE *fp, uint8_t **buf)
{
    const size_t CHUNK = 1024;

    size_t n, np, r;
    uint8_t *buffer_1, *buffer_2;

    n = CHUNK;
    np = n;
    buffer_1 = malloc( n );

    while( ( r = fread( buffer_1, 1, CHUNK, fp) ) > 0) {
        n += r;
        if( np - n < CHUNK ) { // buffer is too small, the next read could overflow!
            np *= 2;
            buffer_2 = malloc( np );
            memcpy( buffer_2, buffer_1, n );
            free( buffer_1 );
            buffer_1 = buffer_2;
        }
    }

    *buf = buffer_1;

    return n;
}









uint8_t* fread_file(FILE * stream, size_t *length)
{
    const size_t CHUNK = 1024;

    uint8_t *buf = malloc( CHUNK );

    size_t alloc = CHUNK;
    size_t size = 0;

    for(;;) {
        size_t count;
        size_t requested;

        if( size >= alloc ) {
            alloc += CHUNK;
            uint8_t *new_buf = realloc( buf, alloc );

            if( !new_buf ) {
                perror( "fread_file" );
                exit( EXIT_FAILURE );
            }

            buf = new_buf;
        }

        requested = alloc - size - 1;
        count = fread (buf + size, 1, requested, stream);
        size += count;

        if( count != requested ) {
            buf[ size ] = '\0';
            *length = size;
            return buf;
        }
    }

    free( buf );
    return NULL;
}























// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
