#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "libflashfiler.h"
#include "ff_internal.h"

char ff_getdeltrailer( struct ff_db * db )
{
	struct ff_header_file * h = db->base;
	struct ff_header_data_block * b = ff_getblockptr( db, h->firstDataBlock );

	char const * firstRecordTrailer = (char const *)( b+1 ) + h->recordLength;
	
	// zOMG! derefing stuff when the table is EMPTY is bad
	if (!(h->numRecords + h->numDeletedRecords))
		return 0;

	if (!h->numDeletedRecords)
	{	// return something different from the first actual record's trailer!
		return ~*firstRecordTrailer;
	}
	else
	{
		char const * delTrailer = 
			(char const *)db->base + h->firstDeletedRecord + h->recordLength;

		// deleted marker in trailer is ONLY USED when there's deleted records BEFORE real records
		if ((delTrailer != firstRecordTrailer) && (*delTrailer == *firstRecordTrailer))
			return ~*firstRecordTrailer;

		return *delTrailer;
	}
}