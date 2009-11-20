#include "httpget.h"

#define PORT		80
#define USERAGENT	"HTTPGET 1.0"

char* alloc_httpget(char* host, char* path)
{
	struct sockaddr_in*	remote;
	int		sock;
	int		tmpres;
	char*	ip;
	char*	get;
	char	buf[BUFSIZ + 1];
	int		sent = 0;
	int		http_start = 0;
	char*	http_content;
	char*	ret;
	int		ret_pos;

	//	printf("httpget: %s/%s\n", host, path);

	//	prepare socket connection
	sock = create_tcp_socket();
	ip = get_ip(host);
#ifdef HTTPGET_OUTPUT
	fprintf(stderr, "IP is %s\n", ip);
#endif
	remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
	remote->sin_family = AF_INET;
	tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
	if (tmpres < 0)  
	{
		perror("ERROR: cannot set remote->sin_addr.s_addr");
		exit(1);
	}
	else if (tmpres == 0)
	{
		fprintf(stderr, "ERROR: %s is not a valid IP address\n", ip);
		exit(1);
	}
	remote->sin_port = htons(PORT);

	if (connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0)
	{
		perror("ERROR: connect failed");
		exit(1);
	}
	get = build_get_query(host, path);
#ifdef HTTPGET_OUTPUT
	fprintf(stderr, "HTTP query:\n----START----\n%s-----END-----\n", get);
#endif

	//	send the query to the server
	while (sent < strlen(get))
	{
		tmpres = send(sock, get + sent, strlen(get) - sent, 0);
		if (tmpres == -1)
		{
			perror("ERROR: send failed");
			exit(1);
		}
		sent += tmpres;
	}

	//	receive the content
	ret = malloc(1);
	ret[0] = '\0';
	memset(buf, 0, sizeof(buf));
	while ((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0)
	{
		if (http_start == 0)
		{
			http_content = strstr(buf, "\r\n\r\n");
			if (http_content != NULL)
			{
				http_start = 1;
				http_content += 4;
			}
		}
		else
		{
			http_content = buf;
		}
		if (http_start)
		{
			ret_pos = strlen(ret);
			ret = realloc(ret, ret_pos + strlen(http_content) + 1);
			strcpy(&ret[ret_pos], http_content);
			ret[ret_pos + strlen(http_content) + 1] = '\0';
#ifdef HTTPGET_OUTPUT
			fprintf(stdout, "%s", http_content);
#endif
		}

		memset(buf, 0, tmpres);
	}
	if (tmpres < 0)
	{
		perror("Error: receiving failed");
	}

	//	free buffers
	free(get);
	free(remote);
	free(ip);
	close(sock);

	return ret;
}

int create_tcp_socket()
{
	int sock;
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("ERROR: creating socket failed");
		exit(1);
	}
	return sock;
}

char* get_ip(char* host)
{
	struct hostent *hent;
	int		iplen = 15;			//XXX.XXX.XXX.XXX
	char*	ip = (char *)malloc(iplen + 1);

	memset(ip, 0, iplen + 1);
	if ((hent = gethostbyname(host)) == NULL)
	{
		herror("ERROR getting host failed");
		exit(1);
	}
	if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
	{
		perror("ERROR: resolving host failed");
		exit(1);
	}
	return ip;
}

char* build_get_query(char* host, char* path)
{
	char*	query;
	//char	tpl[] = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	char	tpl[] = "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n";

	// -5: "%s...%s...%s...\0"
	query = (char*)malloc(strlen(host) + 
			strlen(path) + strlen(USERAGENT) + strlen(tpl) - 5);
	sprintf(query, tpl, path, host, USERAGENT);

	return query;
}
