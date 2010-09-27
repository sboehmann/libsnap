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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/Basic.h>

#include <libsnap/snap.h>
#include <libsnap/util.h>

/**
 * @file libsnap/tests/decoder_response_bound_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap packet unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


void snap_test_decode_response_bound_imp(size_t size)
{
    snap_t snap;
    snap_init( &snap );

    size_t addresses[] = { 0, 1, 5, 128, 255, 256, 32434, 65535, 65536, 16777215 };
    const size_t addresses_size = sizeof( addresses ) / sizeof( addresses[ 0 ] );

    for(size_t i = 0; i < addresses_size; ++i) {
        for(size_t k = 0; k < addresses_size; ++k) {
            snap_reset( &snap );

            SNAP_ASSERT( snap_is_ready( &snap ) );

            snap_set_local_address( &snap, addresses[ i ] );
            snap_set_peer_address( &snap, addresses[ k ] );

            if( size < 3 ) {
                SNAP_ASSERT( snap_decode_response_bound( &snap, size ) == 0 );
            }
            else {
                // NOTICE
                // currently snap_util_ack_response_bound and snap_util_nak_response_bound both
                // returns 4 (3 bytes header + 1 byte checksum).

                const size_t expected = 4 + snap_util_get_address_size( addresses[ i ] )
                                          + snap_util_get_address_size( addresses[ k ] );

                SNAP_ASSERT( snap_decode_response_bound( &snap, size ) == expected );

                // with bound (e.g. snap waits for ack );
                size_t bound[] = {
                    3, 4, 5, 6, 7, 8, 9, 10, 15, 16, 17, 18, 19, 20,  31, 32,
                    33, 63, 64, 65, 127, 128, 129, 254, 255, 256, 300, 511, 512
                };

                for(size_t i = 0; i < sizeof( bound ) / sizeof( bound[ 0 ] ); ++i) {
                    snap.packet.size = 43;
                    SNAP_ASSERT( !snap_is_ready( &snap ) );

                    snap.packet.bound = bound[ i ];

                    SNAP_ASSERT( snap_decode_response_bound( &snap, size ) == bound[ i ] );
                }
            }
        }
    }
}



void snap_test_decode_response_bound_8()
{
    snap_test_decode_response_bound_imp( 0 );
    snap_test_decode_response_bound_imp( 1 );
    snap_test_decode_response_bound_imp( 2 );
    snap_test_decode_response_bound_imp( 3 );
    snap_test_decode_response_bound_imp( 4 );
    snap_test_decode_response_bound_imp( 5 );
    snap_test_decode_response_bound_imp( 6 );
    snap_test_decode_response_bound_imp( 7 );
    snap_test_decode_response_bound_imp( 8 );

}


void snap_test_decode_response_bound_16()
{
    for(size_t i = 9; i <= 16; ++i) {
        snap_test_decode_response_bound_imp( i );
    }
}


void snap_test_decode_response_bound_32()
{
    for(size_t i = 17; i <= 32; ++i) {
        snap_test_decode_response_bound_imp( i );
    }
}


void snap_test_decode_response_bound_64()
{
    for(size_t i = 33; i <= 64; ++i) {
        snap_test_decode_response_bound_imp( i );
    }
}


void snap_test_decode_response_bound_128()
{
    for(size_t i = 65; i <= 128; ++i) {
        snap_test_decode_response_bound_imp( i );
    }
}


void snap_test_decode_response_bound_256()
{
    for(size_t i = 129; i <= 256; ++i) {
        snap_test_decode_response_bound_imp( i );
    }
}


void snap_test_decode_response_bound_512()
{
    for(size_t i = 257; i <= 512; ++i) {
        snap_test_decode_response_bound_imp( i );
    }
}


void snap_test_decode_response_bound_512_plus()
{
    snap_test_decode_response_bound_imp(  512 );
    snap_test_decode_response_bound_imp(  513 );
    snap_test_decode_response_bound_imp(  600 );
    snap_test_decode_response_bound_imp( 1024 );
    snap_test_decode_response_bound_imp( 2048 );
}



char test_suite_name[] = "decoder response bound";

void test_suite(CU_pSuite suite)
{
    CU_add_test( suite, "decoder response bound 0 - 8", snap_test_decode_response_bound_8 );
    CU_add_test( suite, "decoder response bound 9 - 16", snap_test_decode_response_bound_16 );
    CU_add_test( suite, "decoder response bound 17 - 32", snap_test_decode_response_bound_32 );
    CU_add_test( suite, "decoder response bound 33 - 64", snap_test_decode_response_bound_64 );
    CU_add_test( suite, "decoder response bound 65 - 128", snap_test_decode_response_bound_128 );
    CU_add_test( suite, "decoder response bound 129 - 256", snap_test_decode_response_bound_256 );
    CU_add_test( suite, "decoder response bound 257 - 512", snap_test_decode_response_bound_512 );
    CU_add_test( suite, "decoder response bound 512 - ...", snap_test_decode_response_bound_512_plus );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
