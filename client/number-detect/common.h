#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <cstring>
#include <vector>
#include <stdio.h>
using namespace std;

#ifdef WIN32
//#include "stdafx.h"
#define snprintf sprintf_s
#define usleep(t) Sleep(t/1000)
#define sleep(t) Sleep(t*1000)
typedef long long int64_t;
#else
#endif

typedef struct _data_t
{
	string buffer;
}data_t;

typedef struct _number_info_t
{
	string number;
	int weight;
}number_info_t;

#define VERSION "1.0"
#define DETECT_URL_FORMAT "http://user.qzone.qq.com/p/r/cgi-bin/main_page_cgi?uin=%s&param=%%7c16"
#define DETECT_URL_FORMAT_2 "http://user.qzone.qq.com/%s"
//#define GET_NUMBERS_FORMAT   "http://in.becktu.com:8001/tui/ws.php?method=get-qq-numbers&u=%s&version=%s"
//#define UPLOAD_RESULT_FORMAT "http://in.becktu.com:8001/tui/ws.php?method=upload-qq-result&u=%s&version=%s"
#define GET_NUMBERS_FORMAT   "http://192.168.0.82/Php1.0/qqServer.php?method=get-qq-numbers"
#define UPLOAD_RESULT_FORMAT "http://192.168.0.82/Php1.0/qqServer.php?method=upload-qq-result"

int http_get(const char *url, data_t &http_data);
int http_post(const char *url, char *form_data, data_t &http_data);
int memcpy_no_blank(char *src, size_t src_size, string &dst);
int string_line_format(string &src);
int numbers_split(vector<number_info_t> &nis, char *numbers);

char *strsep(char **stringp, const char *delim);

#endif

