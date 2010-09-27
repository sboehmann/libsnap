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
#ifndef LIBSNAP_UTIL_H
#define LIBSNAP_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libsnap/global.h>


/**
 * @file libsnap/util.h
 * @ingroup libsnap
 * @brief
 *
 *
 * @version 0.1
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


/**
 * @brief utility function to allocate memory
 */
extern void* snap_util_alloc(snap_allocator_t *allocator, size_t size);


/**
 * @brief utility function to free memory
 */
extern void snap_util_free(snap_allocator_t *allocator, void *ptr);


extern bool snap_util_test_bit(uint8_t data, uint8_t bit);
extern uint8_t snap_util_set_bit(uint8_t data, uint8_t bit);
extern uint8_t snap_util_clear_bit(uint8_t data, uint8_t bit);

extern bool snap_util_test_bit16(uint16_t data, uint8_t bit);
extern uint16_t snap_util_set_bit16(uint16_t data, uint8_t bit);
extern uint16_t snap_util_clear_bit16(uint16_t data, uint8_t bit);

extern bool snap_util_test_bit32(uint32_t data, uint8_t bit);
extern uint32_t snap_util_set_bit32(uint32_t data, uint8_t bit);
extern uint32_t snap_util_clear_bit32(uint32_t data, uint8_t bit);

extern bool snap_util_test_bit64(uint64_t data, uint8_t bit);
extern uint64_t snap_util_set_bit64(uint64_t data, uint8_t bit);
extern uint64_t snap_util_clear_bit64(uint64_t data, uint8_t bit);

extern void* snap_util_memcpy(void *dest, const void *src, size_t n);

extern uint16_t snap_util_get_ndb_total_bytes(size_t in_data_size);
extern uint16_t snap_util_get_ndb_fill_bytes(size_t in_data_size);

#if 0 //write a unit test before using this function!
extern size_t snap_util_get_max_count_fill_bytes(size_t in_data_size);
#endif

extern uint8_t snap_util_map_ndb_to_value(size_t size);
extern uint16_t snap_util_map_value_to_ndb(uint8_t value);

extern uint8_t snap_util_get_sync_byte(void);
extern uint8_t snap_util_get_fill_byte(void);


extern size_t snap_util_ack_response_bound(uint32_t local_address, uint32_t peer_address);
extern bool snap_util_create_ack_response(uint32_t local_address, uint32_t peer_address, uint8_t *data, size_t *pos, size_t size);

extern size_t snap_util_nak_response_bound(uint32_t local_address, uint32_t peer_address);
extern bool snap_util_create_nak_response(uint32_t local_address, uint32_t peer_address, uint8_t *data, size_t *pos, size_t size);

extern size_t snap_util_get_address_size(uint32_t address);
extern size_t snap_util_get_edm_size(SnapErrorDetectionMode edm);
extern uint8_t snap_util_get_edm_size_value(SnapErrorDetectionMode edm);

extern uint32_t snap_util_get_version(void);


#ifdef __cplusplus
}   // end extern "C"
#endif

#endif

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

