/*
 * Copyright 2010  Stefan Böhmann <kde@hilefoks.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBSNAP_COMPRESS_H
#define LIBSNAP_COMPRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libsnap/global.h>

/**
 * @file libsnap/compress.h
 * @ingroup libsnap
 * @brief In-Memory compression and decompression
 *
 * @version 0.1
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


extern bool snap_lzma_builtin(void);

/**
 * @brief Calculate output buffer size for lzma
 *
 * When trying to compress uncompressible data, the encoded size will be
 * slightly bigger than the input data. This function calculates how much
 * output buffer space is required for snap_XXX_encode().
 *
 * The calculated value is not exact, but it is guaranteed to be big enough.
 * The actual maximum output space required may be slightly smaller.
 * This should not be a problem in practice.
 *
 * @note The limit calculated by this function applies only to single-call encoding.
 *       Multi-call encoding may (and probably will) have larger maximum expansion
 *       when encoding uncompressible data
 *
 * @param size Size of the input data
 *
 * @return Maximum size of output data
 */
extern size_t snap_lzma_bound(size_t size);

/**
 * @brief lzma data stream encoder
 *
 * @param in_data    Beginning of the input buffer
 * @param in_size    Size of the input buffer
 * @param out_data   Beginning of the output buffer
 * @param out_size   Size of the out buffer
 * @param allocator  A pointer to a snap_allocator_t structure or NULL, if the
 *                   default malloc() and free() memory functions should be used.
 *
 * @return Returns true if the encoding was successful, otherwise returns false.
 */
extern bool snap_lzma_encode( const uint8_t *in_data, int in_size,
                              uint8_t *out_data, int out_size,
                              const snap_allocator_t *allocator );

/**
 * @brief lzma data stream decoder
 *
 * @param in_data   Beginning of the input buffer
 * @param in_size   Size of the input buffer
 * @param out_data  Beginning of the output buffer
 * @param out_size  Size of the out buffer
 * @param allocator  A pointer to a snap_allocator_t structure or NULL, if the
 *                   default malloc() and free() memory functions should be used.
 *
 * @return Returns true if the decoding was successful, otherwise returns false.
 */
extern bool snap_lzma_decode( const uint8_t *in_data, int in_size,
                              uint8_t *out_data, int out_size,
                              const snap_allocator_t *allocator );



extern bool snap_zlib_builtin(void);

/**
 * @brief Calculate output buffer size for zlib
 * @copydoc snap_lzma_bound
 */
extern size_t snap_zlib_bound(size_t size);

/**
 * @brief zlib data stream encoder
 * @copydoc snap_lzma_encode
 */
extern bool snap_zlib_encode( const uint8_t *in_data, int in_size,
                              uint8_t *out_data, int out_size,
                              const snap_allocator_t *allocator );

/**
 * @brief zlib data stream decoder
 * @copydoc snap_lzma_decode
 */
extern bool snap_zlib_decode( const uint8_t *in_data, int in_size,
                              uint8_t *out_data, int out_size,
                              const snap_allocator_t *allocator );

#ifdef __cplusplus
}   // end extern "C"
#endif


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

