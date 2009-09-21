#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libflashfiler.h"
#include "ff_internal.h"

struct ff_dictent * ff_dict_findfield( struct ff_dict * dict, char const * name )
{
	struct ff_dictent * e = dict->fields;
	int n = dict->numFields;

	while( n-- )
		if ( !strcmp( name, e->name ) )
			return e;
		else
			++e;

	return 0;
}