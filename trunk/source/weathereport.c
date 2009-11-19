#define CURL_STATICLIB

//#define DISABLE_CACHE
//#define OFFLINE_TEST

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define SIZEOF_TODAY		9
#define SIZEOF_CITY			256
#define SIZEOF_DAT			32
#define SIZEOF_CACHE		(SIZEOF_TODAY + SIZEOF_CITY + 6)
#define FORMAT_DAT_WOEID 	"woeid_%i.txt"
#define FORMAT_DAT_WEATHER	"forecast_%i.txt"
#define FORMAT_CACHE_NAME	"%s_%s.txt"
#define FORMAT_URL_WOEID	"http://where.yahooapis.com/v1/places.q('%s')?appid=uG20.p3V34G.ZXi8DEfWmCAwJd886uhZMmdR2CiKMBALacJSIOn84P3mzITQbw7aK7u_gQ--"
#define FORMAT_URL_WEATHER	"weather.yahooapis.com/forecastrss?w=%s&u=c";
#define WOEID_HEAD			"<woeid>"
#define WOEID_TAIL			"</woeid>"
#define WEATHER_HEAD		"<BR /><b>Forecast:</b><BR />\n"
#define WEATHER_TAIL		"<br />\n<br />\n<a href=\"http://us.rd.yahoo.com/dailynews/rss/weather/"

long	get_filesize(FILE* fp);
size_t	write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
void	curl_download(char* url, char* filename);
void 	get_today(char* buf);

char*	alloc_from_file(char* filename);
char*	alloc_string_between(char* source, char* head, char* tail);
void	format_weather(char* s);
bool cache_exists(char* city);

int main(int argc, char** argv)
{
	char	format_url_woeid[] = FORMAT_URL_WOEID;
	//char	format_url_weather[] = "http://xml.weather.yahoo.com/forecastrss/%s_f.xml";
	char	format_url_weather[] = FORMAT_URL_WEATHER;
	char	filename_woeid[SIZEOF_DAT];
	char	filename_weather[SIZEOF_DAT];
	char	filename_cache[SIZEOF_CACHE];
	char	today[SIZEOF_TODAY];
	char* 	url_woeid;
	char* 	url_weather;
	char*	content_woeid;
	char*	content_weather;
	char*	woeid;
	char*	forecast;

	if (argc == 1)
	{
		printf("Usage: cli_weathereport CITY_NAME\n");
		return 0;
	}

	if (cache_exists(argv[1]))
	{
		get_today(today);
		sprintf(filename_cache, FORMAT_CACHE_NAME, today, argv[1]);
		content_weather = alloc_from_file(filename_cache);
		printf("Loading from cache:\n\n%s\n", content_weather);

		free(content_weather);
		
		return 0;
	}

	//	TODO: enable space support, e.g. 'new york'
	printf("No cache file found.\nTarget: %s\n", argv[1]);

	url_woeid = (char*)malloc(strlen(format_url_woeid) + strlen(argv[1]));
	sprintf(url_woeid, format_url_woeid, argv[1]);
	printf("Fetching WOEID...\n");
	sprintf(filename_woeid, FORMAT_DAT_WOEID, getpid());
#ifndef OFFLINE_TEST
	curl_download(url_woeid, filename_woeid);
#endif
	content_woeid = alloc_from_file(filename_woeid);
	woeid = alloc_string_between(content_woeid, WOEID_HEAD, WOEID_TAIL);
	if (woeid == NULL)
	{
		printf("ERROR: wrong woeid format, please check data file %s.\nExiting...\n",
				filename_woeid);
		exit(0);
	}
	printf("Got WOEID: %s\n", woeid);

	url_weather = (char*)malloc(strlen(format_url_weather) + strlen(woeid));
	sprintf(url_weather, format_url_weather, woeid);
	//	printf("got weather url: %s\n", url_weather);
	printf("Fetching weather info...\n");
	sprintf(filename_weather, FORMAT_DAT_WEATHER, getpid());
#ifndef OFFLINE_TEST
	curl_download(url_weather, filename_weather);
#endif
	content_weather = alloc_from_file(filename_weather);
	forecast = alloc_string_between(content_weather, WEATHER_HEAD, WEATHER_TAIL);
	if (woeid == NULL)
	{
		printf("ERROR: wrong woeid format, please check data file %s.\nExiting...\n",
				filename_weather);
		exit(0);
	}
	format_weather(forecast);
	printf("Got forecast in 2 days:\n\n%s\n", forecast);

#ifndef DISABLE_CACHE
	FILE*	fp;

	get_today(today);
	sprintf(filename_cache, FORMAT_CACHE_NAME, today, argv[1]);
	fp = fopen(filename_cache, "wb");
	if (fp == NULL)
	{
		printf("ERROR: cannot write cache file '%s'\nExiting...\n", filename_cache);
		return 0;
	}
	fwrite(forecast, strlen(forecast), 1, fp);
	fclose(fp);
#endif

	//	TODO: unlink data files
	
	free(forecast);
	free(content_weather);
	free(url_weather);
	free(woeid);
	free(content_woeid);
	free(url_woeid);
	
	return 0;
}

long get_filesize(FILE* fp)
{
	long	ret;

	fseek(fp, 0, SEEK_END);
	ret = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return ret;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void curl_download(char* url, char* filename)
{
    CURL*		curl;
    FILE*		fp;
    CURLcode	res;

    curl = curl_easy_init();
    if (curl)
	{
        fp = fopen(filename, "wb");
		if (fp == NULL)
		{
			printf("ERROR: cannot write data file '%s'\nExiting...\n", filename);
			return;
		}

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    return;
}

void get_today(char* buf)
{
	time_t		now;
	struct tm*	ts;

	now = time(NULL);
	ts = localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d", ts);

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
		printf("ERROR: cannot read from file '%s'\nExiting...\n", filename);
		//	return NULL;
		exit(0);
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
	FILE*	fp;
	char	today[SIZEOF_TODAY];
	char	filename[SIZEOF_CACHE];
	long	pos;

#ifdef DISABLE_CACHE
	return false;
#endif

	get_today(today);
	sprintf(filename, FORMAT_CACHE_NAME, today, city);

	//	TODO: use access and lock to check cache existence
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
