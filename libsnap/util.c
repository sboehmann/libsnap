/*
 * Copyright 2009-2010  Stefan Böhmann <kde@hilefoks.org>
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
#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"
#include "edm.h"

/* sync byte 01010100 - ascii 'T, hex 0x54' */
#define SNAP_SYNC_BYTE 0x54
#define SNAP_FILL_BYTE '\0'


typedef enum
{
    TX_NAK = 0,
    TX_ACK = 1,
    RX_ACK = 2,
    RX_NAK = 3
} PacketType;


// forward declaration
bool snap_util_priv_create_ack_nak_packet(PacketType type, uint32_t local_address, uint32_t peer_address, uint8_t *data, size_t *pos, size_t size);
size_t snap_util_priv_ack_nak_packet_bound(PacketType type, uint32_t local_address, uint32_t peer_address);



void* snap_util_alloc(snap_allocator_t *allocator, size_t size)
{
    SNAP_ASSERT( size > 0 );

    if( allocator != NULL && allocator->alloc != NULL ) {
        return allocator->alloc( size, allocator->opaque );
    }

    return malloc( size );
}


void snap_util_free(snap_allocator_t *allocator, void *ptr)
{
    if( allocator != NULL && allocator->free != NULL ) {
        allocator->free( ptr, allocator->opaque );
    }
    else {
        free( ptr );
    }
}


bool snap_util_test_bit(uint8_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 8 );
    return data & ( 1U << bit );
}


uint8_t snap_util_set_bit(uint8_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 8 );
    return data | ( 1U << bit );
}


uint8_t snap_util_clear_bit(uint8_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 8 );
    return data & ( ~( 1U << bit ) );
}


bool snap_util_test_bit16(uint16_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 16 );
    return data & ( 1U << bit );
}


uint16_t snap_util_set_bit16(uint16_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 16 );
    return data | ( 1U << bit );
}


uint16_t snap_util_clear_bit16(uint16_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 16 );
    return data & ( ~( 1U << bit ) );
}


bool snap_util_test_bit32(uint32_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 32 );
    return data & ( 1UL << bit );
}


uint32_t snap_util_set_bit32(uint32_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 32 );
    return data | ( 1UL << bit );
}


uint32_t snap_util_clear_bit32(uint32_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 32 );
    return data & ( ~( 1UL << bit ) );
}


bool snap_util_test_bit64(uint64_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 64 );
    return data & ( 1ULL << bit );
}


uint64_t snap_util_set_bit64(uint64_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 64 );
    return data | ( 1ULL << bit );
}


uint64_t snap_util_clear_bit64(uint64_t data, uint8_t bit)
{
    SNAP_ASSERT( bit < 64 );
    return data & ( ~( 1ULL << bit ) );
}


void* snap_util_memcpy(void *dest, const void *src, size_t n)
{
    SNAP_CHECK_PTR( dest );
    SNAP_CHECK_PTR( src );

    uint8_t *dp = (uint8_t*) dest;
    const uint8_t *sp = (const uint8_t*) src;

    while( n-- ) {
        *dp++ = *sp++;
    }

    return dest;
}


uint16_t snap_util_get_ndb_total_bytes(size_t in_data_size)
{
    if( in_data_size <= 8 ) {
        return in_data_size;
    }
    else if( in_data_size <= 16 ) {
        return 16;
    }
    else if( in_data_size <= 32 ) {
        return 32;
    }
    else if( in_data_size <= 64 ) {
        return 64;
    }
    else if( in_data_size <= 128 ) {
        return 128;
    }
    else if( in_data_size <= 256 ) {
        return 256;
    }

    return 512;
}


uint16_t snap_util_get_ndb_fill_bytes(size_t in_data_size)
{
    if( in_data_size <= 8 ) {
        return 0;
    }
    else if( in_data_size <= 16 ) {
        return 16 - in_data_size;
    }
    else if( in_data_size <= 32 ) {
        return 32 - in_data_size;
    }
    else if( in_data_size <= 64 ) {
        return 64 - in_data_size;
    }
    else if( in_data_size <= 128 ) {
        return 128 - in_data_size;
    }
    else if( in_data_size <= 256 ) {
        return 256 - in_data_size;
    }
    else if( in_data_size <= 512 ) {
        return 512 - in_data_size;
    }

    return 0;
}

#if 0
size_t snap_util_get_max_count_fill_bytes(size_t in_data_size)
{
    if( in_data_size <= 8 ) {
        return 0;
    }
    else if(  in_data_size <= 16 ) {
        return 16 - 9;
    }
    else if( in_data_size <= 32 ) {
        return 32 - 17;
    }
    else if( in_data_size <= 64 ) {
        return 64 - 33;
    }
    else if( in_data_size <= 128 ) {
        return 128 - 65;
    }
    else if( in_data_size <= 256 ) {
        return 256 - 129;
    }

    return 512 - 257;
}
#endif

uint8_t snap_util_map_ndb_to_value(size_t size)
{
    const size_t ndb = snap_util_get_ndb_total_bytes( size );

    if( ndb <= 8 ) {
        return ndb;
    }
    else if( ndb <= 16 ) {
        return 9;
    }
    else if( ndb <= 32 ) {
        return 10;
    }
    else if( ndb <= 64 ) {
        return 11;
    }
    else if( ndb <= 128 ) {
        return 12;
    }
    else if( ndb <= 256 ) {
        return 13;
    }

    return 14;
}


uint16_t snap_util_map_value_to_ndb(uint8_t value)
{
    SNAP_ASSERT( value <= 14 );
    static const size_t table[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 32, 64, 128, 256, 512 };

    return table[ value ];
}


uint8_t snap_util_get_sync_byte()
{
    return SNAP_SYNC_BYTE;
}


uint8_t snap_util_get_fill_byte()
{
    return SNAP_FILL_BYTE;
}


size_t snap_util_ack_response_bound(uint32_t local_address, uint32_t peer_address)
{
    return snap_util_priv_ack_nak_packet_bound( RX_ACK, local_address, peer_address );
}


bool snap_util_create_ack_response(uint32_t local_address, uint32_t peer_address, uint8_t *data, size_t *pos, size_t size)
{
    return snap_util_priv_create_ack_nak_packet(
                           RX_ACK, local_address, peer_address, data, pos, size );
}


size_t snap_util_nak_response_bound(uint32_t local_address, uint32_t peer_address)
{
    return snap_util_priv_ack_nak_packet_bound( RX_NAK, local_address, peer_address );
}


bool snap_util_create_nak_response(uint32_t local_address, uint32_t peer_address, uint8_t *data, size_t *pos, size_t size)
{
    return snap_util_priv_create_ack_nak_packet(
                RX_NAK, local_address, peer_address, data, pos, size );
}


extern size_t snap_util_get_address_size(uint32_t address)
{
    SNAP_ASSERT( address < ( 1 << 24 ) );

    if( address <= 0 ) {
        return 0;
    }
    else if( address < ( 1 << 8 ) ) {
        return 1;
    }
    else if( address < ( 1 << 16 ) ) {
        return 2;
    }

    return 3;
}


size_t snap_util_get_edm_size(SnapErrorDetectionMode edm)
{
    size_t size = 0;

    switch( edm ) {
        case EDM_THREETIMES:
            break;

        case EDM_CHECKSUM:
        case EDM_CRC8:
            size = 1;
            break;

        case EDM_CRC16:
            size = 2;
            break;

        case EDM_CRC32:
            size = 4;
            break;

        case EDM_FEC:
            //FIXME forward error correction not implemented yet
            //      (and not specified by the S.N.A.P specification).
            break;

        case EDM_USER:
            //FIXME not implemented yet
            break;

        default:
            SNAP_ASSERT( edm == EDM_NONE );
            break;
    }

    return size;
}


uint8_t snap_util_get_edm_size_value(SnapErrorDetectionMode edm)
{
    uint8_t value = 0;

    switch( edm ) {
        case EDM_THREETIMES:
            value = 1 << 4;
            break;

        case EDM_CHECKSUM:
            value = 2 << 4;
            break;

        case EDM_CRC8:
            value = 3 << 4;
            break;

        case EDM_CRC16:
            value = 4 << 4;
            break;

        case EDM_CRC32:
            value = 5 << 4;
            break;

        case EDM_FEC:
            value = 6 << 4;
            break;

        case EDM_USER:
            value = 7 << 4;
            break;

        default: // (No EDM)
            SNAP_ASSERT( edm == EDM_NONE );
            break;
    }

    return value;
}



uint32_t snap_util_get_version()
{
    return ( SNAP_MAJOR_VERSION << 16 ) + ( SNAP_MINOR_VERSION << 8 ) + SNAP_PATCH_LEVEL;
}


bool snap_util_priv_create_ack_nak_packet(PacketType type, uint32_t local_address, uint32_t peer_address, uint8_t *data, size_t *pos, size_t size)
{
    SNAP_CHECK_PTR( data );

    SNAP_ASSERT( snap_util_ack_response_bound(local_address, peer_address ) == snap_util_nak_response_bound(local_address, peer_address ) );

    if( ( size - *pos ) < snap_util_ack_response_bound(local_address, peer_address ) ) {
        return false;
    }

    const size_t local_add_size = snap_util_get_address_size( local_address );
    const size_t peer_add_size = snap_util_get_address_size( peer_address );

    const size_t initialPos = *pos;

    data[ (*pos)++ ] = snap_util_get_sync_byte();
    data[ (*pos)++ ] = ( peer_add_size << 6 ) + ( local_add_size << 4 ) + (uint8_t) type;
    data[ (*pos)++ ] = 32; /* Only bit 5 (e.g. edm is 8 bit checksum) */

    switch( peer_add_size ) {
    case 3:
        data[ (*pos)++ ] = (uint8_t) ( peer_address >> 16 );
    case 2:
        data[ (*pos)++ ] = (uint8_t) ( peer_address >> 8 );
    case 1:
        data[ (*pos)++ ] = (uint8_t) ( peer_address );
    default:
        break;
    }

    switch( local_add_size ) {
    case 3:
        data[ (*pos)++ ] = (uint8_t) ( local_address >> 16 );
    case 2:
        data[ (*pos)++ ] = (uint8_t) ( local_address >> 8 );
    case 1:
        data[ (*pos)++ ] = (uint8_t) ( local_address );
    default:
        break;
    }

    data[ *pos ] = snap_checksum( &data[ initialPos ], (*pos) - initialPos );
    (*pos)++;

    return true;
}


size_t snap_util_priv_ack_nak_packet_bound(PacketType type, uint32_t local_address, uint32_t peer_address)
{
    SNAP_UNUSED( type );

    const size_t sync = 1;
    const size_t header = 2;
    const size_t checksum = 1; //TODO make ack/nak checksum type configurable
    const size_t local_add_size = snap_util_get_address_size( local_address );
    const size_t peer_add_size = snap_util_get_address_size( peer_address );

    return sync + header + checksum + local_add_size + peer_add_size;
}


#ifdef __cplusplus
}   // end extern "C"
#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
