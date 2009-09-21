#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

void ff_closedict( struct ff_dict * dict )
{
	if (dict)
		free( dict->fields );
	free( dict );
}