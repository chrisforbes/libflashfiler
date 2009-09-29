#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "libflashfiler.h"
#include "ff_internal.h"

static struct ff_header_blob_block * ff_blob_find_block( struct ff_db * db, int /* 64 */ nblob, /* out */ int * offset )
{
	struct ff_header_file * h = db->base;
	int mask = (1LL << h->logBlockSize) - 1;
	int block = (DWORD)(nblob & ~mask);
	*offset = nblob & mask;

	return block ? ff_getblockptr( db, block ) : 0;
}

VARIANT ff_blob_read( struct ff_db * db, __int64 n )
{
	int offset = 0;
	struct ff_header_blob_block * hb = ff_blob_find_block( db, (int) n, &offset );

	if (!hb) goto failed;

	/* todo: make it work :D */
	if (hb)
	{
		VARIANT v; v.vt = VT_BSTR; v.bstrVal = SysAllocString(L"This doesn't work yet"); return v;
	}

failed:
	{ VARIANT v; v.vt = VT_BSTR; v.bstrVal = SysAllocString(L""); return v; }
}