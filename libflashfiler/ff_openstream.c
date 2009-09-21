#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

struct ff_stream * ff_openstream( struct ff_db * db, DWORD stream )
{
	struct ff_header_stream_block const * b = ff_getblockptr( db, stream );
	struct ff_stream * s = malloc( sizeof( struct ff_stream ) );

	s->db = db;
	s->cursor = 0;
	s->length = b->streamLength;
	s->firstBlock = stream;
	s->curBlock = s->firstBlock;
	s->blockCursor = 0;

	return s;
}