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
/** @defgroup csnap csnap, the S.N.A.P command line helper */

#define _POSIX_SOURCE

#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>


#include <libsnap/global.h>
#include <libsnap/snap.h>
#include <libsnap/util.h>

#include "color.h"
#include "util.h"
#include "message.h"



typedef enum
{
    NONE,
    ENCODE,
    DECODE
} Mode;


typedef struct
{
    FILE *in;
    FILE *out;

    char *in_file;

    Mode mode;
    uint16_t size;

    SnapErrorDetectionMode error_detection_mode;
    bool acknowledgement_mode;
    bool hex;

    int32_t local_address;
    int32_t peer_address;
} args_info;



void parse_command_line(args_info *args, int argc, char **argv)
{
    args->in = NULL;
    args->out = stdout;
    args->in_file = NULL;
    args->mode = NONE;
    args->size = 64;
    args->error_detection_mode = EDM_NONE;
    args->acknowledgement_mode = false;
    args->hex = false;
    args->local_address = 0;
    args->peer_address = 0;

    static const char short_opts[] = "edaAE:s:l:p:ChoVH";

    static const struct option long_options[] =
    {
        // Operation mode
        { "encode",   no_argument, NULL, 'e' },
        { "decode",   no_argument, NULL, 'd' },
        { "ack",      no_argument, NULL, 'a' },
        { "noack",    no_argument, NULL, 'A' },
        { "hex",      no_argument, NULL, 'H' },

        { "edm",      required_argument, NULL, 'E' },
        { "size",     required_argument, NULL, 's' },

        { "local", required_argument, NULL, 'l' },
        { "peer", required_argument,  NULL, 'p' },

        // Other options
        { "nocolor",  no_argument, NULL, 'C' },
        { "help",     no_argument, NULL, 'h' },
        { "author",   no_argument, NULL, 'o' },
        { "version",  no_argument, NULL, 'V' },
        { NULL,                0, NULL, 0   }
    };

    while( optind < argc )
    {
        int index = -1;
        const int result = getopt_long(argc, argv, short_opts, long_options, &index);

        if( result == -1 ) {
            break; /* end of list */
        }

        switch( result ) {
        case 'e':
            args->mode = ENCODE;
            break;

        case 'd':
            args->mode = DECODE;
            break;

        case 'a':
            args->acknowledgement_mode = true;
            break;

        case 'A':
            args->acknowledgement_mode = false;
            break;

        case 'H':
            args->hex = true;
            break;

        case 'E': //edm
            if( strcmp( optarg, "none" ) == 0 || strcmp( optarg, "None" ) == 0 ||
                strcmp( optarg, "No" ) == 0 || strcmp( optarg, "no" ) == 0 )
            {
                args->error_detection_mode = EDM_NONE;
            }
            else if( strcmp( optarg, "3times" ) == 0 ) {
                args->error_detection_mode = EDM_THREETIMES;
            }
            else if( strcmp( optarg, "checksum" ) == 0 ) {
                args->error_detection_mode = EDM_CHECKSUM;
            }
            else if( strcmp( optarg, "crc8" ) == 0 || strcmp( optarg, "8" ) == 0 ) {
                args->error_detection_mode = EDM_CRC8;
            }
            else if( strcmp( optarg, "crc16" ) == 0 || strcmp( optarg, "16" ) == 0 ) {
                args->error_detection_mode = EDM_CRC16;
            }
            else if( strcmp( optarg, "crc32" ) == 0 || strcmp( optarg, "32" ) == 0 ) {
                args->error_detection_mode = EDM_CRC32;
            }
            else if( strcmp( optarg, "fec" ) == 0 ) {
                args->error_detection_mode = EDM_FEC;
            }
            else {
                printf( "%s: option --edm: invalid choice: '%s' ", argv[ 0 ], optarg );
                printf( "(choose from 'none', '3times', 'checksum', 'crc8', 'crc16', 'crc32', 'fec')\n" );
                printf( "Usage: %s <options> <file>\n", argv[ 0 ] );
                printf( "Try `%s --help' for more information.\n", argv[ 0 ] );
                exit( EXIT_FAILURE );
            }
            break;

        case 's': //packet size
            {
                uint32_t result = 0;
                if( str_to_uint32( optarg, 1, 512, &result ) ) {
                    args->size = result;
                }
                else {
                    printf( "%s: option --size: invalid value: '%s' ", argv[ 0 ], optarg );
                    printf( "Usage: %s <options> <file>\n", argv[ 0 ] );
                    printf( "Try `%s --help' for more information.\n", argv[ 0 ] );
                    exit( EXIT_FAILURE );
                }
            }
            break;

        case 'l':
            {
                uint32_t result = 0;
                if( str_to_uint32( optarg, 1, 16777216, &result ) ) {
                    args->local_address = result;
                }
                else {
                    printf( "%s: option --local: invalid value: '%s' ", argv[ 0 ], optarg );
                    printf( "Usage: %s <options> <file>\n", argv[ 0 ] );
                    printf( "Try `%s --help' for more information.\n", argv[ 0 ] );
                    exit( EXIT_FAILURE );
                }
            }
            break;

        case 'p':
            {
                uint32_t result = 0;
                if( str_to_uint32( optarg, 1, 16777216, &result ) ) {
                    args->peer_address = result;
                }
                else {
                    printf( "%s: option --peer: invalid value: '%s' ", argv[ 0 ], optarg );
                    printf( "Usage: %s <options> <file>\n", argv[ 0 ] );
                    printf( "Try `%s --help' for more information.\n", argv[ 0 ] );
                    exit( EXIT_FAILURE );
                }
            }
            break;

        case 'C':
            color_print_enable_colors = false;
            break;

        case 'o':
            message_author( argv[ 0 ] );
            exit( EXIT_SUCCESS );

        case 'h':
            message_help( argv[ 0 ] );
            exit( EXIT_SUCCESS );

        case 'V':
            message_version( argv[ 0 ] );
            exit( EXIT_SUCCESS );

        default:
            exit( EXIT_FAILURE );
        }
    }

    if( optind < argc ) {
        if( optind < argc -1 ) {
            printf( "%s: too many arguments\n", argv[ 0 ] );
            printf( "Usage: %s <options> <file>\n", argv[ 0 ] );
            printf( "Try `%s --help' for more information.\n", argv[ 0 ] );
            exit( EXIT_FAILURE );
        }

        if( strcmp( argv[ optind ], "-" ) == 0 ) {
            args->in_file = NULL;
        }
        else {
            args->in_file = argv[ optind ];
        }
    }
}


int encode(snap_t *snap, args_info *args)
{
    size_t count_in = 0;
    size_t count_out = 0;
    size_t packet_count = 0;

    uint8_t data[ 512 ];
    size_t pos = 0;

    while( !feof( args->in ) ) {
        data[ pos ] = fgetc( args->in );

        if( !feof( args->in ) ) {
            count_in++;
            pos++;
        }

        if( feof( args->in ) || pos >= ( args->size > 0 ? args->size : 512 ) ) {
            size_t out_size = snap_encode_bound( snap, pos );
            uint8_t *out = malloc( out_size );
            size_t out_pos = 0;

            packet_count++;

            if( !snap_encode( snap, data, pos, out, &out_pos, out_size ) ) {
                fprintf( stderr, "csnap: snap_encode error" );
                exit( EXIT_FAILURE );
            }

            for(size_t i = 0; i < out_pos; ++i) {
                if( isatty( fileno( args->out ) ) ) {
                    char *color = COLOR_NORMAL;

                    if( i <= 2 ) {
                        color = COLOR_BOLD_GREEN;
                    }
                    else if( i < 3 + snap_get_local_address_size( snap )
                                   + snap_get_peer_address_size( snap ) )
                    { // TODO local address
                        color = COLOR_BOLD_GREEN;
                    }
                    else if( false ) { // TODO colour edm bytes
                        color = COLOR_BOLD_YELLOW;
                    }
                    else if( false ) { // TODO colour fill bytes
                        color = COLOR_BOLD_MAGENTA;
                    }

                    if( args->hex ) {
                        color_fprintf( args->out, color, "0x%02X ", out[ i ] );
                    }
                    else if( isprint( out[ i ] ) ) {
                        color_fprintf( args->out, color , "%c", out[ i ] );
                    }
                    else {
                        color_fprintf( args->out, color , "." );
                    }
                }
                else {
                    if( args->hex ) {
                        fprintf( args->out, "0x%02X", out[ i ] );
                        if( i < out_pos - 2 ) {
                            fprintf( args->out, ", " );
                        }
                    }
                    else {
                        fputc( out[ i ], args->out );
                    }
                }

                count_out++;
            }

            fflush( args->out );
            pos = 0;
        }
    }

    count_out -= count_out > 0 ? 1 : 0;

    if( args->in_file != NULL ) {
        fclose( args->in );
    }

    if( isatty( fileno( args->out ) ) ) {
        fprintf( stderr, "\n" );
    }

    if( count_in / 1024.0 / 1024.0 > 1 ) {
        color_fprintf( stderr, COLOR_BOLD,
            " %s: %.1fM (%d) read, %.1fM (%d) written in %d packets (+%.2f%%)\n",
            args->in_file, count_in / 1024.0 / 1024.0, count_in,
            count_out / 1024.0 / 1024.0, count_out, packet_count,
            ( 100.0 / count_in * count_out ) - 100.0 );
    }
    else if( count_in / 1024.0 > 1 ) {
        color_fprintf( stderr, COLOR_BOLD,
            " %s: %.1fK (%d) read, %.1fK (%d) written in %d packets (+%.2f%%)\n",
            args->in_file, count_in / 1024.0, count_in, count_out / 1024.0,
            packet_count, count_out, ( 100.0 / count_in * count_out ) - 100.0 );
    }
    else {
        color_fprintf( stderr, COLOR_BOLD,
            " %s: %d byte read, %d byte written in %d packets (+%.2f%%)\n",
            args->in_file, count_in, count_out, packet_count,
            ( 100.0 / count_in * count_out ) - 100.0 );
    }

    return EXIT_FAILURE;
}


int decode(snap_t *snap, args_info *args)
{
    size_t in_size;
    uint8_t *data = fread_file( args->in, &in_size );
    size_t in_data_pos = 0;

    size_t out_size = 0;
    size_t packet_count = 0;
    
    while( in_data_pos < in_size ) {

        snap_reset( snap );
        snap_set_local_address( snap, args->local_address );
        snap_set_peer_address( snap, args->peer_address );

        size_t result_size = snap_decode_result_bound( snap, in_size );
        uint8_t *result = malloc( result_size );
        size_t result_pos = 0;

        size_t response_size = snap_decode_response_bound( snap, in_size );
        uint8_t *response = malloc( response_size );
        size_t response_pos = 0;

        if( !snap_decode( snap, data, &in_data_pos, in_size, result,
                            &result_pos, result_size, response,
                            &response_pos, response_size ) )
        {
            fprintf( stderr, "csnap: snap_decode error" );
            exit( EXIT_FAILURE );
        }

        packet_count++;

        for(size_t i = 0; i < result_pos; ++i) {
            fputc( result[ i ], args->out );
            out_size++;
        }

        fflush( args->out );

        free( result );
        free( response );
    }

    packet_count -= packet_count > 0 ? 1 : 0;
    
    free( data );

    if( args->in_file != NULL ) {
        fclose( args->in );
    }

    if( isatty( fileno( args->out ) ) ) {
        fprintf( stderr, "\n" );
    }

    if( out_size / 1024.0 / 1024.0 > 1 ) {
        color_fprintf( stderr, COLOR_BOLD,
            " %s: %.1fM (%d) read (%d packets), %.1fM (%d) written.\n",
            args->in_file, in_size / 1024.0 / 1024.0, in_size, packet_count,
            out_size / 1024.0 / 1024.0, out_size 
        );
    }
    else if( out_size / 1024.0 > 1 ) {
        color_fprintf( stderr, COLOR_BOLD,
            " %s: %.1fK (%d) read (%d packets), %.1fK (%d) written.\n",
            args->in_file, in_size / 1024.0, in_size, packet_count,
            out_size / 1024.0, out_size 
        );
    }
    else {
        color_fprintf( stderr, COLOR_BOLD,
            " %s: %d byte read (%d packets), %d byte written.\n",
            args->in_file, in_size, packet_count, out_size 
        );
    }
    return EXIT_FAILURE;
}



int main(int argc, char **argv)
{
    args_info args;
    parse_command_line( &args, argc, argv );

    snap_t snap;
    snap_init( &snap );

    snap_set_acknowledgement( &snap, args.acknowledgement_mode );
    snap_set_error_detection_mode( &snap, args.error_detection_mode );
    snap_set_local_address( &snap, args.local_address );
    snap_set_peer_address( &snap,  args.peer_address );


    if( args.in_file == NULL ) {
        args.in = stdin;

        //TODO temp file
        args.out = stdout;
    }
    else {
        if( is_dir( args.in_file ) ) {
            fprintf( stderr, "%s: %s: Is a directory\n", argv[ 0 ], args.in_file );
            exit( EXIT_FAILURE );
        }

        if( access( args.in_file, F_OK | R_OK ) == 0 ) {
            args.in = fopen( args.in_file, "r" );

            if( args.in == NULL ) {
                perror( "fopen" );
                exit( EXIT_FAILURE );
            }
        }
        else {
            perror( args.in_file );
            exit( EXIT_FAILURE );
        }
    }

    int result;
    switch( args.mode ) {
        case ENCODE:
            result = encode( &snap, &args );
            break;
        case DECODE:
            result = decode( &snap, &args );
            break;
        default:
            printf( "Usage: %s <options> <file>\n", argv[ 0 ] );
            printf( "Try `%s --help' for more information.\n", argv[ 0 ] );
            exit( EXIT_FAILURE );
    }

    return result;
}


// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:

