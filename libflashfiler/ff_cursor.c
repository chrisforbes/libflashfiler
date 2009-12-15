#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "libflashfiler.h"
#include "ff_internal.h"

struct ff_cursor
{
	struct ff_db * db;
	struct ff_dict * dict;
	char deltrailer;

	struct ff_header_data_block * b;
	int remaining_in_block;
	int remaining_records;
	int remaining_real_records;
	char const * record_base;
};

extern char ff_getdeltrailer( struct ff_db * db );
extern time_t ff_delphi_to_unixtime( double d );

struct ff_cursor * ff_cur_open( struct ff_db * db )
{
	struct ff_cursor * cur = malloc( sizeof( *cur ) );
	struct ff_header_file * h = db->base;
	
	cur->db = db;
	cur->dict = ff_opendict( db );
	cur->deltrailer = ff_getdeltrailer( db );
	cur->b = 0;

	cur->remaining_records = h->numRecords + h->numDeletedRecords;
	cur->remaining_real_records = h->numRecords;
	cur->remaining_in_block = 0;

	return cur;
}

void ff_cur_close( struct ff_cursor * cur )
{
	if (!cur) return;
	ff_closedict( cur->dict );
	free( cur );
}

static void ff_cur_next_inner( struct ff_cursor * cur )
{
	struct ff_header_file * h = cur->db->base;
	if (!cur->remaining_in_block)
	{
		cur->b = ff_getblockptr( cur->db, cur->b ? cur->b->nextBlock : h->firstDataBlock );
		cur->remaining_in_block = min( cur->remaining_records, (int)cur->b->numRecords );
		cur->record_base = (char const *)(cur->b + 1);
		--cur->remaining_in_block;
		--cur->remaining_records;
	}
	else
	{
		cur->record_base += h->recordLengthEx;
		--cur->remaining_in_block;
		--cur->remaining_records;
	}
}

int ff_cur_next( struct ff_cursor * cur )
{
	while( cur->remaining_records && cur->remaining_real_records )
	{
		char this_trailer;
		ff_cur_next_inner( cur );

		this_trailer = cur->record_base[ cur->b->recordLength ];
		if (cur->deltrailer != this_trailer && cur->record_base[0] != (char)(unsigned char)0xff)
		{
			--cur->remaining_real_records;
			return 1;
		}
	}
	return 0;
}

int ff_cur_hasnext( struct ff_cursor * cur )
{
	return cur->remaining_real_records;
}

VARIANT ff_cur_get( struct ff_cursor * cur, char const * field )
{
	struct ff_dictent * e = ff_dict_findfield( cur->dict, field );
	char const * q;
	VARIANT v; v.vt = VT_EMPTY;

	if (!e) return v;
	q = cur->record_base + e->offset;

	switch( e->type )
	{
	case 0:
		v.vt = VT_BOOL;		v.boolVal = ~(VARIANT_BOOL)q[1];	break;
	case 1:
		v.vt = VT_I1;		v.bVal = (BYTE)q[1];				break;
	case 5: case 8: case 9:
		v.vt = VT_I4;		v.lVal = *(int const *)(q+1);		break;
	case 11:
		if (e->length == 4) { v.vt = VT_R4;	v.fltVal = *(float const *)(q+1); break; }
		if (e->length == 8) { v.vt = VT_R8; v.dblVal = *(double const *)(q+1); break; }
		break;
	case 14:		/* it's possible this isn't a very good solution. maybe VT_DECIMAL or VT_CY? */
		if (e->length == 8) { v.vt = VT_R8; v.dblVal = *(__int64 const *)(q+1) / 10000.0f; break; }	
		break;
	case 17:
		if (e->length == 8) { v.vt = VT_DATE; v.date = (DATE) *(double const *)(q+1); break; }
		break;
	case 19:
		if (e->length == 8) { v = ff_blob_read( cur->db, *(__int64 const *)(q+1) ); break; }
		break;
	case 45:
	case 46:		/* todo: oem charset vs utf-8 vs 8859-1; yes, this sucks. */
		{
			int len = (int)(unsigned)(unsigned char) q[1];
			if (len)
			{
				wchar_t * wsz = alloca( (len+1) * sizeof(wchar_t) );
				mbstowcs( wsz, q+2, len );
				wsz[ len ] = 0;
				v.vt = VT_BSTR;
				v.bstrVal = SysAllocString( wsz );
			}
			else
			{
				v.vt = VT_BSTR;
				v.bstrVal = SysAllocString( L"" );
			}
			
		} break;
	case 48:
		{
			int len = strlen( q+1 );
			wchar_t * wsz = alloca( (len+1) * sizeof(wchar_t) );
			mbstowcs( wsz, q+1, len+1 );
			v.vt = VT_BSTR;
			v.bstrVal = SysAllocString( wsz );
		} break;
	case 49:
		{
			v.vt = VT_BSTR;
			v.bstrVal = SysAllocString( (wchar_t const *) (q+1) );
		} break;
	}

	return v;
}