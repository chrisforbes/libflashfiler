#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

void ff_dumpstats( struct ff_db * db )
{
	struct ff_header_file const * h = db->base;
//	struct ff_header_data_block const * b = ff_getblockptr( db, h->firstDataBlock );
	printf( "-- records: %u fields: %u indexes: %u blocksize: 0x%x\n",
			h->numRecords,
			h->numFields,
			h->numIndexes,
			h->blockSize);

/*	while( b ) {
		printf( "walking data block: %u with %u records\n", b->h.thisBlock, b->numRecords );
		b = ff_getblockptr( db, b->nextBlock );
	}	*/
}