#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZEOF_TODAY		9

long	get_filesize(FILE* fp);
void 	get_today(char* buf);

char*	alloc_city_uri(char* s);
char*	alloc_from_file(char* filename);
char*	alloc_string_between(char* source, char* head, char* tail);

#endif	//	 __UTILS_H
