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
#include <time.h>

#include <CUnit/Basic.h>

#include <libsnap/snap.h>
#include <libsnap/util.h>
#include <libsnap/edm.h>

/**
 * @file libsnap/tests/encoder_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap packet unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


void snap_test_empty_encode()
{
    snap_t snap;
    snap_init( &snap );

    uint8_t out[3] = { '1', '2', '3' };
    size_t pos = 0;

    SNAP_ASSERT( snap_encode( &snap, NULL, 0, out, &pos, 3 ) );
    SNAP_ASSERT( pos == 0 );
    SNAP_ASSERT( out[ 0 ] == '1' );
    SNAP_ASSERT( out[ 1 ] == '2' );
    SNAP_ASSERT( out[ 2 ] == '3' );

    SNAP_ASSERT( snap_encode( &snap, out, 0, out, &pos, 3 ) );
    SNAP_ASSERT( pos == 0 );
    SNAP_ASSERT( out[ 0 ] == '1' );
    SNAP_ASSERT( out[ 1 ] == '2' );
    SNAP_ASSERT( out[ 2 ] == '3' );

    SNAP_ASSERT( snap_encode( &snap, NULL, 50, out, &pos, 3 ) );
    SNAP_ASSERT( pos == 0 );
    SNAP_ASSERT( out[ 0 ] == '1' );
    SNAP_ASSERT( out[ 1 ] == '2' );
    SNAP_ASSERT( out[ 2 ] == '3' );
}


void snap_test_simpe_encode()
{
    snap_t snap;
    snap_init( &snap );

    {
        uint8_t in[] = { 'A', 'B', 'C' };
        uint8_t out[] = { '1', '2', '3', '4', '5', '6' };
        size_t pos = 0;

        SNAP_ASSERT( snap_encode( &snap, in, 3, out, &pos, 6 ) );
        SNAP_ASSERT( pos == 3 + 3 + 1 );
        SNAP_ASSERT( in[ 0 ] == 'A' );
        SNAP_ASSERT( in[ 1 ] == 'B' );
        SNAP_ASSERT( in[ 2 ] == 'C' );
        SNAP_ASSERT( out[ 0 ] == snap_util_get_sync_byte() );
        SNAP_ASSERT( out[ 1 ] == 0 );
        SNAP_ASSERT( out[ 2 ] == 3 );
        SNAP_ASSERT( out[ 3 ] == 'A' );
        SNAP_ASSERT( out[ 4 ] == 'B' );
        SNAP_ASSERT( out[ 5 ] == 'C' );
    }

    {
        snap_reset( &snap );
        snap_set_acknowledgement( &snap, true );

        uint8_t in[] = { 'A', 'B', 'C' };
        uint8_t out[] = { '1', '2', '3', '4', '5', '6' };
        size_t pos = 0;

        SNAP_ASSERT( snap_encode( &snap, in, 3, out, &pos, 6 ) );
        SNAP_ASSERT( pos == 3 + 3 + 1 );
        SNAP_ASSERT( in[ 0 ] == 'A' );
        SNAP_ASSERT( in[ 1 ] == 'B' );
        SNAP_ASSERT( in[ 2 ] == 'C' );
        SNAP_ASSERT( out[ 0 ] == snap_util_get_sync_byte() );
        SNAP_ASSERT( out[ 1 ] == 1 );
        SNAP_ASSERT( out[ 2 ] == 3 );
        SNAP_ASSERT( out[ 3 ] == 'A' );
        SNAP_ASSERT( out[ 4 ] == 'B' );
        SNAP_ASSERT( out[ 5 ] == 'C' );
    }

    {
        uint8_t in[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M' };
        size_t in_size = sizeof( in ) / sizeof( in[ 0 ] );

        for(size_t i = 1; i <= in_size; ++i) {
            snap_reset( &snap );

            size_t out_size = snap_encode_bound( &snap, i );
            uint8_t *out = malloc( out_size );
            size_t pos = 0;

            SNAP_ASSERT( snap_encode( &snap, in, i, out, &pos, out_size ) );

            // 3 header bytes + in_size + 1 byte offset (pos points to the next byte)
            // + fill bytes
            SNAP_ASSERT( pos == 3 + i + 1 + snap_util_get_ndb_fill_bytes( i ) );

            SNAP_ASSERT( in[ 0 ] == 'A' );
            SNAP_ASSERT( in[ 1 ] == 'B' );
            SNAP_ASSERT( in[ 2 ] == 'C' );
            SNAP_ASSERT( in[ 3 ] == 'D' );
            SNAP_ASSERT( in[ 4 ] == 'E' );
            SNAP_ASSERT( in[ 5 ] == 'F' );
            SNAP_ASSERT( in[ 6 ] == 'G' );
            SNAP_ASSERT( in[ 7 ] == 'H' );
            SNAP_ASSERT( in[ 8 ] == 'I' );
            SNAP_ASSERT( in[ 9 ] == 'J' );
            SNAP_ASSERT( in[ 10 ] == 'K' );

            SNAP_ASSERT( out[ 0 ] == snap_util_get_sync_byte() );
            SNAP_ASSERT( out[ 1 ] == 0 );
            SNAP_ASSERT( out[ 2 ] == snap_util_map_ndb_to_value( i ) );

            for(size_t k = 0; k < i; k++) {
                SNAP_ASSERT( out[ k + 3 ] == in[ k ] );
            }
        }
    }
}


void snap_test_priv_encode_example(uint32_t localAddress, uint32_t peerAddress, bool ack, SnapErrorDetectionMode edm, uint8_t *in, size_t in_size, uint8_t *expected, size_t expected_size)
{
    snap_t snap;
    snap_init( &snap );

    const size_t in_bytes = in_size <= 512 ? in_size : 512;

    snap_set_acknowledgement( &snap, ack );
    snap_set_local_address( &snap, localAddress );
    snap_set_peer_address( &snap, peerAddress );
    snap_set_error_detection_mode( &snap, edm );

    size_t out_size = snap_encode_bound( &snap, in_size );
    uint8_t *out = malloc( out_size );
    size_t pos = 0;

    SNAP_ASSERT( snap_encode( &snap, in, in_size, out, &pos, out_size ) );

    pos--; // pos points to the next __unwritten__ byte, therefor pos--

    const size_t header = 3;
    const size_t address_size = snap_util_get_address_size( localAddress ) + snap_util_get_address_size( peerAddress );
    SNAP_ASSERT( pos == ( edm == EDM_THREETIMES ? 3 : 1 )
                            * ( header + address_size + snap_util_get_ndb_total_bytes( in_size ) + snap_util_get_edm_size( edm ) )
               );

    SNAP_ASSERT( pos == expected_size );

    SNAP_ASSERT( out[ 0 ] == snap_util_get_sync_byte() );
    SNAP_ASSERT( out[ 0 ] == expected[ 0 ] );
    SNAP_ASSERT( out[ 1 ] == expected[ 1 ] );
    SNAP_ASSERT( out[ 2 ] == expected[ 2 ] );

    for(size_t i = 3; i < pos; ++i) {
        SNAP_ASSERT( out[ i ] == expected[ i ] );
    }

    snap_statistic_t stat = snap_get_statistic( &snap );
    SNAP_ASSERT( stat.rejected_packets == 0 );
    SNAP_ASSERT( stat.received_packets == 0 );
    SNAP_ASSERT( stat.total_packets == 1 );
    SNAP_ASSERT( stat.rejected_bytes == 0 );
    SNAP_ASSERT( stat.received_bytes == 0 );
    SNAP_ASSERT( stat.total_bytes == pos );
    SNAP_ASSERT( stat.total_payload_bytes == in_bytes );
    SNAP_ASSERT( stat.total_fill_bytes == snap_util_get_ndb_fill_bytes( in_size ) );

    if( ack ) {
        SNAP_ASSERT( !snap_is_ready( &snap ) );

        SNAP_ASSERT( snap.packet.bound == out_size );
        SNAP_ASSERT( snap.packet.edm == edm );
        SNAP_ASSERT( snap.packet.local_address == localAddress );
        SNAP_ASSERT( snap.packet.peer_address == peerAddress );
        SNAP_ASSERT( snap.packet.size == in_bytes );

        for(size_t i = 0; i < in_bytes; ++i) {
            SNAP_ASSERT( snap.packet.data[ i ] == in[ i ] );
        }

        SNAP_ASSERT( stat.transmitted_packets == 0 );
        SNAP_ASSERT( stat.transmitted_bytes == 0 );

        SNAP_ASSERT( !snap_encode( &snap, in, in_size, out, &pos, out_size ) );
        pos = 0;
        SNAP_ASSERT( !snap_encode( &snap, in, in_size, out, &pos, out_size ) );
    }
    else {
        SNAP_ASSERT( snap_is_ready( &snap ) );
        SNAP_ASSERT( snap.packet.bound == 0 );

        SNAP_ASSERT( stat.transmitted_packets == 1 );
        SNAP_ASSERT( stat.transmitted_bytes == pos );

        // this call fails because &pos is too big
        SNAP_ASSERT( !snap_encode( &snap, in, in_size, out, &pos, out_size ) );

        // because of the last failed snap_encode call the encoder isn't ready anymore
        SNAP_ASSERT( !snap_is_ready( &snap ) );

        // therefor even a 'correct' call must fail
        pos = 0;
        SNAP_ASSERT( !snap_encode( &snap, in, in_size, out, &pos, out_size ) );

        // but not if we reset the error state
        snap_clear( &snap );
        SNAP_ASSERT( snap_encode( &snap, in, in_size, out, &pos, out_size ) );
    }
}


void snap_test_encode_example_1()
{
    // Tests with data from the S.N.A.P. specification (Document revision 1.03)
    // Appendix A - Example 1 (Page 20)

    // In the example below the transmitting node has address 00000001 and
    // is sending data 11111111 to node 00000010. Since no acknowledge is
    // required the transmitting node will not expect any ACK or NAK packet
    // in return.

    uint32_t local = 1; // 1 Byte
    uint32_t peer = 2; // 1 Byte
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_CRC16;

    uint8_t data[] = { 255 }; // 1 Byte Data
    size_t in_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected[] = { 0x54, 0x50, 0x41, 0x02, 0x01, 0xFF, 0x4E, 0xBB };
    size_t expected_size = sizeof( expected ) / sizeof( expected[ 0 ] );

    snap_test_priv_encode_example(local, peer, ack, edm, data, in_size, expected, expected_size);
}


void snap_test_encode_example_2()
{
    // Tests with data from the S.N.A.P. specification (Document revision 1.03)
    // Appendix A - Example 2 (Page 21)

    // In the example below the transmitting node has address 00000001 and is
    // sending data 11110000 to node 00000011. This time acknowledge is
    // requested and the transmitting node expect to receive an ACK or NAK
    // packet, else it should time-out and take proper action.

    uint32_t local = 1; // 1 Byte
    uint32_t peer = 3;  // 1 Byte
    bool ack = true;
    SnapErrorDetectionMode edm = EDM_CRC16;

    uint8_t data[] = { 0xF0 }; // 1 Byte Data
    size_t in_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected[] = { 0x54, 0x51, 0x41, 0x03, 0x01, 0xF0, 0x22, 0x35 };
    size_t expected_size = sizeof( expected ) / sizeof( expected[ 0 ] );

    snap_test_priv_encode_example(local, peer, ack, edm, data, in_size, expected, expected_size);
}


void snap_test_encode_example_3()
{
    // Tests with data from the S.N.A.P. specification (Document revision 1.03)
    // Appendix A - Example 3 (Page 22)

    // In the example below the transmitting node has address 00000001 and
    // is sending data 11110000 to node 00000011. This time to, acknowledge
    // is requested and the transmitting node expect to receive an ACK or NAK
    // packet, else it should time-out and take proper action. Further more
    // one byte of protocol specific flags are used, these flags are set to
    // 00000011

    // TODO Protocol specific flags are optional and not implemented yet.
}


void snap_test_encode_example_4_helper(bool ack, uint32_t localAddress, uint32_t peerAddress, SnapErrorDetectionMode edm, uint8_t *in, size_t in_size)
{
    // example 4 helper function

    size_t expected_size = 1 + 2
                + snap_util_get_address_size( localAddress )
                + snap_util_get_address_size( peerAddress )
                + snap_util_get_ndb_total_bytes( in_size )
                + snap_util_get_edm_size( edm );

    if( edm == EDM_THREETIMES ) {
        expected_size *= 3;
    }

    uint8_t *expected = malloc( expected_size );

    size_t index = 0;

    for(int i = 0; i < 3; ++i) {
        expected[ index++ ] = snap_util_get_sync_byte();
        expected[ index++ ] = snap_util_get_address_size( peerAddress ) * 64u
                                  + snap_util_get_address_size( localAddress ) * 16u
                                  + (uint8_t) ( ack ? 1 : 0 );

        expected[ index++ ] = snap_util_get_edm_size_value( edm )
                                  + snap_util_map_ndb_to_value( in_size );

        switch( snap_util_get_address_size( peerAddress ) ) {
            case 3:
                expected[ index++ ] = (uint8_t) ( peerAddress >> 16 );
            case 2:
                expected[ index++ ] = (uint8_t) ( peerAddress >> 8 );
            case 1:
                expected[ index++ ] = (uint8_t) peerAddress;
            default:
                break;
        }

        switch( snap_util_get_address_size( localAddress ) ) {
            case 3:
                expected[ index++ ] = (uint8_t) ( localAddress >> 16 );
            case 2:
                expected[ index++ ] = (uint8_t) ( localAddress >> 8 );
            case 1:
                expected[ index++ ] = (uint8_t) localAddress;
            default:
                break;
        }

        for(size_t i = 0; i < in_size && i < 512; i++) {
            expected[ index++ ] = in[ i ];
        }

        for(size_t i = 0; i < snap_util_get_ndb_fill_bytes( in_size ); i++) {
            expected[ index++ ] = snap_util_get_fill_byte();
        }

        switch( edm ) {
            case EDM_THREETIMES:
                break;

            case EDM_CHECKSUM:
                expected[ index ] = snap_checksum( &expected[ 1 ], index - 1 );
                index++;
                break;

            case EDM_CRC8:
                expected[ index ] = snap_crc8( &expected[ 1 ], index - 1 );
                index++;
                break;

            case EDM_CRC16:
                {
                    uint16_t value = snap_crc16( &expected[ 1 ], index - 1 );

                    expected[ index++ ] = (uint8_t) ( value >> 8 );
                    expected[ index++ ] = (uint8_t) value;
                }
                break;
            case EDM_CRC32:
                {
                    uint32_t value =  snap_crc32( &expected[ 1 ], index - 1 );

                    expected[ index++ ] = (uint8_t) ( value >> 24 );
                    expected[ index++ ] = (uint8_t) ( value >> 16 );
                    expected[ index++ ] = (uint8_t) ( value >> 8 );
                    expected[ index++ ] = (uint8_t) value;
                }
                break;

            case EDM_FEC:
            case EDM_USER:
                break;

            default:
                SNAP_ASSERT( edm == EDM_NONE );
                break;
        }

        if( edm != EDM_THREETIMES ) {
            break;
        }
    }

    snap_test_priv_encode_example( localAddress, peerAddress, ack, edm, in, in_size, expected, expected_size );

    free( expected );
}


void snap_test_encode_example_4()
{
    // All possible parameters and parameter combinations will be treated in this test case.

    srand( time( 0 ) );

    uint8_t data[ 512 ];

    for(int i = 0; i < 512; ++i) {
        data[ i ] = (uint8_t) rand() % 91 + 32;
    }

    // test data
    size_t addresses[] = { 0, 1, 5, 128, 255, 256, 32434, 65535, 65536, 16777215 };
    SnapErrorDetectionMode edm[] = { EDM_NONE, EDM_THREETIMES, EDM_CHECKSUM, EDM_CRC8, EDM_CRC16, EDM_CRC32, EDM_FEC, EDM_USER };
    size_t packet_size[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 16, 17, 18, 19, 20,  31, 32, 33, 63,
        64, 65, 127, 128, 129, 254, 255, 256, 300, 511, 512, 513, 1024, 1569
    };

    // for each test data...
    for(size_t s = 0; s < sizeof( packet_size ) / sizeof( packet_size[ 0 ] ); ++s) {
        const size_t addresses_size = sizeof( addresses ) / sizeof( addresses[ 0 ] );
        for(size_t i = 0; i < addresses_size; ++i) {
            for(size_t k = 0; k < addresses_size; ++k) {
                for(size_t e = 0; e < sizeof( edm ) / sizeof( edm[ 0 ] ); ++e) {
                    snap_test_encode_example_4_helper( false, addresses[ i ], addresses[ k ], edm[ e ], data, packet_size[ s ] );
                    snap_test_encode_example_4_helper( true,  addresses[ i ], addresses[ k ], edm[ e ], data, packet_size[ s ] );
                }
            }
        }
    }
}


void snap_test_encode_example_5()
{
    // Same data as in example/simple_encode.c and example/simple_decode.c

    uint32_t local = 0; // 0 Bytes
    uint32_t peer = 0;  // 0 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_NONE;

    uint8_t data[] = "If a packet hits a pocket on a socket on a port, "
        "and the bus is interrupted and the interrupt's not caught, "
        "then the socket packet pocket has an error to report.";

    size_t in_size = sizeof( data ) / sizeof( data[ 0 ] );
    in_size--; // remove implicit trailing '\0' (introduced through the string notation of data)

    uint8_t expected[] = {
        0x54, 0x00, 0x0D, 0x49, 0x66, 0x20, 0x61, 0x20, 0x70, 0x61, 0x63, 0x6B,
        0x65, 0x74, 0x20, 0x68, 0x69, 0x74, 0x73, 0x20, 0x61, 0x20, 0x70, 0x6F,
        0x63, 0x6B, 0x65, 0x74, 0x20, 0x6F, 0x6E, 0x20, 0x61, 0x20, 0x73, 0x6F,
        0x63, 0x6B, 0x65, 0x74, 0x20, 0x6F, 0x6E, 0x20, 0x61, 0x20, 0x70, 0x6F,
        0x72, 0x74, 0x2C, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x74, 0x68, 0x65, 0x20,
        0x62, 0x75, 0x73, 0x20, 0x69, 0x73, 0x20, 0x69, 0x6E, 0x74, 0x65, 0x72,
        0x72, 0x75, 0x70, 0x74, 0x65, 0x64, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x74,
        0x68, 0x65, 0x20, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x72, 0x75, 0x70, 0x74,
        0x27, 0x73, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x63, 0x61, 0x75, 0x67, 0x68,
        0x74, 0x2C, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x74, 0x68, 0x65, 0x20,
        0x73, 0x6F, 0x63, 0x6B, 0x65, 0x74, 0x20, 0x70, 0x61, 0x63, 0x6B, 0x65,
        0x74, 0x20, 0x70, 0x6F, 0x63, 0x6B, 0x65, 0x74, 0x20, 0x68, 0x61, 0x73,
        0x20, 0x61, 0x6E, 0x20, 0x65, 0x72, 0x72, 0x6F, 0x72, 0x20, 0x74, 0x6F,
        0x20, 0x72, 0x65, 0x70, 0x6F, 0x72, 0x74, 0x2E, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    size_t expected_size = sizeof( expected ) / sizeof( expected[ 0 ] );

    snap_test_priv_encode_example(local, peer, ack, edm, data, in_size, expected, expected_size);
}


void snap_test_encode_corner_cases()
{
    //TODO
    SNAP_ASSERT( true );
}


char test_suite_name[] = "encoder";

void test_suite(CU_pSuite suite)
{
    CU_add_test( suite, "encoder (empty test)", snap_test_empty_encode );
    CU_add_test( suite, "encoder (simple test)", snap_test_simpe_encode );
    CU_add_test( suite, "encode example 1", snap_test_encode_example_1 );
    CU_add_test( suite, "encode example 2", snap_test_encode_example_2 );
    CU_add_test( suite, "encode example 3", snap_test_encode_example_3 );
    CU_add_test( suite, "encode example 4", snap_test_encode_example_4 );
    CU_add_test( suite, "encode example 5", snap_test_encode_example_5 );
    CU_add_test( suite, "encode corner cases", snap_test_encode_corner_cases );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
