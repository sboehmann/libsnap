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

#include <CUnit/Basic.h>

#include <libsnap/snap.h>
#include <libsnap/util.h>

/**
 * @file libsnap/tests/decoder_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap packet unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


#define SNAP_HELPER_MACRO_1()                                           \
    snap_t snap;                                                        \
    snap_init( &snap );                                                 \
                                                                        \
    uint8_t data[] = { 'S', 'T', 0, 0, 'X', 'Y', 'Z' };                 \
    size_t real_data_size = sizeof( data ) / sizeof( data[ 0 ] );       \
    size_t data_size = real_data_size - 3;                              \
    size_t data_pos = 0;                                                \
                                                                        \
    /* suppress potential warnings */                                   \
    SNAP_UNUSED( data_size );                                           \
    SNAP_UNUSED( real_data_size );                                      \
                                                                        \
    uint8_t result[512];                                                \
    size_t result_pos = 0;                                              \
    size_t result_size = 512;                                           \
                                                                        \
    uint8_t response[1570];                                             \
    size_t response_pos = 0;                                            \
    size_t response_size = 1570



#define SNAP_HELPER_MACRO_1_DATA_INTEGRITY_CHECK()                      \
    SNAP_ASSERT( data[ 0 ] == 'S' );                                    \
    SNAP_ASSERT( data[ 1 ] == 'T' );                                    \
    SNAP_ASSERT( data[ 2 ] == 0 );                                      \
    SNAP_ASSERT( data[ 3 ] == 0 );                                      \
    SNAP_ASSERT( data[ 4 ] == 'X' );                                    \
    SNAP_ASSERT( data[ 5 ] == 'Y' );                                    \
    SNAP_ASSERT( data[ 6 ] == 'Z' );




void snap_test_priv_decode_example(uint32_t localAddress, uint32_t peerAddress,
             bool ack, SnapErrorDetectionMode edm, uint8_t *in, size_t in_size,
             size_t expected_in_pos, uint8_t *expected_result, 
             size_t expected_result_size, uint8_t *expected_response, 
             size_t expected_response_size)
{
    snap_t snap;
    snap_init( &snap );

    snap_set_acknowledgement( &snap, ack );
    snap_set_local_address( &snap, localAddress );
    snap_set_peer_address( &snap, peerAddress );
    snap_set_error_detection_mode( &snap, edm );

    size_t in_pos = 0;

    size_t result_size = snap_encode_bound( &snap, in_size );
    uint8_t *result = malloc( result_size );
    size_t result_pos = 0;

    size_t response_size = snap_encode_bound( &snap, in_size );
    uint8_t *response = malloc( response_size );
    size_t response_pos = 0;


    SNAP_ASSERT(
        snap_decode( &snap, in, &in_pos, in_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );

    SNAP_ASSERT( in_size == expected_in_pos );
    SNAP_ASSERT( expected_result_size == result_pos );
    SNAP_ASSERT( expected_response_size == response_pos );

    for(size_t i = 0; i < expected_result_size; ++i) {
        SNAP_ASSERT( result[ i ] == expected_result[ i ] );
    }

    for(size_t i = 0; i < expected_response_size; ++i) {
        SNAP_ASSERT( response[ i ] == expected_response[ i ] );
    }
}


void snap_test_decode_illegal_ndb()
{
    // with illegal ndb value

    SNAP_HELPER_MACRO_1();
    data[ 3 ] = 15; // replace NDB

    SNAP_ASSERT(
        !snap_decode( &snap, data, &data_pos, data_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == 1 + 1 + 2 + 1 );
    SNAP_ASSERT( snap.last_decode_error_pos == 4 );
    SNAP_ASSERT( snap.last_error == SNAP_ERROR_UNKNOWN_NDB_VALUE );
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == 0 );
}


void snap_test_decode_illegal_edm()
{
    // with illegal ndb value

    SNAP_HELPER_MACRO_1();
    data[ 3 ] = 112; // replace EDM

    SNAP_ASSERT(
        !snap_decode( &snap, data, &data_pos, data_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == 1 + 1 + 2 + 1 );
    SNAP_ASSERT( snap.last_decode_error_pos == 4 );
    SNAP_ASSERT( snap.last_error == SNAP_ERROR_UNKNOWN_EDM_TYPE );
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == 0 );
}


void snap_test_decode_invalid_edm()
{
    uint32_t local = 0;     // 0 Bytes
    uint32_t peer = 86542;  // 3 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_CRC16;

    uint8_t data[] = {
        0x54, 0xC0, 0x4A, 0x01, 0x52, 0x0E, 0x48, 0x65, 0x6C, 0x6C, 0x6F,
        0x20, 0x53, 0x4E, 0x41, 0x50, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
        0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0x61 /* a valid checksum is 0x60 instead of 0x61 */
    };
    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "Hello SNAP World!";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_t snap;
    snap_init( &snap );

    snap_set_acknowledgement( &snap, ack );
    snap_set_local_address( &snap, local );
    snap_set_peer_address( &snap, peer );
    snap_set_error_detection_mode( &snap, edm );

    size_t in_pos = 0;

    size_t result_size = snap_encode_bound( &snap, data_size );
    uint8_t *result = malloc( result_size );
    size_t result_pos = 0;

    size_t response_size = snap_encode_bound( &snap, data_size );
    uint8_t *response = malloc( response_size );
    size_t response_pos = 0;


    SNAP_ASSERT(
        !snap_decode( &snap, data, &in_pos, data_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );

    SNAP_ASSERT( in_pos == data_size );
    SNAP_ASSERT( snap.last_decode_error_pos == data_size - 2 ); // - 2 byte size of edm
    SNAP_ASSERT( snap.last_error == SNAP_ERROR_INVALID_EDM );
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == 0 );

    SNAP_ASSERT( !snap_is_ready( &snap ) );
    snap_reset( &snap );
    SNAP_ASSERT( snap_is_ready( &snap ) );

    in_pos = 0;
    data[ 1 ] = data[ 1 ] + 1;

    SNAP_ASSERT(
        !snap_decode( &snap, data, &in_pos, data_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );

    SNAP_ASSERT( in_pos == data_size );
    SNAP_ASSERT( snap.last_decode_error_pos == data_size - 2 ); // - 2 byte size of edm
    SNAP_ASSERT( snap.last_error == SNAP_ERROR_INVALID_EDM );
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == snap_util_nak_response_bound( local, peer ) );
    SNAP_ASSERT( response[ 0 ] == 'T' );

    SNAP_ASSERT( !snap_is_ready( &snap ) );

    snap_reset( &snap );
    SNAP_ASSERT( snap_is_ready( &snap ) );

    //TODO
    SNAP_UNUSED( expected_response_size );
    SNAP_UNUSED( expected_response );
}


void snap_test_decode_no_data()
{
    // no data (size 0)

    SNAP_HELPER_MACRO_1();

    SNAP_ASSERT(
        snap_decode( &snap, data, &data_pos, 0, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == 0 );
    SNAP_HELPER_MACRO_1_DATA_INTEGRITY_CHECK();
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == 0 );

    SNAP_ASSERT(
        snap_decode( &snap, data + 2, &data_pos, data_size - 2, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == data_size - 2 );
    SNAP_HELPER_MACRO_1_DATA_INTEGRITY_CHECK();
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == 0 );
}


void snap_test_decode_codswallop_1()
{
    // only random noise

    SNAP_HELPER_MACRO_1();
    data[ 1 ] = 'H'; // replace SYNC Byte

    SNAP_ASSERT(
        snap_decode( &snap, data, &data_pos, data_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == data_size );
    SNAP_ASSERT( data[ 0 ] == 'S' );
    SNAP_ASSERT( data[ 1 ] == 'H' );
    SNAP_ASSERT( data[ 2 ] == 0 );
    SNAP_ASSERT( data[ 3 ] == 0 );
    SNAP_ASSERT( data[ 4 ] == 'X' );
    SNAP_ASSERT( data[ 5 ] == 'Y' );
    SNAP_ASSERT( data[ 6 ] == 'Z' );
}


void snap_test_decode_codswallop_2()
{
    // only random noise

    SNAP_HELPER_MACRO_1();
    data[ 1 ] = 'H'; // replace SYNC Byte

    SNAP_ASSERT(
        snap_decode( &snap, data, &data_pos, real_data_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == real_data_size );
    SNAP_ASSERT( data[ 0 ] == 'S' );
    SNAP_ASSERT( data[ 1 ] == 'H' );
    SNAP_ASSERT( data[ 2 ] == 0 );
    SNAP_ASSERT( data[ 3 ] == 0 );
    SNAP_ASSERT( data[ 4 ] == 'X' );
    SNAP_ASSERT( data[ 5 ] == 'Y' );
    SNAP_ASSERT( data[ 6 ] == 'Z' );
}


void snap_test_decode_corner_case_sync_at_end()
{
    // corner case when SYNC is at the end of the stream.

    SNAP_HELPER_MACRO_1();

    data[ 1 ] = 'H'; // replace old SYNC byte
    data[ 6 ] = 'T'; // add SYNC byte add the end

    SNAP_ASSERT(
        snap_decode( &snap, data, &data_pos, real_data_size, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );

    SNAP_ASSERT( data_pos == real_data_size - 1 );
    SNAP_ASSERT( data[ data_pos ] == 'T' );
    SNAP_ASSERT( data[ 0 ] == 'S' );
    SNAP_ASSERT( data[ 1 ] == 'H' );
    SNAP_ASSERT( data[ 2 ] == 0 );
    SNAP_ASSERT( data[ 3 ] == 0 );
    SNAP_ASSERT( data[ 4 ] == 'X' );
    SNAP_ASSERT( data[ 5 ] == 'Y' );
    SNAP_ASSERT( data[ 6 ] == 'T' );
}


void snap_test_decode_too_small()
{
    // simple packet without leading unnecessary stuff (e.g preamble bytes)
    // but too small

    SNAP_HELPER_MACRO_1();

    SNAP_ASSERT(
        snap_decode( &snap, data + 1, &data_pos, data_size - 2, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == 0 );
    SNAP_HELPER_MACRO_1_DATA_INTEGRITY_CHECK();
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == 0 );
}


void snap_test_decode_too_small_with_preamble()
{
    // simple packet with leading unnecessary stuff (e.g preamble bytes)
    // but too small

    SNAP_HELPER_MACRO_1();

    SNAP_ASSERT(
        snap_decode( &snap, data, &data_pos, data_size - 2, result, &result_pos,
                          result_size, response, &response_pos, response_size )
    );
    SNAP_ASSERT( data_pos == 1 ); // snap_encode should skip the leading stuff
    SNAP_ASSERT( data[ data_pos ] == 'T' ); // and point to the SYNC byte
    SNAP_HELPER_MACRO_1_DATA_INTEGRITY_CHECK();
    SNAP_ASSERT( result_pos == 0 );
    SNAP_ASSERT( response_pos == 0 );
}


void snap_test_decode_example_1()
{
    uint32_t local = 0; // 0 Bytes
    uint32_t peer = 0;  // 0 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_NONE;

    uint8_t data[] = {
        0x54, 0x00, 0x0A, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x53, 0x4E,
        0x41, 0x50, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00
    };

    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "Hello SNAP World!";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


void snap_test_decode_example_2()
{
    uint32_t local = 123;   // 1 Bytes
    uint32_t peer = 43232;  // 2 Bytes
    bool ack = true;
    SnapErrorDetectionMode edm = EDM_NONE;

    uint8_t data[] = {
        0x54, 0x91, 0x0A, 0xA8, 0xEA, 0x7B, 0x48, 0x65, 0x6C, 0x6C, 0x6F,
        0x20, 0x53, 0x4E, 0x41, 0x50, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
        0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00
    };
    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "Hello SNAP World!";
    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { 0x54, 0x62, 0x20, 0x7B, 0xA8, 0xEA, 0xE3 }; // ACK packet
    size_t expected_response_size = 7;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


void snap_test_decode_example_3()
{
    uint32_t local = 0;     // 0 Bytes
    uint32_t peer = 86542;  // 3 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_CRC16;

    uint8_t data[] = {
        0x54, 0xC0, 0x4A, 0x01, 0x52, 0x0E, 0x48, 0x65, 0x6C, 0x6C, 0x6F,
        0x20, 0x53, 0x4E, 0x41, 0x50, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
        0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFA, 0x60
    };
    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "Hello SNAP World!";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


void snap_test_decode_example_4()
{
    // crc32 and no fill bytes

    uint32_t local = 0; // 0 Bytes
    uint32_t peer = 0;  // 0 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_CRC32;

    uint8_t data[] = {
        0x54, 0x00, 0x58, 0x53, 0x2E, 0x4E, 0x2E, 0x41,
        0x2E, 0x50, 0x2E, 0x52, 0x9E, 0xC3, 0xAC
    };
    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "S.N.A.P.";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


void snap_test_decode_example_5()
{
    // Same data as in example/simple_encode.c and example/simple_decode.c

    uint32_t local = 0; // 0 Bytes
    uint32_t peer = 0;  // 0 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_NONE;

    uint8_t data[] = {
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

    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "If a packet hits a pocket on a socket on a port, "
        "and the bus is interrupted and the interrupt's not caught, "
        "then the socket packet pocket has an error to report.";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );

    expected_result_size = 0;
    data[ 2 ] = data[ 2 ] + 1;
    edm = EDM_THREETIMES;
    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


void snap_test_decode_example_6()
{
    // as in example 5, but with 8 bit checksum

    uint32_t local = 0; // 0 Bytes
    uint32_t peer = 0;  // 0 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_CHECKSUM;

    uint8_t data[] = {
        0x54, 0x00, 0x2D, 0x49, 0x66, 0x20, 0x61, 0x20, 0x70, 0x61, 0x63, 0x6B,
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
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7
    };

    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "If a packet hits a pocket on a socket on a port, "
        "and the bus is interrupted and the interrupt's not caught, "
        "then the socket packet pocket has an error to report.";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


void snap_test_decode_example_7()
{
    // as in example 5, but with 8 bit crc

    uint32_t local = 0; // 0 Bytes
    uint32_t peer = 0;  // 0 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_CRC8;

    uint8_t data[] = {
        0x54, 0x00, 0x3D, 0x49, 0x66, 0x20, 0x61, 0x20, 0x70, 0x61, 0x63, 0x6B,
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
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8
    };

    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "If a packet hits a pocket on a socket on a port, "
        "and the bus is interrupted and the interrupt's not caught, "
        "then the socket packet pocket has an error to report.";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


void snap_test_decode_example_8()
{
    // full 3 times packet
    // TODO more test with this EDM type - here be dragons!

    uint32_t local = 0;     // 0 Bytes
    uint32_t peer = 86542;  // 3 Bytes
    bool ack = false;
    SnapErrorDetectionMode edm = EDM_THREETIMES;

    uint8_t data[] = {
        0x54, 0x00, 0x1A, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x53, 0x4E,
        0x41, 0x50, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x54, 0x00, 0x1A, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20,
        0x53, 0x4E, 0x41, 0x50, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x54, 0x00, 0x1A, 0x48, 0x65, 0x6C, 0x6C,
        0x6F, 0x20, 0x53, 0x4E, 0x41, 0x50, 0x20, 0x57, 0x6F, 0x72, 0x6C,
        0x64, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    size_t data_size = sizeof( data ) / sizeof( data[ 0 ] );

    uint8_t expected_result[] = "Hello SNAP World!";

    size_t expected_result_size = sizeof( expected_result ) / sizeof( expected_result[ 0 ] );

    // remove implicit trailing '\0' (introduced through the string notation of expected_result)
    expected_result_size--;

    uint8_t expected_response[] = { '\0' };
    size_t expected_response_size = 0;

    snap_test_priv_decode_example(
            local, peer, ack, edm, data, data_size, data_size, expected_result,
            expected_result_size, expected_response, expected_response_size
    );
}


char test_suite_name[] = "decoder";

void test_suite(CU_pSuite suite)
{
    //CU_add_test( suite, "", );

    CU_add_test( suite, "decode with illegal ndb value", snap_test_decode_illegal_ndb );
    CU_add_test( suite, "decode with illegal edm value", snap_test_decode_illegal_edm );
    CU_add_test( suite, "decode with invalid edm value", snap_test_decode_invalid_edm );
    CU_add_test( suite, "decode no data", snap_test_decode_no_data );
    CU_add_test( suite, "decode codswallop 1", snap_test_decode_codswallop_1 );
    CU_add_test( suite, "decode codswallop 2", snap_test_decode_codswallop_2 );
    CU_add_test( suite, "decode corner case when SYNC is at the end of the stream", snap_test_decode_corner_case_sync_at_end );
    CU_add_test( suite, "decode too small data", snap_test_decode_too_small );
    CU_add_test( suite, "decode too small with preamble", snap_test_decode_too_small_with_preamble );

    CU_add_test( suite, "decode example 1", snap_test_decode_example_1 );
    CU_add_test( suite, "decode example 2", snap_test_decode_example_2 );
    CU_add_test( suite, "decode example 3", snap_test_decode_example_3 );
    CU_add_test( suite, "decode example 4", snap_test_decode_example_4 );
    CU_add_test( suite, "decode example 5", snap_test_decode_example_5 );
    CU_add_test( suite, "decode example 6", snap_test_decode_example_6 );
    CU_add_test( suite, "decode example 7", snap_test_decode_example_7 );
    CU_add_test( suite, "decode example 8", snap_test_decode_example_8 );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
