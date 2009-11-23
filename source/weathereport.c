#include "weathereport.h"

int main(int argc, char** argv)
{
	char	filename_cache[SIZEOF_CACHE];
	char	today[SIZEOF_TODAY];
	char*	forecast;
	char*	city;

	//	exit if city is not given
	if (argc == 1)
	{
		printf("Usage: cli_weathereport CITY_NAME\n");
		return 0;
	}

	//	change space to %20, e.g. 'New York' -> 'New%20York'
	city = alloc_city_uri(argv[1]);

	if (cache_exists(city))
	{
		//	read from cache
		get_today(today);
		sprintf(filename_cache, FORMAT_CACHE_NAME, today, city);
		forecast = alloc_from_file(filename_cache);
		printf("Loading from cache:\n\n%s\n", forecast);
	}
	else
	{
		//	download from internet
		printf("No cache file found.\nFetching data of '%s', please wait...\n", argv[1]);

		forecast = yahoo_alloc_forecast(city);
		printf("Got forecast in 2 days:\n\n%s\n", forecast);

#ifndef DISABLE_CACHE
		//	write cache file as 'YYYYMMDD_CITYNAME.txt'
		FILE*	fp;

		get_today(today);
		sprintf(filename_cache, FORMAT_CACHE_NAME, today, city);
		fp = fopen(filename_cache, "wb");
		if (fp == NULL)
		{
			printf("ERROR: cannot write cache file '%s'\n", filename_cache);
			exit(2);
		}
		fwrite(forecast, strlen(forecast), 1, fp);
		fclose(fp);
#endif
	}	// end if cache_exists(city)

	//	free buffers
	free(forecast);
	free(city);

	return 0;
}
