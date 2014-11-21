#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "detect.h"

int get_ss_weight(string &html);
int get_content_weight(string &html);

int qzone_detect(char *number)
{
	int weight = 0, ret = 0;
	char detect_url_1[1024];
	char detect_url_2[1024];
	data_t http_data;
	sprintf(detect_url_1, DETECT_URL_FORMAT, number);
	ret = http_get(detect_url_1, http_data);
	string_line_format(http_data.buffer);
	//printf("http response 1: %s, ret: %d\n", http_data.buffer.c_str(), ret);
	weight = get_ss_weight(http_data.buffer);
	if(weight == -1){
		http_data.buffer.clear();
		sprintf(detect_url_2, DETECT_URL_FORMAT_2, number);
		ret = http_get(detect_url_2, http_data);
		//string_line_format(http_data.buffer);
		//printf("http response 2: %s, ret: %d\n", http_data.buffer.c_str(), ret);
		weight = get_content_weight(http_data.buffer);
	}
	printf("mail weight: %d\n", weight);
	return 0; 
}

/*
 *	通过“说说”的数量来计算权重
 */
int get_ss_weight(string &html)
{
	string::size_type pos1, pos2;
	pos1 = html.find("SS");
	if(pos1 == string::npos) return -1;

	pos2 = html.find(",", pos1);
	if(pos2 == string::npos) return -1;

	string sub = html.substr(pos1+4, pos2-pos1-4);
	int weight = atoi(html.substr(pos1+4, pos2-pos1-4).c_str());
	return weight;
}

/*
 *	当QQ空间没开通或访问受限时，通过QQ空间主页是否含有'<div class="error_content">'
 *	或'<div class="main_content">'来区分
 */
int get_content_weight(string &html)
{
	string::size_type pos1 = html.find("<div class=\"main_content\">");
	string::size_type pos2 = html.find("<div class=\"error_content\">");
	//printf("main_content: %d, error_content: %d\n", pos1, pos2);
	if(pos1 != string::npos) return 2;
	if(pos2 != string::npos) return 0;
	return 0;
}

