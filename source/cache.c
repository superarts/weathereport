#include "cache.h"

/*
 * Returns true if cache file <city> exists.
 * Returns false otherwise.
 */
bool cache_exists(char* city)
{
	char	today[SIZEOF_TODAY];
	char	filename[SIZEOF_CACHE];
	FILE*	fp;
	long	pos;

#ifdef DISABLE_CACHE
	return false;
#endif

	get_today(today);
	sprintf(filename, FORMAT_CACHE_NAME, today, city);

	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		pos = ftell(fp);
		fclose(fp);

		if (pos > 0)
			return true;
	}

	return false;
}
