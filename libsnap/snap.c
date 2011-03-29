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
#ifdef __cplusplus
extern "C" {
#endif

#include "snap.h"

#include "util.h"
#include "edm.h"


void snap_init(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    ptr->ack = false;
    ptr->edm = EDM_NONE;
    ptr->local_address = 0;
    ptr->peer_address = 0;

    ptr->packet.bound = 0;
    ptr->packet.edm = EDM_NONE;
    ptr->packet.local_address = 0;
    ptr->packet.peer_address = 0;
    ptr->packet.size = 0;

    ptr->last_decode_error_pos = 0;

    snap_clear_statistic( ptr );
    snap_clear( ptr );
}


void snap_reset(snap_t *ptr)
{
    snap_init( ptr );
}


bool snap_is_ready(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    if( ptr->last_error != SNAP_ERROR_NONE ) {
        return false;
    }

    if( ptr->packet.size > 0 ) {
        return false;
    }

    return true;
}


void snap_clear(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    ptr->packet.size = 0;
    ptr->packet.bound = 0;
    ptr->last_error = SNAP_ERROR_NONE;
}


snap_statistic_t snap_get_statistic(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return ptr->stat;
}


void snap_clear_statistic(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    ptr->stat.rejected_packets = 0;
    ptr->stat.transmitted_packets = 0;
    ptr->stat.received_packets = 0;
    ptr->stat.total_packets = 0;
    ptr->stat.rejected_bytes = 0;
    ptr->stat.transmitted_bytes = 0;
    ptr->stat.received_bytes = 0;
    ptr->stat.total_bytes = 0;
    ptr->stat.transmitted_payload_bytes = 0;
    ptr->stat.received_payload_bytes = 0;
    ptr->stat.total_payload_bytes = 0;
    ptr->stat.transmitted_fill_bytes = 0;
    ptr->stat.received_fill_bytes = 0;
    ptr->stat.total_fill_bytes = 0;
}


SnapErrorCode snap_get_last_error(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return ptr->last_error;
}


size_t snap_get_metadata_size(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    const size_t sync = 1;
    const size_t hdb = 2;
    const size_t dab = snap_get_peer_address_size( ptr );
    const size_t sab = snap_get_local_address_size( ptr );
    const size_t crc = snap_get_error_detection_mode_size( ptr );

    return sync + hdb + dab + sab + crc;
}


bool snap_get_acknowledgement(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return ptr->ack;
}


void snap_set_acknowledgement(snap_t *ptr, bool ack)
{
    SNAP_CHECK_PTR( ptr );

    ptr->ack = ack;
}


size_t snap_get_error_detection_mode_size(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return snap_util_get_edm_size( snap_get_error_detection_mode( ptr ) );
}


SnapErrorDetectionMode snap_get_error_detection_mode(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    if( ptr->edm <= EDM_USER ) {
        return ptr->edm;
    }

    return EDM_NONE;
}


void snap_set_error_detection_mode(snap_t *ptr, SnapErrorDetectionMode edm)
{
    SNAP_CHECK_PTR( ptr );

    if( edm <= EDM_USER ) {
        ptr->edm = edm;
    }
    else {
        ptr->edm = EDM_NONE;
    }
}


size_t snap_get_local_address_size(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return snap_util_get_address_size( ptr->local_address );
}


uint32_t snap_get_local_address(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return ptr->local_address;
}


void snap_set_local_address(snap_t *ptr, uint32_t address)
{
    SNAP_CHECK_PTR( ptr );

    ptr->local_address = address;
}


size_t snap_get_peer_address_size(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return snap_util_get_address_size( ptr->peer_address );
}


uint32_t snap_get_peer_address(snap_t *ptr)
{
    SNAP_CHECK_PTR( ptr );

    return ptr->peer_address;
}


void snap_set_peer_address(snap_t *ptr, uint32_t address)
{
    SNAP_CHECK_PTR( ptr );

    ptr->peer_address = address;
}


uint16_t snap_get_header(snap_t *ptr, size_t in_data_size)
{
    const uint16_t result = snap_get_hdb2( ptr, in_data_size ) << 8u;

    return result + snap_get_hdb1( ptr, in_data_size );
}


uint8_t snap_get_hdb1(snap_t *ptr, size_t in_data_size)
{
    SNAP_CHECK_PTR( ptr );

    uint8_t hdb1 = snap_util_get_edm_size_value( snap_get_error_detection_mode( ptr ) );
    //hdb1 += ( snap_is_command_mode_enabled( ptr ) ? 1 : 0 ) << 7;

    return hdb1 + snap_util_map_ndb_to_value( in_data_size );
}


uint8_t snap_get_hdb2(snap_t *ptr, size_t in_data_size)
{
    SNAP_CHECK_PTR( ptr );
    SNAP_UNUSED( in_data_size );

    uint8_t hdb2 = 0;
    hdb2 += snap_get_peer_address_size( ptr ) << 6;
    hdb2 += snap_get_local_address_size( ptr ) << 4;
    //hdb2 += snap_get_protocol_specify_flag_size( ptr ) << 2;

    hdb2 += snap_get_acknowledgement( ptr ) ? 1 : 0;

    return hdb2;
}


size_t snap_encode_bound(snap_t *ptr, size_t in_data_size)
{
    SNAP_CHECK_PTR( ptr );

    size_t size = snap_get_metadata_size( ptr ) + snap_util_get_ndb_total_bytes( in_data_size );

    if( snap_get_error_detection_mode( ptr ) == EDM_THREETIMES ) {
        size *= 3;
    }

    return size;
}


bool snap_encode(snap_t *ptr, const uint8_t *in_data, size_t in_data_size, uint8_t *out_data, size_t *out_data_pos, size_t out_data_size)
{
    SNAP_CHECK_PTR( ptr );
    SNAP_CHECK_PTR( out_data );
    SNAP_CHECK_PTR( out_data_pos );

    const size_t in_bytes = in_data_size <= 512 ? in_data_size : 512;

    if( !snap_is_ready( ptr ) ) {
        ptr->last_error = SNAP_ERROR_STATE;
        return false;
    }

    ptr->last_error = SNAP_ERROR_NONE;

    if( in_data == NULL || in_bytes <= 0 ) {
        return true;
    }

    ptr->last_decode_error_pos = 0;

    const size_t initialOutDataPos = *out_data_pos;

    const size_t bound = snap_encode_bound( ptr, in_data_size );

    if( ( out_data_size - *out_data_pos ) < bound ) {
        ptr->last_error = SNAP_ERROR_BOUND;
        return false;
    }

    for(int i = 0; i < 3; ++i) {
        out_data[ (*out_data_pos)++ ] = snap_util_get_sync_byte();
        out_data[ (*out_data_pos)++ ] = snap_get_hdb2( ptr, in_data_size );
        out_data[ (*out_data_pos)++ ] = snap_get_hdb1( ptr, in_data_size );

        switch( snap_get_peer_address_size( ptr ) ) {
        case 3:
            out_data[ (*out_data_pos)++ ] =
                (uint8_t) ( snap_get_peer_address( ptr ) >> 16 );
        case 2:
            out_data[ (*out_data_pos)++ ] =
                (uint8_t) ( snap_get_peer_address( ptr ) >> 8 );
        case 1:
            out_data[ (*out_data_pos)++ ] =
                (uint8_t) snap_get_peer_address( ptr );
        default:
            break;
        }

        switch( snap_get_local_address_size( ptr ) ) {
        case 3:
            out_data[ (*out_data_pos)++ ] =
                (uint8_t) ( snap_get_local_address( ptr ) >> 16 );
        case 2:
            out_data[ (*out_data_pos)++ ] =
                (uint8_t) ( snap_get_local_address( ptr ) >> 8 );
        case 1:
            out_data[ (*out_data_pos)++ ] =
                (uint8_t) snap_get_local_address( ptr );
        default:
            break;
        }


        // copy data
        for(size_t i = 0; i < in_bytes; ++i) {
            out_data[ (*out_data_pos)++ ] = in_data[ i ];
        }

        // fill
        for(size_t i = 0; i < snap_util_get_ndb_fill_bytes( in_data_size ); ++i) {
            out_data[ (*out_data_pos)++ ] = snap_util_get_fill_byte();
        }

        switch( snap_get_error_detection_mode( ptr ) ) {
        case EDM_THREETIMES:
            break;

        case EDM_CHECKSUM:
            out_data[ *out_data_pos ] =
                snap_checksum( &out_data[ initialOutDataPos + 1 ],
                               *out_data_pos - initialOutDataPos - 1
                             );
            (*out_data_pos)++;
            break;

        case EDM_CRC8:
            out_data[ *out_data_pos ] =
                snap_crc8( &out_data[ initialOutDataPos + 1 ],
                           *out_data_pos - initialOutDataPos - 1
                         );
            (*out_data_pos)++;
            break;

        case EDM_CRC16:
            {
                uint16_t value = snap_crc16( &out_data[ initialOutDataPos + 1 ],
                                             *out_data_pos - initialOutDataPos - 1
                                           );

                out_data[ (*out_data_pos)++ ] = (uint8_t) ( value >> 8 );
                out_data[ (*out_data_pos)++ ] = (uint8_t) value;
            }
            break;
        case EDM_CRC32:
            {
                uint32_t value =  snap_crc32( &out_data[ initialOutDataPos + 1 ],
                                            *out_data_pos - initialOutDataPos - 1
                                            );

                out_data[ (*out_data_pos)++ ] = (uint8_t) ( value >> 24 );
                out_data[ (*out_data_pos)++ ] = (uint8_t) ( value >> 16 );
                out_data[ (*out_data_pos)++ ] = (uint8_t) ( value >> 8 );
                out_data[ (*out_data_pos)++ ] = (uint8_t) value;
            }
            break;
        case EDM_FEC:
        case EDM_USER:
            break;

        default:
            SNAP_ASSERT( snap_get_error_detection_mode( ptr ) == EDM_NONE );
            break;
        }

        if( snap_get_error_detection_mode( ptr ) != EDM_THREETIMES ) {
            break;
        }
    }

    ptr->stat.transmitted_payload_bytes = 0;
    ptr->stat.transmitted_fill_bytes = 0;

    ptr->stat.total_packets++;
    ptr->stat.total_bytes += (*out_data_pos) - initialOutDataPos;
    ptr->stat.total_payload_bytes += in_bytes;
    ptr->stat.total_fill_bytes += snap_util_get_ndb_fill_bytes( in_data_size );


    if( snap_get_acknowledgement( ptr ) ) {
        ptr->packet.bound = bound;
        ptr->packet.edm = snap_get_error_detection_mode( ptr );
        ptr->packet.local_address = snap_get_local_address( ptr );
        ptr->packet.peer_address = snap_get_peer_address( ptr );
        ptr->packet.size = in_bytes;

        snap_util_memcpy( ptr->packet.data, in_data, in_bytes );
    }
    else {
        ptr->stat.transmitted_packets++;
        ptr->stat.transmitted_bytes += (*out_data_pos) - initialOutDataPos;
        ptr->stat.transmitted_payload_bytes += in_bytes;
        ptr->stat.transmitted_fill_bytes += snap_util_get_ndb_fill_bytes( in_data_size );
    }

    // out data pos should point to the next not yet written byte
    (*out_data_pos)++;

    return true;
}


size_t snap_decode_result_bound(snap_t *ptr, size_t in_data_size)
{
    SNAP_CHECK_PTR( ptr );
    SNAP_UNUSED( ptr );

    if( in_data_size >= 512 ) {
        return 512;
    }

    return in_data_size;
}


size_t snap_decode_response_bound(snap_t *ptr, size_t in_data_size)
{
    SNAP_CHECK_PTR( ptr );
    SNAP_UNUSED( in_data_size );

    // We need at least 3 bytes (one packet is at least 3 bytes without any data).
    if( in_data_size < 3 ) {
        return 0;
    }

    if( !snap_is_ready( ptr ) ) {
        // the bound of the last packet can't be less then 3 (sync + hdb2 + hdb1 )
        SNAP_ASSERT( ptr->packet.bound >= 3 );

        // 3 * ( sync byte + header bytes + max address bytes (3 local + 3 peer) + max data bytes + max edm bytes )
        SNAP_ASSERT( ptr->packet.bound <= ( 3 * ( 1 + 2 + 6 + 512 + 2 ) ) ); // == 1569

        // if we wait for an ACK, the max. response can be the last packet (in case of NAK)
        return ptr->packet.bound;
    }

    const uint32_t l = snap_get_local_address( ptr );
    const uint32_t p = snap_get_peer_address( ptr );

    const size_t a = snap_util_ack_response_bound( l, p );
    const size_t n = snap_util_nak_response_bound( l, p );

    // in all other cases the response can only be an ACK/NAK packet, if at all.
    return a >= n ? a : n;
}


bool snap_decode(snap_t *ptr,
        const uint8_t *in_data, size_t *in_data_pos, size_t in_data_size,
              uint8_t *result, size_t *result_pos, size_t result_size,
              uint8_t *response, size_t *response_pos, size_t response_size)
{
    SNAP_CHECK_PTR( ptr );
    SNAP_CHECK_PTR( in_data );
    SNAP_CHECK_PTR( in_data_pos );
    SNAP_CHECK_PTR( result );
    SNAP_CHECK_PTR( result_pos );
    SNAP_CHECK_PTR( response );
    SNAP_CHECK_PTR( response_pos );

    SNAP_ASSERT( in_data_size >= *in_data_pos );
    SNAP_ASSERT( result_size >= *result_pos );
    SNAP_ASSERT( response_size >= *response_pos );

    SNAP_ASSERT( ( result_size - *result_pos ) >= snap_decode_result_bound( ptr, in_data_size - *in_data_pos ) );
    SNAP_ASSERT( ( response_size - *response_pos ) >= snap_decode_response_bound( ptr, in_data_size - *in_data_pos ) );

    ptr->last_error = SNAP_ERROR_NONE;
    ptr->last_decode_error_pos = 0;

    size_t real_pos = *in_data_pos;
    size_t skip_preamble = real_pos;

    // if there are really no data...
    if( ( in_data_size - *in_data_pos ) <= 0 ) {
        return true; // not enough data
    }

    // skip the codswallop
    while( in_data[ real_pos ] != snap_util_get_sync_byte() ) {
        real_pos++;
        skip_preamble++;
        (*in_data_pos)++;

        if( ( in_data_size - *in_data_pos ) <= 0 ) {
            return true; // not enough data
        }
    }

    SNAP_ASSERT( in_data[ real_pos ] == snap_util_get_sync_byte() );
    SNAP_ASSERT( in_data[ skip_preamble ] == snap_util_get_sync_byte() );

    // min packet size is 3 byte (1 sync byte, 2 header bytes).
    if( ( in_data_size - *in_data_pos ) < 3 ) {
        return true; // not enough data
    }

    real_pos++; // skip sync byte

    const uint8_t hdb2 = in_data[ real_pos++ ];  // header byte 2
    const uint8_t hdb1 = in_data[ real_pos++ ];  // header byte 1

    const bool cmdmode               = ( hdb1 & 0x80 ) >> 7u;  // 10000000
    const SnapErrorDetectionMode edm = ( hdb1 & 0x70 ) >> 4u;  // 01110000
    uint16_t ndb                     = hdb1 & 0x0F;            // 00001111

    const uint8_t dab                = ( hdb2 & 0xC0 ) >> 6u;  // 11000000
    const uint8_t sab                = ( hdb2 & 0x30 ) >> 4u;  // 00110000
    const uint8_t pfb                = ( hdb2 & 0x0C ) >> 2u;  // 00001100
    const uint8_t ack                = hdb2 & 0x03;            // 00000011

    // NOTE not implemented yet (usefull but optional feature)
    SNAP_UNUSED( cmdmode );

    // NOTE The pfb flag byte are reserved but not yet specified by the
    //      S.N.A.P. specification. They are intended for future enhancements.
    SNAP_UNUSED( pfb );


    // calculate size of edm
    uint8_t edm_size = 0;
    switch( edm ) {
        case EDM_NONE:
        case EDM_THREETIMES:
        case EDM_FEC:
        case EDM_USER:
            break;

        case EDM_CHECKSUM:
        case EDM_CRC8:
            edm_size = 1;
            break;

        case EDM_CRC16:
            edm_size = 2;
            break;

        case EDM_CRC32:
            edm_size = 4;
            break;

        default:
            // bullshit...
            ptr->last_decode_error_pos = real_pos;
            ptr->last_error = SNAP_ERROR_UNKNOWN_EDM_TYPE;

            // Because this looks like a broken packet, we only skip the preamble,
            // the sync byte and the 2 header bytes. All the other crap will be
            // skipped anyway when calling snap_decode again.
            *in_data_pos += skip_preamble + 1 + 2;

            // We can't send an ACK. We do not know who sent the package.
            // We even do not know who are the recipient.
            return false;
    }

    // retranslate ndb
    if( ndb <= 14 ) {
        ndb = snap_util_map_value_to_ndb( ndb );
    }
    else {
        // illegal ndb value...
        ptr->last_decode_error_pos = real_pos;
        ptr->last_error = SNAP_ERROR_UNKNOWN_NDB_VALUE;

        // Because this looks like a broken packet, we only skip the preamble,
        // the sync byte and the 2 header bytes. All the other crap will be
        // skipped anyway when calling snap_decode again.
        *in_data_pos += skip_preamble + 1 + 2;

        // We can't send an ACK. We do not know who sent the package.
        // We even do not know who are the recipient.
        return false;
    }

    // calculate some significant packet sizes
    const uint16_t packet_size = 1 + 2 + ndb + dab + sab + edm_size;
    const uint16_t begin_of_packet = skip_preamble;
    const uint16_t begin_of_data = begin_of_packet + 1 + 2 + dab + sab;
    const uint16_t end_of_data = begin_of_data + ndb;
    const uint16_t end_of_packet = end_of_data + edm_size;

    SNAP_ASSERT( packet_size == end_of_packet - begin_of_packet );
    SNAP_ASSERT( ndb == end_of_data - begin_of_data );

    // check if we have enough data
    const uint16_t required_data_bytes = edm == EDM_THREETIMES
                                           ? 3 * packet_size
                                            : packet_size;

    if( in_data_size < required_data_bytes ) {
        // not enough data. That's okay, we just have to wait for more data.
        return true;
    }

    // read destination address bytes, if any
    uint32_t dest_address = 0;
    for(uint8_t i = dab; i > 0; --i) {
        dest_address += ( in_data[ real_pos++ ] << ( ( i - 1u ) * 8u ) );
    }

    // read source address bytes, if any
    uint32_t source_address = 0;
    for(uint8_t i = sab; i > 0; --i) {
        source_address += ( in_data[ real_pos++ ] << ( ( i - 1u ) * 8u ) );
    }

#ifdef SNAP_REJECT_PACKETS_WITH_WRONG_ADDRESS
    // TODO Test (i.e. write unit tests) !!!
    // if we have a real destination address (0 means broadcarst or no address
    // at all) then check if we are the receiver.
    if( dest_address != 0 && snap_get_local_address( ptr ) != 0 ) {
        if( snap_get_local_address( ptr ) != dest_address ) {
            // We are not the receiver of ptr data packet, so skip ptr packet.
            *in_data_pos += end_of_data;

            return true;
        }
    }
#endif

    // calculate current packet checksum
    uint32_t edm_code_expected = 0;
    switch( edm ) {
        case EDM_THREETIMES:
        case EDM_FEC:
        case EDM_USER:
            break;

        case EDM_CHECKSUM:
            edm_code_expected = snap_checksum( in_data + begin_of_packet + 1,
                                            end_of_data - 1
                                             );
            break;

        case EDM_CRC8:
            edm_code_expected = snap_crc8( in_data + begin_of_packet + 1,
                                            end_of_data - 1
                                         );
            break;

        case EDM_CRC16:
            edm_code_expected = snap_crc16( in_data + begin_of_packet + 1,
                                            end_of_data - 1
                                           );
            break;

        case EDM_CRC32:
            edm_code_expected = snap_crc32( in_data + begin_of_packet + 1,
                                            end_of_data - 1
                                          );
            break;

        default: // (No EDM)
            SNAP_ASSERT( edm == EDM_NONE );
            break;
    }

    // check error detection code
    SNAP_ASSERT( end_of_packet - end_of_data == edm_size );

    uint32_t edm_code = 0;
    size_t edm_pos = end_of_data;
    for(size_t i = edm_size; i > 0; --i) {
        edm_code += in_data[ edm_pos++ ] << ( ( i - 1u ) * 8u );
    }

    if( edm_code != edm_code_expected ) {
        ptr->last_decode_error_pos = end_of_data;
        ptr->last_error = SNAP_ERROR_INVALID_EDM;

        *in_data_pos += end_of_packet;

        if( ack == 1 ) {
            snap_util_create_nak_response( dest_address, source_address,
                                         response, response_pos, response_size
                                       );
        }

        return false;
    }

    // calculate number of fill bytes (0-bytes) we can/have to skip from the end.
    size_t skip_byte_count = 0;
    for(size_t i = end_of_data - 1; i > begin_of_data; --i) {
        if( in_data[ i ] != snap_util_get_fill_byte() ) {
            break;
        }

        skip_byte_count++;
    }

    // read significant data bytes
    for(size_t i = begin_of_data; i < end_of_data - skip_byte_count; ++i) {
        uint8_t value = in_data[ i ];
        result[ *result_pos ] = value;
        (*result_pos)++;
        real_pos++;
    }

    if( ack == 1 ) {
        snap_util_create_ack_response( dest_address, source_address,
                                     response, response_pos, response_size
                                   );
    }

    if( edm == EDM_THREETIMES ) {
        *in_data_pos += 3 * end_of_packet;
    }
    else {
        *in_data_pos += end_of_packet;
    }

    return true;
}


#ifdef __cplusplus
}   // end extern "C"
#endif


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
