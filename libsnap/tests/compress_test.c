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
#include <CUnit/CUnit.h>

#include <libsnap/compress.h>

/**
 * @file libsnap/tests/compress_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap compress unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


void snap_test_lzma_bound()
{
    if( snap_lzma_builtin() ) {
        //TODO
    }
    else {
        CU_ASSERT( snap_lzma_bound( 0 ) == 0 );
        CU_ASSERT( snap_lzma_bound( 1 ) == 1 );
        CU_ASSERT( snap_lzma_bound( 1024) == 1024 );
        CU_ASSERT( snap_lzma_bound( 16000 ) == 16000 );
    }
}


void snap_test_lzma_encode()
{
    if( snap_lzma_builtin() ) {
            //TODO
    }
    else {
        CU_ASSERT( snap_lzma_encode( 0, 0, 0, 0, 0 ) == false );
    }
}


void snap_test_lzma_decode()
{
    if( snap_lzma_builtin() ) {
            //TODO
    }
    else {
        CU_ASSERT( snap_lzma_decode( 0, 0, 0, 0, 0 ) == false );
    }
}


void snap_test_zlib_bound()
{
    if( snap_zlib_builtin() ) {
        //TODO
    }
    else {
        CU_ASSERT( snap_zlib_bound( 0 ) == 0 );
        CU_ASSERT( snap_zlib_bound( 1 ) == 1 );
        CU_ASSERT( snap_zlib_bound( 1024) == 1024 );
        CU_ASSERT( snap_zlib_bound( 16000 ) == 16000 );
    }
}


void snap_test_zlib_encode()
{
    if( snap_zlib_builtin() ) {
        //TODO
    }
    else {
        CU_ASSERT( snap_zlib_encode( 0, 0, 0, 0, 0 ) == false );
    }
}


void snap_test_zlib_decode()
{
    if( snap_zlib_builtin() ) {
        //TODO
    }
    else {
        CU_ASSERT( snap_zlib_decode( 0, 0, 0, 0, 0 ) == false );
    }
}


char test_suite_name[] = "Compress";

void test_suite(CU_pSuite suite)
{
    (void) suite; // avoid compiler warning

    CU_add_test( suite, "test LZMA bound", snap_test_lzma_bound );
    CU_add_test( suite, "test LZMA encode", snap_test_lzma_encode );
    CU_add_test( suite, "test LZMA decode", snap_test_lzma_decode );

    CU_add_test( suite, "test ZLIB bound", snap_test_zlib_bound );
    CU_add_test( suite, "test ZLIB encode", snap_test_zlib_encode );
    CU_add_test( suite, "test ZLIB decode", snap_test_zlib_decode );
}




// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
