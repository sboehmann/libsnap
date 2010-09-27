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
#include <libsnap/util.h>
#include <libsnap/edm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <CUnit/Basic.h>

/**
 * @file libsnap/tests/utils_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap packet unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


#define POWEROF2(x) ( (1ull) << (x) )


void* priv_snap_utils_test_allocator_alloc(size_t size, long opaque)
{
    SNAP_ASSERT( size == 23 );
    SNAP_ASSERT( opaque == 42 );

    return NULL;
}


void priv_snap_utils_test_allocator_free(void *ptr, long opaque)
{
    SNAP_ASSERT( ptr == NULL );
    SNAP_ASSERT( opaque == 23 );
}


void snap_utils_test_alloc()
{
    {
        // This test is nothing to write home about. But no crash (SegFault) is
        // also a realization and Valgrind can also say something about it.
        char *c = snap_util_alloc( NULL, 3 );
        c[ 0 ] = 'A';
        c[ 1 ] = 'B';
        c[ 2 ] = 'C';
        snap_util_free( NULL, c );
    }

    snap_allocator_t a;
    a.opaque = 42;
    a.alloc = priv_snap_utils_test_allocator_alloc;

    SNAP_ASSERT( snap_util_alloc( &a, 23 ) == NULL );

    a.free = priv_snap_utils_test_allocator_free;
    SNAP_ASSERT( snap_util_alloc( &a, 23 ) == NULL );
}


void snap_utils_test_free()
{
    snap_allocator_t a;
    a.opaque = 23;
    a.free = priv_snap_utils_test_allocator_free;

    snap_util_free( &a, NULL );

    a.alloc = priv_snap_utils_test_allocator_alloc;
    snap_util_free( &a, NULL );
}


void snap_utils_test_test_bit()
{
    for( uint8_t i = 0; i < 8; ++i ) {
        SNAP_ASSERT( snap_util_test_bit( 0xAA, i ) == ( i % 2 == 1 ) );
        SNAP_ASSERT( snap_util_test_bit16( 0xAA, i ) == ( i % 2 == 1 ) );
        SNAP_ASSERT( snap_util_test_bit32( 0xAA, i ) == ( i % 2 == 1 ) );
        SNAP_ASSERT( snap_util_test_bit64( 0xAA, i ) == ( i % 2 == 1 ) );
    }

    for( uint8_t i = 0; i < 16; ++i ) {
        SNAP_ASSERT( snap_util_test_bit16( 0xAAAA, i ) == ( i % 2 == 1 ) );
        SNAP_ASSERT( snap_util_test_bit32( 0xAAAA, i ) == ( i % 2 == 1 ) );
        SNAP_ASSERT( snap_util_test_bit64( 0xAAAA, i ) == ( i % 2 == 1 ) );
    }

    for( uint8_t i = 0; i < 32; ++i ) {
        SNAP_ASSERT( snap_util_test_bit32( 0xAAAAAAAA, i ) == ( i % 2 == 1 ) );
        SNAP_ASSERT( snap_util_test_bit64( 0xAAAAAAAA, i ) == ( i % 2 == 1 ) );
    }

    for( uint8_t i = 0; i < 64; ++i ) {
        SNAP_ASSERT( snap_util_test_bit64( 0xAAAAAAAAAAAAAAAA, i ) == ( i % 2 == 1 ) );
    }
}


void snap_utils_test_set_bit()
{
    for( uint8_t i = 0; i < 8; ++i) {
        SNAP_ASSERT( snap_util_set_bit( 0, i ) == 1 << i );
        SNAP_ASSERT( snap_util_set_bit16( 0, i ) == 1 << i );
        SNAP_ASSERT( snap_util_set_bit32( 0, i ) == 1u << i );
        SNAP_ASSERT( snap_util_set_bit64( 0, i ) == 1ull << i );
    }

    for( uint8_t i = 0; i < 16; ++i) {
        SNAP_ASSERT( snap_util_set_bit16( 0, i ) == 1 << i );
        SNAP_ASSERT( snap_util_set_bit32( 0, i ) == 1u << i );
        SNAP_ASSERT( snap_util_set_bit64( 0, i ) == 1ull << i );
    }

    for( uint8_t i = 0; i < 32; ++i) {
        SNAP_ASSERT( snap_util_set_bit32( 0, i ) == 1u << i );
        SNAP_ASSERT( snap_util_set_bit64( 0, i ) == 1ull << i );
    }

    for( uint8_t i = 0; i < 64; ++i) {
        SNAP_ASSERT( snap_util_set_bit64( 0, i ) == 1ull << i );
    }
}


void snap_utils_test_clear_bit()
{
    for( uint8_t i = 0; i < 8; ++i) {
        SNAP_ASSERT( snap_util_clear_bit( 0xFF, i ) == 0xFF - POWEROF2( i ) );
        SNAP_ASSERT( snap_util_clear_bit16( 0xFFFF, i ) == 0xFFFF - POWEROF2( i ) );
        SNAP_ASSERT( snap_util_clear_bit32( 0xFFFFFFFF, i ) == 0xFFFFFFFF - POWEROF2( i ) );
        SNAP_ASSERT( snap_util_clear_bit64( 0xFFFFFFFFFFFFFFFF, i ) == 0xFFFFFFFFFFFFFFFF - POWEROF2( i ) );
    }

    for( uint8_t i = 0; i < 16; ++i) {
        SNAP_ASSERT( snap_util_clear_bit16( 0xFFFF, i ) == 0xFFFF - POWEROF2( i ) );
        SNAP_ASSERT( snap_util_clear_bit32( 0xFFFFFFFF, i ) == 0xFFFFFFFF - POWEROF2( i ) );
        SNAP_ASSERT( snap_util_clear_bit64( 0xFFFFFFFFFFFFFFFF, i ) == 0xFFFFFFFFFFFFFFFF - POWEROF2( i ) );
    }

    for( uint8_t i = 0; i < 32; ++i) {
        SNAP_ASSERT( snap_util_clear_bit32( 0xFFFFFFFF, i ) == 0xFFFFFFFF - POWEROF2( i ) );
        SNAP_ASSERT( snap_util_clear_bit64( 0xFFFFFFFFFFFFFFFF, i ) == 0xFFFFFFFFFFFFFFFF - POWEROF2( i ) );
    }

    for( uint8_t i = 0; i < 64; ++i) {
        SNAP_ASSERT( snap_util_clear_bit64( 0xFFFFFFFFFFFFFFFF, i ) == 0xFFFFFFFFFFFFFFFF - POWEROF2( i ) );
    }
}


void snap_utils_test_memcpy()
{
    uint8_t b1[] = { 'A', 'B', 'C' };
    uint8_t b2[] = { '0', '1', '2' };

    snap_util_memcpy( &b2, &b1, 0 );

    SNAP_ASSERT( b1[ 0 ] == 'A' );
    SNAP_ASSERT( b1[ 1 ] == 'B' );
    SNAP_ASSERT( b1[ 2 ] == 'C' );
    SNAP_ASSERT( b2[ 0 ] == '0' );
    SNAP_ASSERT( b2[ 1 ] == '1' );
    SNAP_ASSERT( b2[ 2 ] == '2' );

    snap_util_memcpy( &b2, &b1, 3 );

    SNAP_ASSERT( b1[ 0 ] == 'A' );
    SNAP_ASSERT( b1[ 1 ] == 'B' );
    SNAP_ASSERT( b1[ 2 ] == 'C' );
    SNAP_ASSERT( b2[ 0 ] == 'A' );
    SNAP_ASSERT( b2[ 1 ] == 'B' );
    SNAP_ASSERT( b2[ 2 ] == 'C' );

    //TODO (more tests)
}


void snap_utils_test_get_ndb_total_bytes()
{
    unsigned int i = 0;
    for(; i <= 8; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_total_bytes( i ) == i );
    }

    for(; i <= 16; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_total_bytes( i ) == 16 );
    }

    for(; i <= 32; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_total_bytes( i ) == 32 );
    }

    for(; i <= 64; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_total_bytes( i ) == 64 );
    }

    for(; i <= 128; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_total_bytes( i ) == 128 );
    }

    for(; i <= 256; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_total_bytes( i ) == 256 );
    }

    for(; i <= 512; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_total_bytes( i ) == 512 );
    }
}


void snap_utils_test_get_ndb_fill_bytes()
{
    unsigned int i = 0;
    for(; i <= 8; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_fill_bytes( i ) == 0 );
    }

    for(; i <= 16; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_fill_bytes( i ) == 16 - i );
    }

    for(; i <= 32; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_fill_bytes( i ) == 32 - i );
    }

    for(; i <= 64; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_fill_bytes( i ) == 64 - i );
    }

    for(; i <= 128; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_fill_bytes( i ) == 128 - i );
    }

    for(; i <= 256; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_fill_bytes( i ) == 256 - i );
    }

    for(; i <= 512; ++i) {
        SNAP_ASSERT( snap_util_get_ndb_fill_bytes( i ) == 512 - i );
    }
}


void snap_utils_test_map_ndb_to_value()
{
    unsigned int i = 0;
    for(; i <= 8; ++i) {
        SNAP_ASSERT( snap_util_map_ndb_to_value( i ) == i );
    }

    for(; i <= 16; ++i) {
        SNAP_ASSERT( snap_util_map_ndb_to_value( i ) == 9 );
    }

    for(; i <= 32; ++i) {
        SNAP_ASSERT( snap_util_map_ndb_to_value( i ) == 10 );
    }

    for(; i <= 64; ++i) {
        SNAP_ASSERT( snap_util_map_ndb_to_value( i ) == 11 );
    }

    for(; i <= 128; ++i) {
        SNAP_ASSERT( snap_util_map_ndb_to_value( i ) == 12 );
    }

    for(; i <= 256; ++i) {
        SNAP_ASSERT( snap_util_map_ndb_to_value( i ) == 13 );
    }

    for(; i <= 512; ++i) {
        SNAP_ASSERT( snap_util_map_ndb_to_value( i ) == 14 );
    }
}


void snap_utils_test_map_value_to_ndb()
{
    SNAP_ASSERT( snap_util_map_value_to_ndb( 0 ) == 0 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 1 ) == 1 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 2 ) == 2 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 3 ) == 3 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 4 ) == 4 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 5 ) == 5 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 6 ) == 6 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 7 ) == 7 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 8 ) == 8 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 9 ) == 16 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 10 ) == 32 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 11 ) == 64 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 12 ) == 128 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 13 ) == 256 );
    SNAP_ASSERT( snap_util_map_value_to_ndb( 14 ) == 512 );
}


void snap_utils_test_get_sync_byte()
{
    SNAP_ASSERT( snap_util_get_sync_byte() == 'T' );
}


void snap_utils_test_get_fill_byte()
{
    SNAP_ASSERT( snap_util_get_fill_byte() == '\0' );
}


void snap_utils_test_get_address_size()
{
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

    SNAP_ASSERT( snap_util_get_address_size( 0 ) == 0 );
    SNAP_ASSERT( snap_util_get_address_size( 1 ) == 1 );
    SNAP_ASSERT( snap_util_get_address_size( 256 ) == 2 );
    SNAP_ASSERT( snap_util_get_address_size( 65536 ) == 3 );

    for(size_t i = 0; i < sizeof( v ) / sizeof( v[0] ); ++i) {
        SNAP_ASSERT( snap_util_get_address_size( v[ i ][ 0 ] ) == v[ i ][ 1 ] );
    }
}


void snap_utils_test_get_edm_size()
{
    // FIXME
}


void snap_utils_test_get_edm_size_value()
{
    // FIXME
}


void snap_utils_priv_create_ack_nak_response(bool ack, uint32_t lAddr, uint32_t pAddr)
{
    size_t (*bound)(uint32_t local_address, uint32_t peer_address) = NULL;
    bool (*create)(uint32_t local_address, uint32_t peer_address, uint8_t *data, size_t *pos, size_t size) = NULL;

    if( ack ) {
        bound = snap_util_ack_response_bound;
        create = snap_util_create_ack_response;
    }
    else {
        bound = snap_util_nak_response_bound;
        create = snap_util_create_nak_response;
    }

    size_t size =  bound( lAddr, pAddr );
    uint8_t *data = malloc( size );
    uint8_t *data_2 = data;
    size_t pos = 1;

    const size_t local_add_size = snap_util_get_address_size( lAddr );
    const size_t peer_add_size = snap_util_get_address_size( pAddr );

    SNAP_ASSERT( !create( lAddr, pAddr, data, &pos, size) );
    SNAP_ASSERT( pos == 1 );
    SNAP_ASSERT( data == data_2 );

    pos = 0;
    size--;
    SNAP_ASSERT( !create( lAddr, pAddr, data, &pos, size) );
    SNAP_ASSERT( pos == 0 );
    SNAP_ASSERT( data == data_2 );

    size++;

    SNAP_ASSERT( create( lAddr, pAddr, data, &pos, size) );
    SNAP_ASSERT( pos == 4 + local_add_size + peer_add_size );
    SNAP_ASSERT( pos == size );     // the bound function provides
                                    // accurate results in this case.
    SNAP_ASSERT( data == data_2 );

    SNAP_ASSERT( data[ 0 ] == snap_util_get_sync_byte() );
    SNAP_ASSERT( data[ 1 ] ==    ( peer_add_size * 64 )
                               + ( local_add_size * 16 )
                               + (uint8_t) ( ack ? 2 : 3 ) ); //< HDB2
    SNAP_ASSERT( data[ 2 ] == 32 ); // HDB1

    int index = 3;
    uint8_t crc_data[ 3 + local_add_size + peer_add_size ];
    crc_data[ 0 ] = snap_util_get_sync_byte(),
    crc_data[ 1 ] = ( peer_add_size * 64 ) + ( local_add_size * 16 );

    if( ack ) {
        crc_data[ 1 ] += (uint8_t) 2;
    }
    else {
        crc_data[ 1 ] += (uint8_t) 3;
    }

    crc_data[ 2 ] = 32;

    switch( peer_add_size ) {
    case 3:
        SNAP_ASSERT( data[ index ] == (uint8_t) ( pAddr >> 16 ) );
        crc_data[ index ] = (uint8_t) ( pAddr >> 16 );
        index++;
    case 2:
        SNAP_ASSERT( data[ index ] == (uint8_t) ( pAddr >> 8 ) );
        crc_data[ index ] = (uint8_t) ( pAddr >> 8 );
        index++;
    case 1:
        SNAP_ASSERT( data[ index ] == (uint8_t) ( pAddr ) );
        crc_data[ index ] = (uint8_t) ( pAddr );
        index++;
    default:
        break;
    }

    switch( local_add_size ) {
    case 3:
        SNAP_ASSERT( data[ index ] == (uint8_t) ( lAddr >> 16 ) );
        crc_data[ index ] = (uint8_t) ( lAddr >> 16 );
        index++;
    case 2:
        SNAP_ASSERT( data[ index ] == (uint8_t) ( lAddr >> 8 ) );
        crc_data[ index ] = (uint8_t) ( lAddr >> 8 );
        index++;
    case 1:
        SNAP_ASSERT( data[ index ] == ( (uint8_t) lAddr ) );
        crc_data[ index ] = (uint8_t) lAddr;
        index++;
    default:
        break;
    }

    for(int i = 0; i < index; ++i) {
        SNAP_ASSERT( data[ i ] == crc_data[ i ] );
    }

    SNAP_ASSERT( data[ index ] == snap_checksum( &crc_data[ 0 ], index ) );

    free( data );
}


void snap_utils_priv_create_ack_nak_response_mix(uint32_t lAddr, uint32_t pAddr)
{
    snap_utils_priv_create_ack_nak_response( true, lAddr, pAddr );
    snap_utils_priv_create_ack_nak_response( false, lAddr, pAddr );

    if( pAddr != lAddr ) {
        snap_utils_priv_create_ack_nak_response( true, pAddr, lAddr );
        snap_utils_priv_create_ack_nak_response( false, pAddr, lAddr );
    }
}


void snap_utils_test_create_ack_nak_response()
{
    const uint32_t v[] = {
        0, 1, 127, 128, 255, 256, 32767,
        32768, 65535, 65536, 8388607, 8388608, 16777215
    };

    size_t size = sizeof( v ) / sizeof( v[ 0 ] );

    for(size_t i = 0; i < size; ++i ) {
        for(size_t k = 0; k < size; ++k ) {
            snap_utils_priv_create_ack_nak_response_mix( v[ i ], v[ k ] );
        }
    }
}


void snap_utils_test_version()
{
    // LOL
    SNAP_ASSERT( snap_util_get_version() ==
       ( SNAP_MAJOR_VERSION << 16 ) + ( SNAP_MINOR_VERSION << 8 ) + SNAP_PATCH_LEVEL
    );
}



char test_suite_name[] = "utils";

void test_suite(CU_pSuite suite)
{
    CU_add_test( suite, "snap_allocator alloc", snap_utils_test_alloc );
    CU_add_test( suite, "snap_allocator free",  snap_utils_test_free );

    CU_add_test( suite, "test bit",  snap_utils_test_test_bit );
    CU_add_test( suite, "set bit",   snap_utils_test_set_bit );
    CU_add_test( suite, "clear bit", snap_utils_test_clear_bit );

    CU_add_test( suite, "memcpy", snap_utils_test_memcpy );

    CU_add_test( suite, "ndb total bytes",  snap_utils_test_get_ndb_total_bytes );
    CU_add_test( suite, "ndb fill bytes",   snap_utils_test_get_ndb_fill_bytes );
    CU_add_test( suite, "map ndb to value", snap_utils_test_map_ndb_to_value );
    CU_add_test( suite, "map value to ndb", snap_utils_test_map_value_to_ndb );
    CU_add_test( suite, "address size",     snap_utils_test_get_address_size );
    CU_add_test( suite, "edm size",         snap_utils_test_get_edm_size );
    CU_add_test( suite, "edm size value",   snap_utils_test_get_edm_size_value );

    CU_add_test( suite, "create ACK/NAK response", snap_utils_test_create_ack_nak_response );

    CU_add_test( suite, "sync byte",    snap_utils_test_get_sync_byte );
    CU_add_test( suite, "fill byte",    snap_utils_test_get_fill_byte );
    CU_add_test( suite, "snap version", snap_utils_test_version );
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
