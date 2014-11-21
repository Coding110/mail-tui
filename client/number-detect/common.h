#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <cstring>
#include <stdio.h>
using namespace std;

typedef struct _data_t
{
	string buffer;
}data_t;

#define DETECT_URL_FORMAT "http://user.qzone.qq.com/p/r/cgi-bin/main_page_cgi?uin=%s&param=%%7c16"
#define DETECT_URL_FORMAT_2 "http://user.qzone.qq.com/%%s"

int http_get(char *url, data_t &http_data);
size_t http_write_data(void *ptr, size_t size, size_t nmemb, void *args);
int memcpy_no_blank(char *src, size_t src_size, string &dst);
int string_line_format(string &src);

#endif

