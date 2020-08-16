#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "libflashfiler.h"
#include "ff_internal.h"

extern time_t ff_delphi_to_unixtime( double d );

static char * ff_escape_sql_string( char * dest, char const * src, size_t n )
{
	while( n-- ) {
		if ( *src == '\'' )
			*dest++ = '\'';
		*dest++ = *src++;
	}

	return dest;
}

void ff_dumpddl( struct ff_db * db, FILE * f, char const * dumpTableName )
{
	struct ff_dict * dict = ff_opendict( db );
	struct ff_cursor * c = ff_cur_open( db );

	while( ff_cur_next( c ))
	{
		struct ff_dictent * e = dict->fields;
		int fields = dict->numFields;

		fprintf( f, "INSERT INTO [%s] VALUES(\n", dumpTableName );

		for(; fields; fields-- , e++)
		{
			VARIANT v = ff_cur_get( c, e->name );

#define impl_vt( vt, fmt, val )\
		case (vt): fprintf( f, "\t" fmt ",\n", val ); break

			switch( v.vt )
			{
				impl_vt( VT_BOOL, "%s", v.boolVal ? "false" : "true" );
				impl_vt( VT_I1, "'%c'", v.bVal );
				impl_vt( VT_I4, "%d", v.lVal );
				impl_vt( VT_I8, "%I64d", v.llVal );
				impl_vt( VT_R4, "%g", v.fltVal );
				impl_vt( VT_R8, "%g", v.dblVal );
				
			case VT_DATE:
				{
					time_t t = ff_delphi_to_unixtime( v.date - 94 );
					char sz[1024];
					struct tm * tm = gmtime( &t );
					strcpy( sz, "invalid date" );
					if (tm) {
					strftime( sz, sizeof(sz), "%Y-%m-%d %H:%M:%S", tm );
					}
					fprintf( f, "\t'%s',\n", sz);
				} break;

			case VT_BSTR:
				{
					fprintf( f, "\t'%ls',\n", v.bstrVal );
					SysFreeString( v.bstrVal );
				} break;

			default:
				fprintf( f, "\t$(val %d 0x%x 0x%x), \n", e->type, e->offset, e->length );
			}
		}

		fprintf( f, ");\n" );
	}

	ff_cur_close( c );
	ff_closedict( dict );
}