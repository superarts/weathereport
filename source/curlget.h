#ifndef __CURLGET_H
#define __CURLGET_H

#define CURL_STATICLIB

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string.h>

size_t	write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
void	curl_download(char* url, char* filename);

#endif	//	__CURLGET_H
