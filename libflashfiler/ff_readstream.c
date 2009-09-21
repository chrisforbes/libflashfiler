#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

static int bytes_remaining_in_block( struct ff_stream * s )
{
	struct ff_header_file const * h = s->db->base;
	size_t sbsize = h->blockSize - sizeof( struct ff_header_stream_block );

	return sbsize - s->blockCursor;
}

static void * get_streamptr( struct ff_stream * s )
{
	struct ff_header_stream_block * sb = ff_getblockptr( s->db, s->curBlock );
	return (char *)(sb+1) + s->blockCursor;
}

int ff_readstream( struct ff_stream * s, void * buf, int len )
{
	int l = len = min( len, (int)(s->length - s->cursor) );

	while( len )
	{
		int chunk = min( bytes_remaining_in_block( s ), len );
		if (chunk)
		{
			memcpy( buf, get_streamptr( s ), chunk );
			buf = (char *)buf + chunk;
			len -= chunk;
			s->cursor += chunk;
			s->blockCursor += chunk;
		}

		if (len)
		{
			struct ff_header_stream_block * sb = ff_getblockptr( s->db, s->curBlock );
			s->curBlock = sb->nextBlock;
			s->blockCursor = 0;
		}
	}

	return l;
}

void ff_seekstream( struct ff_stream * s, int d )
{
	if (d < 0)
		error( "ff_seekstream: reverse seek not supported yet", cleanup_none );

	while( d )
	{
		int chunk = min( bytes_remaining_in_block( s ), d );
		d -= chunk;
		s->cursor += chunk;
		s->blockCursor += chunk;

		if (d)
		{
			struct ff_header_stream_block * sb = ff_getblockptr( s->db, s->curBlock );
			s->curBlock = sb->nextBlock;
			s->blockCursor = 0;
		}
	}

cleanup_none:;
}