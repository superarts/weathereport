#ifndef __WEATHEREPORT_H
#define __WEATHEREPORT_H

//#define DISABLE_CACHE
//#define VERBOSE_MODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
//#include <unistd.h>

#include "curlget.h"
#include "httpget.h"

//#define OFFLINE_TEST
//#define ENABLE_MEMORY_CHECK

#define SIZEOF_TODAY		9
#define SIZEOF_CITY			256
#define SIZEOF_DAT			32
#define SIZEOF_CACHE		(SIZEOF_TODAY + SIZEOF_CITY + 6)
#define FORMAT_DAT_WOEID 	"woeid_%i.txt"
#define FORMAT_DAT_WEATHER	"forecast_%i.txt"
#define FORMAT_CACHE_NAME	"%s_%s.txt"
#define HOST_YAHOO_WOEID	"where.yahooapis.com"
#define HOST_YAHOO_WEATHER	"weather.yahooapis.com"
#define FORMAT_URL_WOEID	"v1/places.q('%s')?appid=uG20.p3V34G.ZXi8DEfWmCAwJd886uhZMmdR2CiKMBALacJSIOn84P3mzITQbw7aK7u_gQ--"
#define FORMAT_URL_WEATHER	"forecastrss?w=%s&u=c";
//#define FORMAT_URL_WOEID	"http://where.yahooapis.com/v1/places.q('%s')?appid=uG20.p3V34G.ZXi8DEfWmCAwJd886uhZMmdR2CiKMBALacJSIOn84P3mzITQbw7aK7u_gQ--"
//#define FORMAT_URL_WEATHER	"weather.yahooapis.com/forecastrss?w=%s&u=c";
#define WOEID_HEAD			"<woeid>"
#define WOEID_TAIL			"</woeid>"
#define WEATHER_HEAD		"<BR /><b>Forecast:</b><BR />\n"
#define WEATHER_TAIL		"<br />\n<br />\n<a href=\"http://us.rd.yahoo.com/dailynews/rss/weather/"

long	get_filesize(FILE* fp);
void 	get_today(char* buf);

char*	alloc_city_uri(char* s);
char*	alloc_from_file(char* filename);
char*	alloc_string_between(char* source, char* head, char* tail);
void	format_weather(char* s);
bool	cache_exists(char* city);

#endif	//	__WEATHEREPORT_H
