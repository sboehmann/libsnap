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
#include <libsnap/snap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/Basic.h>

/**
 * @file libsnap/tests/header_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


void priv_test_hdb1(int value, int result)
{
    // TODO Optional "CMD = Command mode bit" (Page 7)

    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result );

    snap_set_error_detection_mode( &snap, EDM_THREETIMES );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 16 );

    snap_set_error_detection_mode( &snap, EDM_CHECKSUM );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 32 );

    snap_set_error_detection_mode( &snap, EDM_CRC8 );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 48 );

    snap_set_error_detection_mode( &snap, EDM_CRC16 );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 64 );

    snap_set_error_detection_mode( &snap, EDM_CRC32 );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 80 );

    snap_set_error_detection_mode( &snap, EDM_FEC );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 96 );

    snap_set_error_detection_mode( &snap, EDM_USER );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 112 );

    snap_set_error_detection_mode( &snap, EDM_CRC16 );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result + 64 );

    snap_set_error_detection_mode( &snap, EDM_NONE );
    SNAP_ASSERT( snap_get_hdb1( &snap, value ) == result );
}


void snap_test_hdb1()
{
    unsigned int i = 0;
    for(; i <= 8; ++i) {
        priv_test_hdb1( i, i );
    }

    for(; i <= 16; ++i) {
        priv_test_hdb1( i, 9 );
    }

    for(; i <= 32; ++i) {
        priv_test_hdb1( i, 10 );
    }

    for(; i <= 64; ++i) {
        priv_test_hdb1( i, 11 );
    }

    for(; i <= 128; ++i) {
        priv_test_hdb1( i, 12 );
    }

    for(; i <= 256; ++i) {
        priv_test_hdb1( i, 13 );
    }

    for(; i <= 512; ++i) {
        priv_test_hdb1( i, 14 );
    }
}



void priv_test_hdb2_address(snap_t *snap, int i, int value, int address)
{
    {
        snap_set_local_address( snap, address );
        size_t size = snap_get_local_address_size( snap ) * 16;
        SNAP_ASSERT( snap_get_hdb2( snap, i ) == size + value );

        snap_set_local_address( snap, 0 );
        SNAP_ASSERT( snap_get_hdb2( snap, i ) == value );
    }

    {
        snap_set_peer_address( snap, address );
        size_t size = snap_get_peer_address_size( snap ) * 64;
        SNAP_ASSERT( snap_get_hdb2( snap, i ) == size + value );

        snap_set_peer_address( snap, 0 );
        SNAP_ASSERT( snap_get_hdb2( snap, i ) == value );
    }

    {
        snap_set_local_address( snap, address );
        snap_set_peer_address( snap, address );

        size_t size = ( snap_get_peer_address_size( snap ) * 64 ) + ( snap_get_local_address_size( snap ) * 16 );
        SNAP_ASSERT( snap_get_hdb2( snap, i ) == size + value );

        snap_set_local_address( snap, 0 );
        size = ( snap_get_peer_address_size( snap ) * 64 ) + ( snap_get_local_address_size( snap ) * 16 );
        SNAP_ASSERT( snap_get_hdb2( snap, i ) == size + value );

        snap_set_peer_address( snap, 0 );
        size = ( snap_get_peer_address_size( snap ) * 64 ) + ( snap_get_local_address_size( snap ) * 16 );
        SNAP_ASSERT( snap_get_hdb2( snap, i ) == size + value );

        SNAP_ASSERT( snap_get_hdb2( snap, i ) == value );
    }
}


void snap_test_hdb2()
{
    // TODO Optional "Number of Protocol specific Flag Bytes" (Page 7)

    snap_t snap;
    snap_init( &snap );

    for(int i = 0; i <= 512; ++i) {
        snap_reset( &snap );
        SNAP_ASSERT( snap_get_hdb2( &snap, i ) == 0 );

        snap_set_acknowledgement( &snap, true );
        SNAP_ASSERT( snap_get_hdb2( &snap, i ) == 1 );

        snap_set_acknowledgement( &snap, false );
        SNAP_ASSERT( snap_get_hdb2( &snap, i ) == 0 );

        priv_test_hdb2_address( &snap, i, 0, 0 );
        priv_test_hdb2_address( &snap, i, 0, 1 );
        priv_test_hdb2_address( &snap, i, 0, 127 );
        priv_test_hdb2_address( &snap, i, 0, 128 );
        priv_test_hdb2_address( &snap, i, 0, 255 );
        priv_test_hdb2_address( &snap, i, 0, 256 );
        priv_test_hdb2_address( &snap, i, 0, 32767 );
        priv_test_hdb2_address( &snap, i, 0, 32768 );
        priv_test_hdb2_address( &snap, i, 0, 65535 );
        priv_test_hdb2_address( &snap, i, 0, 65536 );
        priv_test_hdb2_address( &snap, i, 0, 8388607 );
        priv_test_hdb2_address( &snap, i, 0, 8388608 );
        priv_test_hdb2_address( &snap, i, 0, 16777215 );

        snap_set_acknowledgement( &snap, true );
        priv_test_hdb2_address( &snap, i, 1, 0 );
        priv_test_hdb2_address( &snap, i, 1, 1 );
        priv_test_hdb2_address( &snap, i, 1, 127 );
        priv_test_hdb2_address( &snap, i, 1, 128 );
        priv_test_hdb2_address( &snap, i, 1, 255 );
        priv_test_hdb2_address( &snap, i, 1, 256 );
        priv_test_hdb2_address( &snap, i, 1, 32767 );
        priv_test_hdb2_address( &snap, i, 1, 32768 );
        priv_test_hdb2_address( &snap, i, 1, 65535 );
        priv_test_hdb2_address( &snap, i, 1, 65536 );
        priv_test_hdb2_address( &snap, i, 1, 8388607 );
        priv_test_hdb2_address( &snap, i, 1, 8388608 );
        priv_test_hdb2_address( &snap, i, 1, 16777215 );

        snap_set_acknowledgement( &snap, false );
    }
}


void snap_test_header()
{
    snap_t snap;
    snap_init( &snap );

    for(int i = 0; i <= 512; ++i) {
        uint16_t value = snap_get_hdb2( &snap, i ) * ( (uint16_t) 256 );
        value += snap_get_hdb1( &snap, i );

        SNAP_ASSERT( value == snap_get_header( &snap, i ) );
    }
}


char test_suite_name[] = "header";

void test_suite(CU_pSuite suite)
{
    CU_add_test( suite, "header byte 1", snap_test_hdb1 );
    CU_add_test( suite, "header byte 2", snap_test_hdb2 );
    CU_add_test( suite, "snap header",   snap_test_header );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
