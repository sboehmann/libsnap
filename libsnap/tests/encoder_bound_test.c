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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <CUnit/Basic.h>

#include <libsnap/snap.h>
#include <libsnap/util.h>
#include <libsnap/edm.h>

/**
 * @file libsnap/tests/encoder_bound_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap packet unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


void snap_test_encode_bound_imp(size_t i, size_t ex)
{
    snap_t snap;
    snap_init( &snap );

    size_t out_size = 0;

    out_size= snap_encode_bound( &snap, i );
    SNAP_ASSERT( out_size == 3 + ex );

    snap_set_error_detection_mode( &snap, EDM_CHECKSUM );
    out_size = snap_encode_bound( &snap, i );
    SNAP_ASSERT( out_size == 3 + ex + 1 );

    snap_set_error_detection_mode( &snap, EDM_THREETIMES );
    out_size = snap_encode_bound( &snap, i );
    SNAP_ASSERT( out_size == ( 3 + ex ) * 3 );

    snap_set_error_detection_mode( &snap, EDM_CRC8 );
    out_size = snap_encode_bound( &snap, i );
    SNAP_ASSERT( out_size == 3 + ex + 1 );


    snap_set_error_detection_mode( &snap, EDM_CRC16 );
    out_size = snap_encode_bound( &snap, i );
    SNAP_ASSERT( out_size == 3 + ex + 2 );

    snap_set_error_detection_mode( &snap, EDM_CRC32 );
    out_size = snap_encode_bound( &snap, i );
    SNAP_ASSERT( out_size == 3 + ex + 4 );
}


void snap_test_encode_bound_8()
{
    for(size_t i = 0; i <= 8; ++i) {
        snap_test_encode_bound_imp( i, i );
    }
}


void snap_test_encode_bound_16()
{
    for(size_t i = 9; i <= 16; ++i) {
        snap_test_encode_bound_imp( i, 16 );
    }
}


void snap_test_encode_bound_32()
{
    for(size_t i = 17; i <= 32; ++i) {
        snap_test_encode_bound_imp( i, 32 );
    }
}


void snap_test_encode_bound_64()
{
    for(size_t i = 33; i <= 64; ++i) {
        snap_test_encode_bound_imp( i, 64 );
    }
}


void snap_test_encode_bound_128()
{
    for(size_t i = 65; i <= 128; ++i) {
        snap_test_encode_bound_imp( i, 128 );
    }
}


void snap_test_encode_bound_256()
{
    for(size_t i = 129; i <= 256; ++i) {
        snap_test_encode_bound_imp( i, 256 );
    }
}


void snap_test_encode_bound_512()
{
    for(size_t i = 257; i <= 512; ++i) {
        snap_test_encode_bound_imp( i, 512 );
    }
}


void snap_test_encode_bound_512_plus()
{
    snap_test_encode_bound_imp(  512, 512 );
    snap_test_encode_bound_imp(  513, 512 );
    snap_test_encode_bound_imp(  600, 512 );
    snap_test_encode_bound_imp( 1024, 512 );
    snap_test_encode_bound_imp( 2048, 512 );
}


char test_suite_name[] = "encoder bound";

void test_suite(CU_pSuite suite)
{
    CU_add_test( suite, "encoder bound 0 - 8", snap_test_encode_bound_8 );
    CU_add_test( suite, "encoder bound 9 - 16", snap_test_encode_bound_16 );
    CU_add_test( suite, "encoder bound 17 - 32", snap_test_encode_bound_32 );
    CU_add_test( suite, "encoder bound 33 - 64", snap_test_encode_bound_64 );
    CU_add_test( suite, "encoder bound 65 - 128", snap_test_encode_bound_128 );
    CU_add_test( suite, "encoder bound 129 - 256", snap_test_encode_bound_256 );
    CU_add_test( suite, "encoder bound 257 - 512", snap_test_encode_bound_512 );
    CU_add_test( suite, "encoder bound 512 - ...", snap_test_encode_bound_512_plus );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
