#include <stdio.h>
#include <stdlib.h>
#include "../libflashfiler/libflashfiler.h"

// error() like gnulib
static void __declspec(noreturn) error( char const * e, int code )
{
	fflush( 0 );
	fprintf( stderr, "flashfilerdemo: %s\n", e );
	exit( code );
}

static char * fixTableName( char * name )
{
	char * start = name;
	while( name && *name )
	{
		if (*name == '/' || *name == '\\') start = name+1;
		if (*name == '.') *name = 0;
		++name;
	}

	return start;
}

int main( int argc, char ** argv )
{
	--argc; ++argv;
	if ( !argc )
	{
		printf( "usage: flashfilerdemo FF2FILE\n" );
		exit(1);
	}
	else
	{
		struct ff_db * db = ff_opendb( argv[0] );
		struct ff_dict * dict;
		char * tableName = fixTableName( argv[0] );

		if (!db) error( "cannot open db", 1 );

		printf( "-- dumping data dictionary for `%s`\n", tableName );

		ff_dumpstats( db );
		dict = ff_opendict( db );
		ff_dict_dumpddl( dict, stdout, tableName );
		ff_closedict( dict );

		printf( "-- dumping content for `%s`\n", tableName );
		ff_dumpddl( db, stdout, tableName );
		ff_closedb( db );
		return 0;
	}
}