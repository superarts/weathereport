#include "utils.h"

/*
 * Returns the file size of <fp>.
 * fp will be reset to SEEK_SET afterward.
 */
long get_filesize(FILE* fp)
{
	long	ret;

	fseek(fp, 0, SEEK_END);
	ret = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return ret;
}

/*
 * Set the <buf> with current date with "YYYYMMDD" format.
 * <buf> must be prepared at first.
 */
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

/*
 * Allocates and returns the unified city name from <s>.
 * E.g. "New York" -> "New%20York"
 */
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

/*
 * Allocates and reads a string from <filename>.
 * free() is needed afterward.
 */
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

/*
 * Allocates and returns a string from <source> between <head> and <tail>.
 * free() is needed afterward.
 */
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
