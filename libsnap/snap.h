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
#ifndef LIBSNAP_SNAP_H
#define LIBSNAP_SNAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libsnap/global.h>


/**
 * @file libsnap/snap.h
 * @ingroup libsnap
 * @brief S.N.A.P network protocol implementation
 *
 * The S.N.A.P network protocol defines a simple and generic network protocol
 * that can be used in many different environments.
 *
 * @note
 * libsnap is able to compress packets.
 * Because this feature is not part of the S.N.A.P protocol specification,
 * libsnap uses the currently unused protocol specific flag byte PFB
 * (Bit 2 and 3 of HDB2).
 *
 * @code
 * snap_t snap;
 * snap_init( &snap );
 * ...
 * snap_set_local_address( &snap, myLocalAddress );
 * ...
 * snap_encode( ... );
 * @endcode
 *
 * Have fun!
 *
 * @version 0.1
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


#define SNAP_MAXIMUM_PACKET_DATA_SIZE 512

/**
 * @struct snap_packet_t
 * @brief This struct represent a single packet
 *
 */
typedef struct
{
    size_t bound;

    SnapErrorDetectionMode edm;
    uint32_t local_address;
    uint32_t peer_address;

    size_t size;
    uint8_t data[ SNAP_MAXIMUM_PACKET_DATA_SIZE ];
} snap_packet_t;


/**
 * @struct snap_t
 * @brief This is the base structure to handle the S.N.A.P protocol
 *
 * @see snap_init
 * @see snap_reset
 */
typedef struct
{
    bool ack;
    SnapErrorDetectionMode edm;
    uint32_t local_address;
    uint32_t peer_address;

    snap_packet_t packet;
    snap_statistic_t stat;

    size_t last_decode_error_pos;
    SnapErrorCode last_error;
} snap_t;



/**
 * @brief Initialize a snap_t structure
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_init(snap_t *ptr);


/**
 * @brief Resets all communication parameters.
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_reset(snap_t *ptr);


/**
 * @brief
 *
 * @see snap_clear
 *
 * @todo refactor to snap_has_error()
 *
 * @param ptr snap_t instance pointer
 */
extern bool snap_is_ready(snap_t *ptr);


/**
 * @brief Returns the current state
 *
 * @todo refactor to snap_clear_error()
 *
 * @see snap_is_ready
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_clear(snap_t *ptr);


/**
 * @brief Returns the type of error that last occurred.
 *
 * @todo refactor to snap_get_error()
 *
 * @param ptr snap_t instance pointer
 */
extern SnapErrorCode snap_get_last_error(snap_t *ptr);


/**
 * @brief Returns statistical details
 *
 * @see snap_clear_statistic
 *
 * @param ptr snap_t instance pointer
 */
extern snap_statistic_t snap_get_statistic(snap_t *ptr);


/**
 * @brief Clears the current statistic.
 *
 * @see snap_get_statistic
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_clear_statistic(snap_t *ptr);



/**
 * @brief Returns the number of metadata bytes
 *
 * Returns the total number of metadata bytes including the number of bytes
 * used for addressing and error detection (e.g. overhead in bytes).
 *
 * @param ptr snap_t instance pointer
 */
extern size_t snap_get_metadata_size(snap_t *ptr);


/**
 * @brief
 *
 * @param ptr snap_t instance pointer
 */
extern bool snap_get_acknowledgement(snap_t *ptr);


/**
 * @brief
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_set_acknowledgement(snap_t *ptr, bool enable_ack);


/**
 * @brief
 *
 * @param ptr snap_t instance pointer
 */
extern size_t snap_get_error_detection_mode_size(snap_t *ptr);


/**
 * @brief
 *
 * @param ptr snap_t instance pointer
 */
extern SnapErrorDetectionMode snap_get_error_detection_mode(snap_t *ptr);


/**
 * @brief
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_set_error_detection_mode(snap_t *ptr, SnapErrorDetectionMode edm);


#if 0
extern snap_edm_handler* snap_get_user_error_detection_handler(snap_t *ptr);
extern void snap_set_user_error_detection_handler(snap_t *ptr, snap_edm_handler *edm);

extern SnapCompressMode snap_get_compress_mode(snap_t *ptr);
extern void snap_set_compress_mode(snap_t *ptr, SnapCompressMode cmode);

extern snap_compress_handler* snap_get_user_compress_handler(snap_t *ptr);
extern void snap_set_user_compress_handler(snap_t *ptr, snap_compress_handler *edm);

extern void snap_set_maximum_content_size(snap_t *ptr, uint16_t size);
extern uint16_t snap_get_maximum_content_size(snap_t *ptr);

extern void snap_set_minimum_content_size(snap_t *ptr, uint16_t size);
extern uint16_t snap_set_minimum_content_size(snap_t *ptr);

typedef bool (*snapPacketFilterFunction)(const snap_t *snap, const snap_packet_t *packet) snapPacketFilterFunction;
extern bool snap_has_packet_filter(snap_t *ptr);
extern void snap_set_packet_filter(snap_t *ptr, snapPacketFilterFunction);
extern snapPacketFilterFunction snap_get_packet_filter(snap_t *ptr);
extern void snap_clear_packet_filter(snap_t *ptr);

extern bool snap_register_command(snap_t *snap, uint8_t commandNumber, CommandFunctionPointer ptr, bool force);
extern bool snap_remove_command(snap_t *snap, uint8_t commandNumber);
extern bool snap_encode_command(snap_t *snap, uint8_t commandNumber);

extern bool snap_has_local_address(snap_t *snap);
extern bool snap_has_peer_address(snap_t *snap);

extern bool snap_is_local_address_broadcast(snap_t *snap);
extern bool snap_is_peer_address_broadcast(snap_t *snap);

extern void snap_set_local_address_broadcast(snap_t *snap);
extern void snap_set_peer_address_broadcast(snap_t *snap);
#endif


/**
 * @brief Returns the number of bytes used for the source address
 *
 * @param ptr snap_t instance pointer
 */
extern size_t snap_get_local_address_size(snap_t *ptr);


/**
 * @brief Returns the local address
 *
 * @param ptr snap_t instance pointer
 */
extern uint32_t snap_get_local_address(snap_t *ptr);


/**
 * @brief Sets the local address
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_set_local_address(snap_t *ptr, uint32_t address);


/**
 * @brief Returns the number of bytes used for the peer address
 *
 * @param ptr snap_t instance pointer
 */
extern size_t snap_get_peer_address_size(snap_t *ptr);


/**
 * @brief Returns the address of the peer
 *
 * @param ptr snap_t instance pointer
 */
extern uint32_t snap_get_peer_address(snap_t *ptr);


/**
 * @brief Sets the address of the peer
 *
 * @param ptr snap_t instance pointer
 */
extern void snap_set_peer_address(snap_t *ptr, uint32_t address);


/**
 * @brief Returns the header for the given amount of data
 *
 * @see snap_get_hdb1
 * @see snap_get_hdb2
 *
 * @param ptr snap_t instance pointer
 */
extern uint16_t snap_get_header(snap_t *ptr, size_t in_data_size);


/**
 * @brief Returns the 'header byte 1' for the given amount of data
 *
 * @param ptr snap_t instance pointer
 */
extern uint8_t snap_get_hdb1(snap_t *ptr, size_t in_data_size);


/**
 * @brief Returns the 'header byte 2' for the given amount of data
 *
 * @param ptr snap_t instance pointer
 */
extern uint8_t snap_get_hdb2(snap_t *ptr, size_t in_data_size);


/**
 * @brief Calculate output buffer size for snap_encode()
 *
 * This function calculates how much extra output buffer space is
 * required to be sure that snap_encode() doesn't fail.
 *
 * @see snap_encode
 *
 * @code
 * uint8_t *buffer = malloc( snap_encode_bound( snap, in_data_size ) );
 * @endcode
 *
 * @note The calculated value is not exact, but it is guaranteed to be big enough.
 *
 * @param ptr snap_t instance pointer
 * @param in_data_size
 */
extern size_t snap_encode_bound(snap_t *ptr, size_t in_data_size);


/**
 * @brief encode raw in_data as S.N.A.P data stream to out_data
 *
 * The maximum required output buffer size can be calculated with
 * snap_encode_bound().
 *
 * @see snap_encode_bound
 *
 * @param ptr snap_t instance pointer
 * @param in_data
 * @param in_data_size
 * @param out_data      Beginning of the output buffer
 * @param out_data_pos  The next byte will be written to out[*out_data_pos].
 *                      *out_data_pos is updated only if encoding succeeds.
 *                      when snap_encode returns, the first byte into which no
 *                      data is written to is out_data[*out_data_pos].
 * @param out_data_size Size of the out buffer.
 *
 * @return Returns true if the encoding was successful; otherwise returns false.
 */
extern bool snap_encode(snap_t *ptr, const uint8_t *in_data, size_t in_data_size,
                 uint8_t *out_data, size_t *out_data_pos, size_t out_data_size);



/**
 * @brief Calculate the output buffer size for the result of snap_decode().
 *
 * This function calculates how much extra output buffer space is
 * required to be sure that snap_decode() doesn't fail.
 *
 * @see snap_encode
 *
 * @code
 * uint8_t *buffer = malloc( snap_decode_result_bound( snap, in_data_size ) );
 * @endcode
 *
 * @note The calculated value is not exact, but it is guaranteed to be big enough.
 *
 * @param ptr snap_t instance pointer
 * @param in_data_size
 */
extern size_t snap_decode_result_bound(snap_t *ptr, size_t in_data_size);


/**
 * @brief Calculate the output buffer size for the response by snap_decode().
 *
 * This function calculates how much extra output buffer space is
 * required to be sure that snap_decode() doesn't fail.
 *
 * @see snap_encode
 *
 * @code
 * uint8_t *buffer = malloc( snap_encode_bound( snap, in_data_size ) );
 * @endcode
 *
 * @note The calculated value is not exact, but it is guaranteed to be big enough.
 *
 * @param ptr snap_t instance pointer
 * @param in_data_size
 */
extern size_t snap_decode_response_bound(snap_t *ptr, size_t in_data_size);


/**
 * @brief decode S.N.A.P data stream from in_data into result and response
 *
 * The maximum required output buffer sizes can be calculated with
 * snap_decode_result_bound() and snap_decode_response_bound().
 *
 * @see snap_decode_response_bound
 * @see snap_decode_result_bound
 *
 * @param ptr snap_t instance pointer
 * @param in_data
 * @param in_data_size
 * @param result
 * @param result_pos
 * @param result_pos
 * @param result_size
 * @param response
 * @param response_pos
 * @param response_size
 *
 * @return Returns true if the decoding was successful; otherwise returns false.
 */
extern bool snap_decode(snap_t *snap,
           const uint8_t *in_data, size_t *in_data_pos, size_t in_data_size,
                 uint8_t *result, size_t *result_pos, size_t result_size,
                 uint8_t *response, size_t *response_pos, size_t response_size);



#ifdef __cplusplus
}   // end extern "C"
#endif


#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
