#include <libsnap/snap.h>

int main(int argc, char **argv)
{
	snap_t snap;
	snap_init( &snap );

	char in[] = "If a packet hits a pocket on a socket on a port, "
		"and the bus is interrupted and the interrupt's not caught, "
		"then the socket packet pocket has an error to report.";

	size_t in_size = strlen( in );

	size_t out_size = snap_encode_bound( &snap, in_size );
	uint8_t *out = malloc( out_size );
	size_t out_pos = 0;

	snap_encode( &snap, (uint8_t*) in, in_size,
				 out, &out_pos, out_size );
}