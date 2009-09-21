#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"
#include <assert.h>

char ff_stream_readbyte( struct ff_stream * s )
{
	char c;
	ff_readstream( s, &c, 1 );
	return c;
}

int ff_stream_readbool( struct ff_stream * s )
{
	char c = ff_stream_readbyte(s);
	return c != 8;
}

int ff_stream_readint( struct ff_stream * s )
{
	char c = ff_stream_readbyte( s );
	int i = 0;
	ff_readstream( s, &i, c - 1 );
	return i;
}

int ff_stream_readstring( struct ff_stream * s, char * buf )
{
	int len;
	ff_seekstream( s, 1 );
	len = ff_stream_readbyte( s );
	ff_readstream( s, buf, len );
	buf[len] = 0;
	return len;
}

// note: need to free() this ptr!
char * ff_stream_readbin( struct ff_stream * s, int n )
{
	char * result = malloc(n);
	ff_readstream( s, result, n );
	return result;
}

void ff_stream_skipbin( struct ff_stream * s, int n )
{
	ff_seekstream( s, n );
}

void ff_stream_skipstring( struct ff_stream * s )
{
	int len;
	ff_seekstream( s, 1 );
	len = ff_stream_readbyte( s );
	ff_seekstream( s, len );
}