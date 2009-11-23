#ifndef __YAHOO_H
#define __YAHOO_H

#include <stdio.h>
#include <string.h>
#include "httpget.h"
#include "utils.h"

#define FORMAT_DAT_WOEID 	"woeid_%i.txt"
#define FORMAT_DAT_WEATHER	"forecast_%i.txt"
#define HOST_YAHOO_WOEID	"where.yahooapis.com"
#define HOST_YAHOO_WEATHER	"weather.yahooapis.com"
#define FORMAT_URL_WOEID	"v1/places.q('%s')?appid=uG20.p3V34G.ZXi8DEfWmCAwJd886uhZMmdR2CiKMBALacJSIOn84P3mzITQbw7aK7u_gQ--"
#define FORMAT_URL_WEATHER	"forecastrss?w=%s&u=c";
#define WOEID_HEAD			"<woeid>"
#define WOEID_TAIL			"</woeid>"
#define WEATHER_HEAD		"<BR /><b>Forecast:</b><BR />\n"
#define WEATHER_TAIL		"<br />\n<br />\n<a href=\"http://us.rd.yahoo.com/dailynews/rss/weather/"

char*	yahoo_alloc_forecast(char* city);
void	yahoo_format_weather(char* s);

#endif	//	__YAHOO_H
