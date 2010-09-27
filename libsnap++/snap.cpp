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
#include "snap.h"

#include <algorithm>
#include <libsnap/util.h>


namespace Snap
{


Snap::Snap()
  : snap( new snap_t )
{
    snap_init( snap );
}


Snap::~Snap()
{
    snap_reset( snap );
    delete snap;
}

size_t Snap::size() const
{
    return snap_get_metadata_size( snap );
}


void Snap::reset()
{
    snap_reset( snap );
}


const Statistic Snap::statistic() const
{
    return Statistic( snap_get_statistic( snap ) );
}


void Snap::clearStatistic()
{
    snap_clear_statistic( snap );
}


bool Snap::acknowledgementEnabled() const
{
    return snap_get_acknowledgement( snap );
}


void Snap::setAcknowledgementEnabled(bool enabled)
{
    snap_set_acknowledgement( snap, enabled );
}


size_t Snap::errorDetectionModeSize() const
{
    return snap_get_error_detection_mode_size( snap );
}


SnapErrorDetectionMode Snap::errorDetectionMode() const
{
    return snap_get_error_detection_mode( snap );
}


void Snap::setErrorDetectionMode(SnapErrorDetectionMode edm)
{
    snap_set_error_detection_mode( snap, edm );
}


size_t Snap::localAddressSize() const
{
    return snap_get_local_address_size( snap );
}


int32_t Snap::localAddress() const
{
    return snap_get_local_address( snap );
}


void Snap::setLocalAddress(int32_t address)
{
    snap_set_local_address( snap, address );
}


size_t Snap::peerAddressSize() const
{
    return snap_get_peer_address_size( snap );
}


int32_t Snap::peerAddress() const
{
    return snap_get_peer_address( snap );
}


void Snap::setPeerAddress(int32_t address)
{
    snap_set_peer_address( snap, address );
}


std::vector<uint8_t> Snap::encode(const std::vector<uint8_t> &data)
{
    return encode( &data[0], data.size() );
}


std::vector<uint8_t> Snap::encode(const uint8_t *data, size_t size)
{
    size_t alloc_size = encodeBound( size );
    uint8_t *result = new uint8_t[ alloc_size ];

    size_t index = 0;
    snap_encode( snap, data, size, result, &index, alloc_size );

    std::vector<uint8_t> vec;

    if( index > 0 ) {
        vec.resize( index );
        std::copy( result, result + index, std::back_inserter( vec ) );
    }

    delete[] result;

    return vec;
}


std::vector<uint8_t> Snap::decode(const std::vector<uint8_t> &data, std::vector<uint8_t> &response)
{
    return decode( &data[0], data.size(), response );
}


std::vector<uint8_t> Snap::decode(const uint8_t *data, size_t data_size,
                                  std::vector<uint8_t> &response)
{
    size_t result_alloc_size = decodeResultBound( data_size );
    uint8_t *result_data = new uint8_t[ result_alloc_size ];

    size_t response_alloc_size = decodeResponseBound( data_size );
    uint8_t *response_data = new uint8_t[ response_alloc_size ];

    size_t data_pos = 0;
    size_t result_pos = 0;
    size_t response_pos = 0;

    snap_decode( snap, data, &data_pos, data_size,
                 result_data, &result_pos, result_alloc_size,
                 response_data, &response_pos, response_alloc_size
     );

    std::vector<uint8_t> result;
    if( result_pos > 0 ) {
        result.resize( result_pos );
        std::copy( result_data, result_data + result_pos,
                   std::back_inserter( result )
         );
    }

    if( response_pos > 0 ) {
        response.resize( response.size() + response_pos );
        std::copy( response_data, response_data + response_pos,
                   std::back_inserter( response )
         );
    }

    delete[] result_data;
    delete[] response_data;

    return result;
}


uint32_t Snap::version() const
{
    return snap_util_get_version();
}


uint16_t Snap::header(size_t size) const
{
    return snap_get_header( snap, size );
}


size_t Snap::encodeBound(size_t size) const
{
    return snap_encode_bound( snap, size);
}


size_t Snap::decodeResultBound(size_t size) const
{
    return snap_decode_result_bound( snap, size);
}


size_t Snap::decodeResponseBound(size_t size) const
{
    return snap_decode_result_bound( snap, size);
}


} // EndNamespace


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
