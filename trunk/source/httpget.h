#ifndef __HTTPGET_H
#define __HTTPGET_H

//	#define HTTPGET_OUTPUT

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

char*	alloc_httpget(char* host, char* path);
int		create_tcp_socket();
char*	get_ip(char* host);
char*	build_get_query(char* host, char* path);

#endif	//	__HTTPGET_H
