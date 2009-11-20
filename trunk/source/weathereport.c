#include "weathereport.h"

int main(int argc, char** argv)
{
	char	format_url_woeid[] = FORMAT_URL_WOEID;
	char	format_url_weather[] = FORMAT_URL_WEATHER;
	char	filename_cache[SIZEOF_CACHE];
	char	today[SIZEOF_TODAY];
	char* 	url_woeid;
	char* 	url_weather;
	char*	content_woeid;
	char*	content_weather;
	char*	woeid;
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

	//	read from cache instead of downloading if the city and date info already exist
	if (cache_exists(city))
	{
		get_today(today);
		sprintf(filename_cache, FORMAT_CACHE_NAME, today, city);
		content_weather = alloc_from_file(filename_cache);
		printf("Loading from cache:\n\n%s\n", content_weather);
		free(content_weather);
		free(city);
		
		return 0;
	}

	printf("No cache file found.\nFetching data of '%s', please wait...\n", argv[1]);

	//	fetch woeid
	url_woeid = (char*)malloc(strlen(format_url_woeid) + strlen(city));
	sprintf(url_woeid, format_url_woeid, city);
	content_woeid = alloc_httpget(HOST_YAHOO_WOEID, url_woeid);

	//	parse woeid
	woeid = alloc_string_between(content_woeid, WOEID_HEAD, WOEID_TAIL);
	if (woeid == NULL)
	{
		printf("ERROR: wrong woeid format, please check the content below:\n%s\n",
				content_woeid);
		exit(2);
	}

	//	fetch forecast
	url_weather = (char*)malloc(strlen(format_url_weather) + strlen(woeid));
	sprintf(url_weather, format_url_weather, woeid);
	content_weather = alloc_httpget(HOST_YAHOO_WEATHER, url_weather);

	//	parse forecast
	forecast = alloc_string_between(content_weather, WEATHER_HEAD, WEATHER_TAIL);
	if (forecast == NULL)
	{
		printf("ERROR: wrong forecast format, please check the content below:\n%s\n",
				content_weather);
		exit(2);
	}
	format_weather(forecast);
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

	//	free buffers
	free(forecast);
	free(content_weather);
	free(url_weather);
	free(woeid);
	free(content_woeid);
	free(url_woeid);
	free(city);

	return 0;
}

char* alloc_city_uri(char* s)
{
	int		i;
	int		count = 0;
	int		index = 0;
	char*	ret;

	for (i = 0; i < strlen(s); i++)
	{
		if (s[i] == ' ')
			count++;
	}
	ret = malloc(strlen(s) + i * 2);
	ret[strlen(s) + i * 2] = '\0';
	for (i = 0; i < strlen(s); i++)
	{
		if (s[i] != ' ')
		{
			ret[index] = s[i];
			index++;
		}
		else
		{
			ret[index + 0] = '%';
			ret[index + 1] = '2';
			ret[index + 2] = '0';
			index += 3;
		}
	}

	return ret;
}

long get_filesize(FILE* fp)
{
	long	ret;

	fseek(fp, 0, SEEK_END);
	ret = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return ret;
}

void get_today(char* buf)
{
	time_t		now;
	struct tm*	ts;

	now = time(NULL);
	ts = localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d", ts);
	buf[8] = '\0';

	return;
}

char* alloc_from_file(char* filename)
{
	char*	ret;
	FILE*	fp;
	long	filesize;

	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("ERROR: cannot read from file '%s'\n", filename);
		exit(2);
	}

	filesize = get_filesize(fp);
	ret = malloc(filesize + 1);
	fread(ret, filesize, 1, fp);
	ret[filesize] = '\0';
	fclose(fp);

	return ret;
}

char* alloc_string_between(char* source, char* head, char* tail)
{
	char*	ret;
	int		i1, i2, length;
	char*	p1;
	char*	p2;

	p1 = strstr(source, head);
	p2 = strstr(source, tail);

	if ((p1 == NULL) || (p2 == NULL))
	{
		printf("DEBUG: string not found in %s\n", source);
		return NULL;
	}

	i1 = p1 - source + strlen(head);
	i2 = p2 - source;
	length = i2 - i1;

	ret = (char*)malloc(length + 1);
	strncpy(ret, source + i1, length);
	ret[length] = '\0';

	return ret;
}

void format_weather(char* s)
{
	char*	p = strstr(s, "<br />");

	memset(&s[p - s], ' ', 6);
}

bool cache_exists(char* city)
{
	char	today[SIZEOF_TODAY];
	char	filename[SIZEOF_CACHE];

#ifdef DISABLE_CACHE
	return false;
#endif

	get_today(today);
	sprintf(filename, FORMAT_CACHE_NAME, today, city);

#if 1
	if (access(filename, F_OK) == 0)
		return true;
	else
		return false;
#endif

	//	fopen version
#if 1
	FILE*	fp;
	long	pos;

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
#endif
}
