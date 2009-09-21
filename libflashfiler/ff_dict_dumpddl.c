#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

static char const * sqltype_for( struct ff_dictent * e )
{
	char const * types[] = 
	{
		"BIT",			//0
		"CHAR",			//1
		"NCHAR",		//2
		"TINYINT",		//3
		"SMALLINT",		//4
		"INT",			//5
		"TINYINT",		//6
		"SMALLINT",		//7
		"INT",			//8
		"INT",	// INT IDENTITY		//9
		"FLOAT",		//10
		"REAL",			//11
		"REAL",			//12
		"MONEY",	// COBOL "COMP". WTF.	//13
		"MONEY",		//14
		"DATETIME",	//systools fail		//15
		"DATETIME",	// ""				//16
		"DATETIME",		//17
		"TEXT",			//18
		"TEXT",			//19

	};

	if (e->type < 20) return types[e->type];
	switch( e->type )
	{
	case 44:
		{
			static char sz[64];
			sprintf( sz, "VARBINARY(%d)", e->length );
			return sz;
		}
	case 45:
	case 46:
	case 47:
	case 48:
		{
			static char sz[64];
			sprintf( sz, "VARCHAR(%d)", e->length );
			return sz;
		}
	case 49:
		{
			static char sz[64];
			sprintf( sz, "NVARCHAR(%d)", e->length >> 1 );
			return sz;
		}
	default:
		{
			static char sz[64];
			sprintf( sz, "$type(%d)", e->type );
			return sz;
		}
	};
}

static void dumpddl_for_field( struct ff_dict * dict, struct ff_dictent * e, FILE * f )
{
	dict;
	fprintf( f, "\t [%s] %s,\n", e->name, sqltype_for( e ) );
}

void ff_dict_dumpddl( struct ff_dict * dict, FILE * f, char const * dumpTableName )
{
	struct ff_dictent * e = dict->fields;
	int n = dict->numFields;

	fprintf( f, "CREATE TABLE [%s] (\n", dumpTableName );

	while( n-- )
		dumpddl_for_field( dict, e++, f );

	fprintf( f, ");\n" );
}