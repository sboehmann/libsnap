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
/** @defgroup libsnap libsnap, the S.N.A.P network protocol library */

#ifndef LIBSNAP_GLOBAL_H
#define LIBSNAP_GLOBAL_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef UNIT_TEST
#include <CUnit/CUnit.h>
#endif

#ifndef SNAP_NO_DEBUG
#include <assert.h>
#endif


/**
 * @file libsnap/global.h
 * @ingroup libsnap
 * @brief S.N.A.P network protocol implementation
 *
 * The S.N.A.P network protocol defines a simple and generic network protocol
 * that can be used in many different environments.
 *
 * Have fun!
 *
 * @version 0.1
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


/**
 * @def SNAP_UNUSED(name)
 * @brief Macro to avoid "unused parameter" warings
 *
 * Indicates to the compiler that the parameter with the specified name is not
 * used in the body of a function. This can be used to suppress compiler warnings
 * while allowing functions to be defined with meaningful parameter names in
 * their signatures.
 *
 * @code
 * void function(int unusedValue)
 * {
 *     SNAP_UNUSED( unusedValue );
 *     ...
 * }
 * @endcode
 */
#define SNAP_UNUSED(name) (void)(name)


/**
 * @def SNAP_CHECK_PTR(ptr)
 * @brief Check, if the pointer is not null
 *
 * if pointer is 0, prints a warning message containing the source code's
 * file name and line number, saying that the program ran out of memory.
 *
 * SNAP_CHECK_PTR does nothing if SNAP_NO_DEBUG was defined during compilation.
 *
 * @code
 * int *iptr;
 *
 * SNAP_CHECK_PTR( iptr = malloc( sizeof( int) ); // WRONG!
 * iptr = malloc( sizeof( int ) );                // Right
 * SNAP_CHECK_PTR( iptr );
 * @endcode
 */
#if defined UNIT_TEST
#define SNAP_CHECK_PTR(ptr) \
    CU_ASSERT_PTR_NOT_NULL_FATAL( ptr )
#elif defined SNAP_NO_DEBUG
#define SNAP_CHECK_PTR(ptr) \
    (void)(ptr)
#else
#define SNAP_CHECK_PTR(ptr) \
        assert(ptr)
#endif


/**
 * @def SNAP_ASSERT(test)
 * @brief Prints a warning message if test is false.
 *
 * Prints a warning message if test is false containing the source code
 * file name and line number.
 *
 * SNAP_ASSERT() is useful for testing pre- and post-conditions during development.
 * It does nothing if SNAP_NO_DEBUG was defined during compilation.
 *
 * @code
 * int divide(int a, int b)
 * {
 *     SNAP_ASSERT( b != 0 )
 *     return a / b;
 * }
 * @endcode
 */
#if defined UNIT_TEST
#define SNAP_ASSERT(test) \
        CU_ASSERT(test)
#elif defined SNAP_NO_DEBUG
#define SNAP_ASSERT(test)
#else
#define SNAP_ASSERT(test)   \
        assert( test );
#endif


/**
 * @def forever
 * @brief This macro is provided for convenience for writing infinite loops.
 *
 * @code
 * forever {
 *     ...
 * }
 * @endcode
 * It is equivalent to for (;;)
 */
#ifndef forever
#define forever for(;;)
#endif


/**
 * @enum SnapDecodeError
 */
typedef enum
{
    SNAP_ERROR_NONE,
    SNAP_ERROR_BOUND,
    SNAP_ERROR_STATE,
    SNAP_ERROR_UNKNOWN_EDM_TYPE,    // illegal edm type value received
    SNAP_ERROR_UNKNOWN_NDB_VALUE,   // illegal ndb value received
    SNAP_ERROR_INVALID_EDM,         // wrong checksum/crc value
    SNAP_ERROR_INVALID_ACK_VALUE,
    SNAP_ERROR_UNKNOWN
} SnapErrorCode;


/**
 * @enum SnapCompressMode
 * @brief Snap Compress Mode
 *
 * The kind of compression method used to compress packet data.
 *
 * @see snap_get_compress_mode
 * @see snap_set_compress_mode
 * @see snap_get_user_compress_handler
 * @see snap_set_user_compress_handler
 */
typedef enum
{
    COMPRESS_NONE = 0,     ///<
    COMPRESS_LZMA = 1,     ///<
    COMPRESS_ZLIB = 2,     ///<
    COMPRESS_USER = 200    ///<
} SnapCompressMode;



/**
 * @enum SnapErrorDetectionMode
 * @brief Error Detection Mode (EDM)
 *
 * The kind of error detecting method used to validate the packet.
 *
 * @see snap_get_error_detection_mode_size
 * @see snap_get_error_detection_mode
 * @see snap_set_error_detection_mode
 * @see snap_get_user_error_detection_handler
 * @see snap_set_user_error_detection_handler
 */
typedef enum
{
    EDM_NONE = 0,     ///< No error detection
    EDM_THREETIMES,   ///< 3 times re-transmission
    EDM_CHECKSUM,     ///< 8 bit checksum
    EDM_CRC8,         ///< 8-bit CRC (known as DOW-CRC or Dallas/Maxim 1-Wire Bus)
    EDM_CRC16,        ///< 16-bit CRC (CRC-CCITT)
    EDM_CRC32,        ///< 32-bit CRC (IEEE802.3)
    EDM_FEC,          ///< forward error correction
    EDM_USER = 200    ///<
} SnapErrorDetectionMode;



/**
 * @struct snap_allocator_t
 * @brief Custom functions for memory handling
 *
 * A pointer to snap_allocator_t may be passed via snap_t structure
 * to libsnap, and some advanced functions take a pointer to snap_allocator_t
 * as a separate function argument. The library will use the functions
 * specified in snap_allocator_t for memory handling instead of the default
 * malloc() and free(). C++ users should note that the custom memory
 * handling functions must not throw exceptions.
 *
 * libsnap doesn't make an internal copy of snap_allocator_t. Thus, it is
 * OK to change these function pointers in the middle of the coding
 * process, but obviously it must be done carefully to make sure that the
 * replacement `free' can deallocate memory allocated by the earlier
 * `alloc' function(s).
 */
typedef struct
{
    /**
     * @brief Pointer to a custom memory allocation function
     *
     * If you don't want a custom allocator, but still want
     * custom free(), set this to NULL and libsnap will use
     * the standard malloc().
     *
     * @note The allocator should not waste time zeroing the allocated buffers.
     *       This is not only about speed, but also memory usage, since the
     *       operating system kernel doesn't necessarily allocate the requested
     *       memory in physical memory until it is actually used.
     *
     * @param size Size of an element in bytes
     * @param opaque snap_allocator_t.opaque (see below)
     *
     * @return Pointer to the beginning of a memory block of `size' bytes,
     *         or NULL if allocation fails for some reason.
     */
    void* (*alloc)(size_t size, long opaque);

    /**
     * \brief Pointer to a custom memory freeing function
     *
     * If you don't want a custom freeing function, but still want a custom
     * allocator, set this to NULL and libsnap will use the standard free().
     *
     * @param ptr  Pointer returned by snap_allocator.alloc(), or when it is set
     *             to NULL, a pointer returned by the standard malloc().
     * @param opaque snap_allocator_t.opaque (see below)
     */
    void (*free)(void *ptr, long opaque);

    /**
     * @brief Custom data passed to .alloc() and .free()
     *
     * opaque is passed as the first argument to snap_allocator_t.alloc() and
     * snap_allocator_t.free(). This intended to ease implementing custom memory
     * allocation functions for use with libsnap.
     */
    long opaque;
} snap_allocator_t;



/**
 * @struct snap_statistic_t
 * @brief The snap_statistic_t struct provides detailed statistical information.
 *
 * @see snap_get_statistic
 * @see snap_clear_statistic
 */
typedef struct
{
    uint64_t rejected_packets;          ///< number of rejected packets
    uint64_t transmitted_packets;       ///< number of transmitted packets
    uint64_t received_packets;          ///< number of received packets
    uint64_t total_packets;             ///< total number of packets
    uint64_t rejected_bytes;            ///< number of rejected bytes
    uint64_t transmitted_bytes;         ///< number of transmitted bytes
    uint64_t received_bytes;            ///< number of received bytes
    uint64_t total_bytes;               ///< total number of bytes
    uint64_t transmitted_payload_bytes; ///<
    uint64_t received_payload_bytes;    ///<
    uint64_t total_payload_bytes;       ///<
    uint64_t transmitted_fill_bytes;    ///<
    uint64_t received_fill_bytes;       ///<
    uint64_t total_fill_bytes;          ///<
} snap_statistic_t;



/**
 * @struct snap_compress_handler_t
 * @brief Functions for data compression handling
 */
typedef struct
{
    size_t (*bound)(size_t size, long opaque );

    bool (*encode)( const uint8_t *in_data, int in_size, uint8_t *out_data,
                    int out_size, long opaque );

    bool (*decode)( const uint8_t *in_data, int in_size, uint8_t *out_data,
                    int out_size, long opaque );

    /**
     * @brief custom data passed to .bound(), .encode() and .decode()
     *
     * opaque is passed as last argument to snap_compress_handler_t.bound(),
     * snap_compress_handler_t.encode() and snap_compress_handler_t.decode().
     * This intended to ease implementing custom compression functions for
     * use with libsnap.
     */
    long opaque;
} snap_compress_handler_t;



/**
 * @struct snap_edm_handler_t
 * @brief Functions for error detection handling
 */
typedef struct
{
    uint8_t (*calculator)( const uint8_t *data, size_t size, long opaque );

    bool (*validator)( const uint8_t *data, size_t size, uint8_t checksum, long opaque );

   /**
     * @brief custom data passed to .calculator() and .validator()
     *
     * opaque is passed as last argument to snap_edm_handler_t.calculator(),
     * and snap_edm_handler_t.validator(). This intended to ease implementing
     * custom error detection functions for use with libsnap.
     */
    long opaque;
} snap_edm_handler_t;



#ifdef __cplusplus
}   // end extern "C"
#endif


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
