#include <libsnap/snap.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    SNAP_UNUSED( argc );
    SNAP_UNUSED( argv );

    snap_t snap;
    snap_init( &snap );

    uint8_t in[] = {
        'I', 'f', ' ', 'a', ' ', 'p', 'a', 'c', 'k', 'e', 't', ' ', 'h', 'i',
        't', 's', ' ', 'a', ' ', 'p', 'o', 'c', 'k', 'e', 't', ' ', 'o', 'n',
        ' ', 'a', ' ', 's', 'o', 'c', 'k', 'e', 't', ' ', 'o', 'n', ' ', 'a',
        ' ', 'p', 'o', 'r', 't', ',', ' ', 'a', 'n', 'd', ' ', 't', 'h', 'e',
        ' ', 'b', 'u', 's', ' ', 'i', 's', ' ', 'i', 'n', 't', 'e', 'r', 'r',
        'u', 'p', 't', 'e', 'd', ' ', 'a', 'n', 'd', ' ', 't', 'h', 'e', ' ',
        'i', 'n', 't', 'e', 'r', 'r', 'u', 'p', 't', '\'', 's', ' ', 'n', 'o',
        't', ' ', 'c', 'a', 'u', 'g', 'h', 't', ',', ' ', 't', 'h', 'e', 'n',
        ' ', 't', 'h', 'e', ' ', 's', 'o', 'c', 'k', 'e', 't', ' ', 'p', 'a',
        'c', 'k', 'e', 't', ' ', 'p', 'o', 'c', 'k', 'e', 't', ' ', 'h', 'a',
        's', ' ', 'a', 'n', ' ', 'e', 'r', 'r', 'o', 'r', ' ', 't', 'o', ' ',
        'r', 'e', 'p', 'o', 'r', 't', '.'
    };

    size_t in_size = sizeof( in ) / sizeof( in[ 0 ] );

    size_t out_size = snap_encode_bound( &snap, in_size );
    uint8_t *out = malloc( out_size );
    size_t out_pos = 0;

    snap_encode( &snap, in, in_size, out, &out_pos, out_size );

    out_pos--; // do not point to the next unwritten byte, do point to the last written one.

    printf( "Input:" );
    for(size_t i = 0; i < in_size; ++i) {
        if( i % 75 == 0 ) {
            printf( "\n    " );
        }
        printf( "%c", in[ i ] );
    }

    printf( "\n\nOutput:" );
    for(size_t i = 0; i < out_pos; ++i) {
        if( i % 12 == 0 ) {
            printf( "\n    " );
        }
        printf( "0x%02X", out[ i ] );
        if( i < out_pos - 1 ) {
            printf( ", " );
        }
    }
    printf( "\n" );
}

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim: set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
