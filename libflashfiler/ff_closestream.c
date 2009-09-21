#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

void ff_closestream( struct ff_stream * s )
{
	free( s );
}