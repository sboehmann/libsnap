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

#include <CUnit/Basic.h>

/**
 * @file libsnap/tests/main.c
 *
 * @ingroup libsnap
 * @defgroup tests libsnap unit tests
 *
 * @ingroup libsnap
 * @ingroup tests
 *
 * @brief libsnap unit tests
 *
 * @author Stefan Böhmann <kde@hilefoks.org>
 */


extern char test_suite_name[];

extern void test_suite();


int main()
{
    if( CUE_SUCCESS == CU_initialize_registry() ) {
        CU_pSuite suite = CU_add_suite( test_suite_name, 0, 0 );
        if( suite != NULL ) {
            test_suite( suite );

            CU_basic_set_mode( CU_BRM_VERBOSE );
            CU_basic_run_tests();

            if( CU_get_number_of_tests_failed() <= 0 ) {
                CU_cleanup_registry();
                return EXIT_SUCCESS;
            }
        }
    }

    return EXIT_FAILURE;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
