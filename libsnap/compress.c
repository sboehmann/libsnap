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
#ifdef __cplusplus
extern "C" {
#endif

#include "compress.h"
#include "global.h"

#include <stdlib.h>

#ifdef WITH_ZLIB
#include <zlib.h>
#endif

#ifdef WITH_LZMA
#include <lzma.h>
#endif


bool snap_lzma_builtin(void)
{
#ifdef WITH_LZMA
    return true;
#endif

    return false;
}


size_t snap_lzma_bound(size_t size)
{
#ifdef WITH_LZMA
    return lzma_stream_buffer_bound( size );
#endif

    return size;
}


bool snap_lzma_encode(const uint8_t *in_data, int in_size, uint8_t *out_data, int out_size, const snap_allocator_t *allocator)
{
    //TODO
    SNAP_UNUSED( in_data );
    SNAP_UNUSED( in_size );
    SNAP_UNUSED( out_data );
    SNAP_UNUSED( out_size );
    SNAP_UNUSED( allocator );

    return false;
}


bool snap_lzma_decode(const uint8_t *in_data, int in_size, uint8_t *out_data, int out_size, const snap_allocator_t *allocator)
{
    //TODO
    SNAP_UNUSED( in_data );
    SNAP_UNUSED( in_size );
    SNAP_UNUSED( out_data );
    SNAP_UNUSED( out_size );
    SNAP_UNUSED( allocator );

    return false;
}


bool snap_zlib_builtin(void)
{
#ifdef WITH_ZLIB
    return true;
#endif
    return false;
}


size_t snap_zlib_bound(size_t size)
{
#ifdef WITH_ZLIB
    return compressBound( size );
#endif
    return size;
}


bool snap_zlib_encode(const uint8_t *in_data, int in_size, uint8_t *out_data, int out_size, const snap_allocator_t *allocator)
{
    //TODO
    SNAP_UNUSED( in_data );
    SNAP_UNUSED( in_size );
    SNAP_UNUSED( out_data );
    SNAP_UNUSED( out_size );
    SNAP_UNUSED( allocator );

    return false;
}


bool snap_zlib_decode(const uint8_t *in_data, int in_size, uint8_t *out_data, int out_size, const snap_allocator_t *allocator)
{
    //TODO
    SNAP_UNUSED( in_data );
    SNAP_UNUSED( in_size );
    SNAP_UNUSED( out_data );
    SNAP_UNUSED( out_size );
    SNAP_UNUSED( allocator );

    return false;
}



#ifdef __cplusplus
}   // end extern "C"
#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
