#include "curlget.h"

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
			printf("ERROR: cannot write data file '%s'\n", filename);
			exit(3);
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

