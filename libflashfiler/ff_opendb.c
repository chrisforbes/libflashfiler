#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

struct ff_db * ff_opendb( char const * filename )
{
	struct ff_db * db = malloc( sizeof(struct ff_db) );
	db->fileHandle = CreateFileA( filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	if (INVALID_HANDLE_VALUE == db->fileHandle) error( "Cannot open file", cleanup_free );

	db->mappingHandle = CreateFileMapping( db->fileHandle, 0, PAGE_WRITECOPY, 0, 0, 0 ); 
	if (INVALID_HANDLE_VALUE == db->mappingHandle) error( "Cannot create file mapping", cleanup_closefile );

	db->base = MapViewOfFile( db->mappingHandle, FILE_MAP_COPY, 0, 0, 0 );
	if (!db->base) error( "Cannot map view of file", cleanup_mapping );
	return db;

cleanup_mapping:
	CloseHandle( db->mappingHandle );
cleanup_closefile:
	CloseHandle( db->fileHandle );
cleanup_free:
	free( db );
	return 0;
}