#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

void ff_closedb( struct ff_db * db )
{
	if (!db) return;
	UnmapViewOfFile( db->base );
	CloseHandle( db->mappingHandle );
	CloseHandle( db->fileHandle );
	free( db );
}