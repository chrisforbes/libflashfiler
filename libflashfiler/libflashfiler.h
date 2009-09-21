#ifndef _LIBFLASHFILER_H
#define _LIBFLASHFILER_H

#include <oaidl.h>			/* for variants */

#ifdef LIBFLASHFILER_EXPORTS
	#define FFEXPORT __declspec(dllexport)
#else
	#define FFEXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	// libflashfiler public interface

	FFEXPORT struct ff_db * ff_opendb( char const * filename );
	FFEXPORT void ff_closedb( struct ff_db * db );
	FFEXPORT void ff_dumpstats( struct ff_db * db );
	FFEXPORT struct ff_dict * ff_opendict( struct ff_db * db );
	FFEXPORT void ff_closedict( struct ff_dict * dict );
	FFEXPORT void ff_dict_dumpddl( struct ff_dict * dict, FILE * f, char const * dumpTableName );
	FFEXPORT void ff_dumpddl( struct ff_db * db, FILE * f, char const * dumpTableName );
	FFEXPORT struct ff_dictent * ff_dict_findfield( struct ff_dict * dict, char const * name );
	FFEXPORT struct ff_cursor * ff_cur_open( struct ff_db * db );
	FFEXPORT void ff_cur_close( struct ff_cursor * cur );
	FFEXPORT int ff_cur_next( struct ff_cursor * cur );
	FFEXPORT int ff_cur_hasnext( struct ff_cursor * cur );
	FFEXPORT VARIANT ff_cur_get( struct ff_cursor * cur, char const * field );
	FFEXPORT __int64 ff_delphi_to_unixtime( double d );

#ifdef __cplusplus
}
#endif

#endif