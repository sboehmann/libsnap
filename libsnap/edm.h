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
#ifndef LIBSNAP_EDM_H
#define LIBSNAP_EDM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


/**
 * @file libsnap/edm.h
 * @ingroup libsnap
 * @brief Functions used to compute the values for error detection mode
 *
 * @section Checksum 8-bit checksum
 *
 * The S.N.A.P variant for 8-bit checksums are a mix of the
 * "longitudinal parity check" and the "modular sum" method.
 *
 * @section CRC CRC - Cyclic Redundancy Check
 * TODO
 *
 * @section FEC FEC - Forward Error Correction
 * TODO
 *
 * @version 0.1
 * @author Stefan Böhmann <kde@hilefoks.org>
 */



/**
 * @brief Return value if snap_fec_decode detects no errors
 */
#define SNAP_FEC_ERROR_NONE 0

/**
 * @brief Return value if snap_fec_decode is able to fix all errors
 */
#define SNAP_FEC_ERROR_CORRECTED 1

/**
 * @brief Return value if snap_fec_decode cannot fix all errors
 */
#define SNAP_FEC_ERROR_FATAL 2


/**
 * @brief Calculate the 8 bit checksum of data that is size bytes long.
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 *
 * @return 8 bit checksum of data[0 .. size]
 */
extern uint8_t snap_checksum(const uint8_t *data, size_t size);

/**
 * @brief Returns true if the given 8-bit checksum match
 *
 * @param data     Pointer to data buffer.
 * @param size     Number of bytes in data buffer
 * @param checksum The assumed checksum result
 *
 * @return Returns true if the checksum match; otherwise returns false.
 */
extern bool snap_checksum_is_valid(const uint8_t *data, size_t size, uint8_t checksum);

/**
 * @brief Calculate the CRC-8 of data that is size bytes long.
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 *
 * @return CRC-8 of data[0 .. size]
 */
extern uint8_t snap_crc8(const uint8_t *data, size_t size);

/**
 * @brief Returns true if the given crc match
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 * @param crc       The assumed crc result
 *
 * @return Returns true if the crc match; otherwise returns false.
 */
extern bool snap_crc8_is_valid(const uint8_t *data, size_t size, uint8_t crc);

/**
 * @brief Calculate the CRC-16 of data that is size bytes long.
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 *
 * @return CRC-16 of data[0 .. size]
 */
extern uint16_t snap_crc16(const uint8_t *data, size_t size);

/**
 * @brief Returns true if the given crc match
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 * @param crc       The assumed crc result
 *
 * @return Returns true if the crc match; otherwise returns false.
 */
extern bool snap_crc16_is_valid(const uint8_t *data, size_t size, uint16_t crc);

/**
 * @brief Calculate the CRC-32 of data that is size bytes long.
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 *
 * @return CRC-32 of data[0 .. size]
 */
extern uint32_t snap_crc32(const uint8_t *data, size_t size);

/**
 * @brief Returns true if the given crc match
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 * @param crc       The assumed crc result
 *
 * @return Returns true if the crc match; otherwise returns false.
 */
extern bool snap_crc32_is_valid(const uint8_t *data, size_t size, uint32_t crc);

/**
 * @brief Calculate the Adler32 of data that is size bytes long.
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 *
 * @return Adler32 of data[0 .. size]
 */
extern uint32_t snap_adler32(const uint8_t *data, size_t size);

/**
 * @brief Returns true if the given crc match
 *
 * @param data      Pointer to data buffer.
 * @param size      Number of bytes in data buffer
 * @param crc       The assumed crc result
 *
 * @return Returns true if the crc match; otherwise returns false.
 */
extern bool snap_adler32_is_valid(const uint8_t *data, size_t size, uint32_t crc);


#ifdef __cplusplus
}   // end extern "C"
#endif

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
