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
 * @file libsnap/tests/snap_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap packet unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


void snap_test_init_and_reset()
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( !snap_get_acknowledgement( &snap ) );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_local_address( &snap ) == 0 );
    SNAP_ASSERT( snap_get_peer_address( &snap ) == 0 );
    SNAP_ASSERT( snap_get_metadata_size( &snap ) == 3 );
    SNAP_ASSERT( snap_is_ready( &snap ) );
    SNAP_ASSERT( snap.last_decode_error_pos == 0 );
    SNAP_ASSERT( snap.packet.bound == 0 );
    SNAP_ASSERT( snap.packet.edm == EDM_NONE );
    SNAP_ASSERT( snap.packet.local_address == 0 );
    SNAP_ASSERT( snap.packet.peer_address == 0 );
    SNAP_ASSERT( snap.packet.size == 0 );

    snap.ack = true;
    snap.edm = EDM_CRC8;
    snap.local_address = 323;
    snap.peer_address = 94323;
    snap.last_decode_error_pos = 43;
    snap.packet.bound = 324;
    snap.packet.edm = EDM_CRC16;
    snap.packet.local_address = 4321;
    snap.packet.peer_address = 4;
    snap.packet.size = 1;

    SNAP_ASSERT( snap_get_acknowledgement( &snap ) );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) != EDM_NONE );
    SNAP_ASSERT( snap_get_local_address( &snap ) != 0 );
    SNAP_ASSERT( snap_get_peer_address( &snap ) != 0 );
    SNAP_ASSERT( snap_get_metadata_size( &snap ) != 3 );
    SNAP_ASSERT( !snap_is_ready( &snap ) );
    SNAP_ASSERT( snap.last_decode_error_pos != 0 );
    SNAP_ASSERT( snap.packet.bound != 0 );
    SNAP_ASSERT( snap.packet.edm != EDM_NONE );
    SNAP_ASSERT( snap.packet.local_address != 0 );
    SNAP_ASSERT( snap.packet.peer_address != 0 );
    SNAP_ASSERT( snap.packet.size != 0 );

    snap_reset( &snap );
    SNAP_ASSERT( !snap_get_acknowledgement( &snap ) );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_local_address( &snap ) == 0 );
    SNAP_ASSERT( snap_get_peer_address( &snap ) == 0 );
    SNAP_ASSERT( snap_get_metadata_size( &snap ) == 3 );
    SNAP_ASSERT( snap_is_ready( &snap ) );
    SNAP_ASSERT( snap.last_decode_error_pos == 0 );
    SNAP_ASSERT( snap.packet.bound == 0 );
    SNAP_ASSERT( snap.packet.edm == EDM_NONE );
    SNAP_ASSERT( snap.packet.local_address == 0 );
    SNAP_ASSERT( snap.packet.peer_address == 0 );
    SNAP_ASSERT( snap.packet.size == 0 );
}


void snap_test_statistic()
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( snap_get_statistic( &snap ).rejected_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).transmitted_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).received_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).total_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).rejected_bytes == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).transmitted_bytes == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).received_bytes == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).total_bytes == 0 );

    snap.stat.rejected_packets = 1;
    snap.stat.transmitted_packets = 5;
    snap.stat.received_packets = 4;
    snap.stat.total_packets = 2;
    snap.stat.rejected_bytes = 3;
    snap.stat.transmitted_bytes = 6;
    snap.stat.received_bytes = 8;
    snap.stat.total_bytes = 7;

    SNAP_ASSERT( snap_get_statistic( &snap ).rejected_packets == 1 );
    SNAP_ASSERT( snap_get_statistic( &snap ).transmitted_packets == 5 );
    SNAP_ASSERT( snap_get_statistic( &snap ).received_packets == 4 );
    SNAP_ASSERT( snap_get_statistic( &snap ).total_packets == 2 );
    SNAP_ASSERT( snap_get_statistic( &snap ).rejected_bytes == 3 );
    SNAP_ASSERT( snap_get_statistic( &snap ).transmitted_bytes == 6 );
    SNAP_ASSERT( snap_get_statistic( &snap ).received_bytes == 8 );
    SNAP_ASSERT( snap_get_statistic( &snap ).total_bytes == 7 );

    snap_clear_statistic( &snap );

    SNAP_ASSERT( snap_get_statistic( &snap ).rejected_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).transmitted_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).received_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).total_packets == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).rejected_bytes == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).transmitted_bytes == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).received_bytes == 0 );
    SNAP_ASSERT( snap_get_statistic( &snap ).total_bytes == 0 );
}


void snap_test_get_reset_state()
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( snap_is_ready( &snap ) );

    snap.last_error = SNAP_ERROR_BOUND;
    SNAP_ASSERT( !snap_is_ready( &snap ) );

    snap.packet.size = 1;
    SNAP_ASSERT( !snap_is_ready( &snap ) );

    snap.last_error = SNAP_ERROR_NONE;
    SNAP_ASSERT( !snap_is_ready( &snap ) );

    snap_clear( &snap );
    SNAP_ASSERT( snap_is_ready( &snap ) );

    snap.packet.size = 1;
    SNAP_ASSERT( !snap_is_ready( &snap ) );

    snap_clear( &snap );
    SNAP_ASSERT( snap_is_ready( &snap ) );
}


void snap_test_get_last_error()
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( snap_get_last_error( &snap ) == SNAP_ERROR_NONE );

    snap.last_error = SNAP_ERROR_UNKNOWN;
    SNAP_ASSERT( snap_get_last_error( &snap ) == SNAP_ERROR_UNKNOWN );

    snap_reset( &snap );
    SNAP_ASSERT( snap_get_last_error( &snap ) == SNAP_ERROR_NONE );
}


void snap_test_size()
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( snap_get_metadata_size( &snap ) == 3 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );
    SNAP_ASSERT( snap_get_local_address_size( &snap ) == 0 );
    SNAP_ASSERT( snap_get_peer_address_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_CRC16 );
    SNAP_ASSERT( snap_get_metadata_size( &snap ) == 3 + 2 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 2 );
    SNAP_ASSERT( snap_get_local_address_size( &snap ) == 0 );
    SNAP_ASSERT( snap_get_peer_address_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_CRC32 );
    snap_set_local_address( &snap, 100 );

    SNAP_ASSERT( snap_get_metadata_size( &snap ) == 3 + 4 + 1);
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 4 );
    SNAP_ASSERT( snap_get_local_address_size( &snap ) == 1 );
    SNAP_ASSERT( snap_get_peer_address_size( &snap ) == 0 );

    snap_reset( &snap );
    SNAP_ASSERT( snap_get_metadata_size( &snap ) == 3 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );
    SNAP_ASSERT( snap_get_local_address_size( &snap ) == 0 );
    SNAP_ASSERT( snap_get_peer_address_size( &snap ) == 0 );
}


void snap_test_acknowledgement()
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( !snap_get_acknowledgement( &snap ) );
    snap_set_acknowledgement( &snap, true );
    SNAP_ASSERT( snap_get_acknowledgement( &snap ) );

    snap_reset( &snap );
    SNAP_ASSERT( !snap_get_acknowledgement( &snap ) );
}


void snap_test_error_detection_mode()
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_THREETIMES );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_THREETIMES );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_CHECKSUM );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_CHECKSUM );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 1 );

    snap_set_error_detection_mode( &snap, EDM_CRC8 );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_CRC8 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 1 );

    snap_set_error_detection_mode( &snap, EDM_CRC16 );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_CRC16 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 2 );

    snap_set_error_detection_mode( &snap, EDM_CRC32 );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_CRC32 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 4 );

    snap_set_error_detection_mode( &snap, EDM_FEC );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_FEC );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_USER );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_USER );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_CRC8 );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_CRC8 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 1 );

    snap_set_error_detection_mode( &snap, EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    snap_set_error_detection_mode( &snap, EDM_CRC32 );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_CRC32 );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 4 );

    snap_reset( &snap );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    // illegal value
    snap.edm = 900;
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    snap_reset( &snap );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );

    // illegal value
    snap_set_error_detection_mode( &snap, 900 );
    SNAP_ASSERT( snap_get_error_detection_mode( &snap ) == EDM_NONE );
    SNAP_ASSERT( snap_get_error_detection_mode_size( &snap ) == 0 );
}


void priv_test_address( uint32_t (*getAddress)(snap_t *ptr), void (*setAddress)(snap_t *ptr, uint32_t value), size_t (*getAddressSize)(snap_t *ptr))
{
    snap_t snap;
    snap_init( &snap );

    SNAP_ASSERT( getAddress( &snap ) == 0 );
    SNAP_ASSERT( getAddressSize( &snap ) == 0 );

    size_t v[][2] = {
        { 0, 0 },
        { 1, 1 },
        { 127, 1 },
        { 128, 1 },
        { 255, 1 },
        { 256, 2 },
        { 32767, 2 },
        { 32768, 2 },
        { 65535, 2 },
        { 65536, 3 },
        { 8388607, 3 },
        { 8388608, 3 },
        { 16777215, 3 }
    };

    for(size_t i = 0; i < sizeof( v ) / sizeof( v[0] ); ++i) {
        setAddress( &snap, v[ i ][ 0 ] );
        SNAP_ASSERT( getAddress( &snap ) == (uint32_t) v[ i ][ 0 ] );
        SNAP_ASSERT( getAddressSize( &snap ) == v[ i ][ 1 ] );

    }

    snap_reset( &snap );
    SNAP_ASSERT( getAddress( &snap ) == 0 );
    SNAP_ASSERT( getAddressSize( &snap ) == 0 );

}

void snap_test_address()
{
    priv_test_address( snap_get_local_address, snap_set_local_address, snap_get_local_address_size );
    priv_test_address( snap_get_peer_address, snap_set_peer_address, snap_get_peer_address_size );
}



char test_suite_name[] = "snap";

void test_suite(CU_pSuite suite)
{
    CU_add_test( suite, "init/reset", snap_test_init_and_reset );
    CU_add_test( suite, "state", snap_test_get_reset_state );
    CU_add_test( suite, "statistic", snap_test_statistic );
    CU_add_test( suite, "last error", snap_test_get_last_error );
    CU_add_test( suite, "acknowledgement", snap_test_acknowledgement );
    CU_add_test( suite, "error detection mode", snap_test_error_detection_mode );
    CU_add_test( suite, "local and peer address", snap_test_address );
    CU_add_test( suite, "size", snap_test_size );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
