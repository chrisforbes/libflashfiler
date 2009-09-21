#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "libflashfiler.h"
#include "ff_internal.h"

time_t ff_delphi_to_unixtime( double d )
{
	return (time_t)( (d - 25569.0 - 693500.0) * 86400.0 );
}