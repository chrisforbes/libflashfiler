#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

static void skip_filespecs( struct ff_stream * s )
{
	int numspecs = ff_stream_readint( s );
	while( numspecs-- )
	{
		ff_stream_skipstring( s );	// desc
		ff_stream_skipstring( s );	// ext
		ff_stream_skipint( s );		// blocksize
		ff_stream_skipint( s );		// type
	}
}

static void build_one_fieldspec( struct ff_dictent * e, struct ff_stream * s )
{
	e->streamOffset = s->cursor;
	ff_stream_readstring( s, e->name );
	ff_stream_skipstring( s );

	ff_stream_skipint( s );
	ff_stream_skipint( s );
	e->offset = ff_stream_readint( s );
	e->length = ff_stream_readint( s );
	e->type = ff_stream_readint( s );

	ff_stream_skipbool( s );	// required
	e->hasConstraints = ff_stream_readbool( s );
	if (e->hasConstraints) 
	{
		ff_stream_skipstring( s );	//picture
		if (ff_stream_readbool( s )) ff_stream_skipbin( s, e->length );		// minvalue
		if (ff_stream_readbool( s )) ff_stream_skipbin( s, e->length );		// maxvalue
		if (ff_stream_readbool( s )) ff_stream_skipbin( s, e->length );		// defvalue
	}
}

static void build_fieldspecs( struct ff_dict * dict, struct ff_stream * s )
{
	int numfields = dict->numFields = ff_stream_readint( s );
	struct ff_dictent * f = dict->fields = malloc( numfields * sizeof( struct ff_dictent ) );

	while( numfields-- )
		build_one_fieldspec( f++, s );
}

struct ff_dict * ff_opendict( struct ff_db * db )
{
	struct ff_header_file const * h = db->base;
	struct ff_stream * s;
	struct ff_dict * dict = malloc( sizeof(struct ff_dict) );
	dict->db = db;

	if (!h->dataDict)
		error( "no data dictionary", cleanup_dict );

	s = ff_openstream( db, h->dataDict );
	if (ff_stream_readbool( s ))
		error( "data dictionary is encrypted!", cleanup_stream );

	skip_filespecs( s );
	build_fieldspecs( dict, s );
	return dict;

cleanup_stream:
	ff_closestream( s );
cleanup_dict:
	free( dict );
	return 0;
}
