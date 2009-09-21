#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

void * ff_getblockptr( struct ff_db * db, DWORD block )
{
	if (block == -1) return 0;

	{
		struct ff_header_file const * h = db->base;
		char * p = db->base;
		return p + h->blockSize * block;
	}
}