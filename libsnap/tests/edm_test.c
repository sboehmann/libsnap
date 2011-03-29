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
#include <CUnit/CUnit.h>

#include <libsnap/edm.h>

/**
 * @file libsnap/tests/edm_test.c
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap edm unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


void snap_test_checksum()
{
    uint8_t data1[] = { 'S','N','A','P' };
    CU_ASSERT( snap_checksum( data1, 4 ) == 0x32 );
    CU_ASSERT( snap_checksum_is_valid( data1, 4, 0x32 ) );

    uint8_t data2[] = { 's','n','a','p' };
    CU_ASSERT( snap_checksum( data2, 4 ) == 0xB2 );
    CU_ASSERT( snap_checksum_is_valid( data2, 4, 0xB2 ) );

    uint8_t data3[] = { 'S','n','a','p' };
    CU_ASSERT_FALSE( snap_checksum( data3, 4 ) == 0x32 );
    CU_ASSERT_FALSE( snap_checksum_is_valid( data3, 4, 0xB2 ) );
}


void snap_test_crc8()
{
    uint8_t data1[] = { 'S','N','A','P' };
    CU_ASSERT( snap_crc8( data1, 4 ) == 0x11 );
    CU_ASSERT( snap_crc8_is_valid( data1, 4, 0x11 ) );

    uint8_t data2[] = { 's','n','a','p' };
    CU_ASSERT( snap_crc8( data2, 4 ) == 0x17 );
    CU_ASSERT( snap_crc8_is_valid( data2, 4, 0x17 ) );

    uint8_t data3[] = { 'S','n','a','p' };
    CU_ASSERT_FALSE( snap_crc8( data3, 4 ) == 0x11 );
    CU_ASSERT_FALSE( snap_crc8_is_valid( data3, 4, 0x17 ) );
}


void snap_test_crc16()
{
    uint8_t data1[] = { 'S','N','A','P' };
    CU_ASSERT( snap_crc16( data1, 4 ) == 0x8C43 );
    CU_ASSERT( snap_crc16_is_valid( data1, 4, 0x8C43 ) );

    uint8_t data2[] = { 's','n','a','p' };
    CU_ASSERT( snap_crc16( data2, 4 ) == 0x1F4F );
    CU_ASSERT( snap_crc16_is_valid( data2, 4, 0x1F4F ) );

    uint8_t data3[] = { 'S','n','a','p' };
    CU_ASSERT_FALSE( snap_crc16( data3, 4 ) == 0x8C43 );
    CU_ASSERT_FALSE( snap_crc16_is_valid( data3, 4, 0x1F4F ) );
}


void snap_test_crc32()
{
    uint8_t data1[] = { 'S','N','A','P' };
    CU_ASSERT( snap_crc32( data1, 4 ) == 0x00F1F02A );
    CU_ASSERT( snap_crc32_is_valid( data1, 4, 0x00F1F02A ) );

    uint8_t data2[] = { 's','n','a','p' };
    CU_ASSERT( snap_crc32( data2, 4 ) == 0x36641D9E );
    CU_ASSERT( snap_crc32_is_valid( data2, 4, 0x36641D9E ) );

    uint8_t data3[] = { 'S','n','a','p' };
    CU_ASSERT_FALSE( snap_crc32( data3, 4 ) == 0x00F1F02A );
    CU_ASSERT_FALSE( snap_crc32_is_valid( data3, 4, 0x36641D9E ) );
}


void snap_test_adler32()
{
    uint8_t data1[] = { 'S','N','A','P' };
    CU_ASSERT( snap_adler32( data1, 4 ) == 0x030C0133 );
    CU_ASSERT( snap_adler32_is_valid( data1, 4, 0x030C0133 ) );

    uint8_t data2[] = { 's','n','a','p' };
    CU_ASSERT( snap_adler32( data2, 4 ) == 0x044C01B3 );
    CU_ASSERT( snap_adler32_is_valid( data2, 4, 0x044C01B3 ) );

    uint8_t data3[] = { 'S','n','a','p' };
    CU_ASSERT_FALSE( snap_adler32( data3, 4 ) == 0x03CC0132 );
    CU_ASSERT_FALSE( snap_adler32_is_valid( data3, 4, 0x03CC0112 ) );
}


char test_suite_name[] = "Error Detection Mode";

void test_suite(CU_pSuite suite)
{
    CU_add_test( suite, "test of 8 bit checksum", snap_test_checksum );
    CU_add_test( suite, "test of 8 bit CRC     ", snap_test_crc8 );
    CU_add_test( suite, "test of 16 bit CRC    ", snap_test_crc16 );
    CU_add_test( suite, "test of 32 bit CRC    ", snap_test_crc32 );
    CU_add_test( suite, "test of Adler32       ", snap_test_adler32 );
}



// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
