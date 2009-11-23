#include "yahoo.h"

void yahoo_format_weather(char* s)
{
	char*	p = strstr(s, "<br />");

	memset(&s[p - s], ' ', 6);
}

char* yahoo_alloc_forecast(char* city)
{
	char	format_url_woeid[] = FORMAT_URL_WOEID;
	char	format_url_weather[] = FORMAT_URL_WEATHER;
	char*	content_woeid;
	char*	content_weather;
	char* 	url_woeid;
	char* 	url_weather;
	char*	woeid;
	char*	forecast;

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
	yahoo_format_weather(forecast);

	//	free buffers
	free(content_weather);
	free(url_weather);
	free(woeid);
	free(content_woeid);
	free(url_woeid);

	return forecast;
}

