#ifndef __CACHE_H
#define __CACHE_H

#include <stdbool.h>
#include "utils.h"

#define SIZEOF_CITY			256
#define SIZEOF_CACHE		(SIZEOF_TODAY + SIZEOF_CITY + 6)
#define FORMAT_CACHE_NAME	"%s_%s.txt"

bool	cache_exists(char* city);

#endif	//	__CACHE_H
