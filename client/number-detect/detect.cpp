#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "common.h"
#include "detect.h"
#include "cJSON.h"
#include "log.h"
#include "config.h"

extern config_t gcfg;

int get_ss_weight(string &html);
int get_content_weight(string &html);

int qzone_detect(const char *number)
{
	int weight = 0, ret = 0;
	char detect_url_1[1024];
	char detect_url_2[1024];
	data_t http_data;
	if (!gcfg.num_detect_url_1.empty() && gcfg.num_detect_url_1.size() > 7){
		snprintf(detect_url_1, 1024, gcfg.num_detect_url_1.c_str(), number);
	}else{
		snprintf(detect_url_1, 1024, DETECT_URL_FORMAT, number);
	}
	ret = http_get(detect_url_1, http_data);
	string_line_format(http_data.buffer);

	printf("detect URL 1: %s, response: %s\n", detect_url_1, http_data.buffer.c_str());

	weight = get_ss_weight(http_data.buffer);
	if (weight == -1){
		http_data.buffer.clear();
		if (!gcfg.num_detect_url_2.empty() && gcfg.num_detect_url_2.size() > 7){
			snprintf(detect_url_2, 1024, gcfg.num_detect_url_2.c_str(), number);
		}
		else{
			snprintf(detect_url_2, 1024, DETECT_URL_FORMAT_2, number);
		}
		
		ret = http_get(detect_url_2, http_data);
		weight = get_content_weight(http_data.buffer);
	}
	printf("mail weight: %d\n", weight);
	return weight;
}

int multi_qzone_detect(vector<number_info_t> &nis)
{
	int len = nis.size();
	int w;
	for (int i = 0; i < len; i++)
	{
		w = qzone_detect(nis[i].number.c_str());
		if (w > 0) nis[i].weight = w;
	}

	// test
	printf("No.\tWeight\n");
	for (int i = 0; i < len; i++)
	{
		printf("%s\t%d\n", nis[i].number.c_str(), nis[i].weight);
	}

	return 0;
}


/*
*	通过“说说”的数量来计算权重
*/
int get_ss_weight(string &html)
{
	string::size_type pos1, pos2;
	pos1 = html.find("SS");
	if (pos1 == string::npos) return -1;

	pos2 = html.find(",", pos1);
	if (pos2 == string::npos) return -1;

	int weight = 1;
	string sub = html.substr(pos1 + 4, pos2 - pos1 - 4);
	int ss = atoi(html.substr(pos1 + 4, pos2 - pos1 - 4).c_str());

	if (ss >= 10) weight++;
	if (ss >= 100) weight++;

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
	if (pos1 != string::npos) return 2;
	if (pos2 != string::npos) return 0;
	return 0;
}

int get_detecting_numbers(vector<number_info_t> &nis, const char *user, const char *url)
{
	int ret = 0;
	char url_str[1024];
	const char *p_url = NULL;
	char *pQQs = NULL;
	data_t http_data;

	do{
		if (url == NULL){
			snprintf(url_str, 1024, GET_NUMBERS_FORMAT, user, VERSION);
			p_url = url_str;
		}
		else{
			p_url = url;
		}

		ret = http_get(p_url, http_data);
		if (ret < 0){
			ret = -1; break;
		}

		cJSON *json, *item;
		json = cJSON_Parse(http_data.buffer.c_str());
		if (!json){
			//Logging(E_LOG_ERROR, "JSON parse error [%s]\n", cJSON_GetErrorPtr());
			ret = -2; break;
		}

		item = cJSON_GetObjectItem(json, "QQs");
		if (!json){
			//Logging(E_LOG_ERROR, "Get array item of JSON error [%s]\n", cJSON_GetErrorPtr());
			ret = -3; break;
		}

		pQQs = cJSON_Print(item);
		//printf("QQs 1: %s\n", pQQs);
		if (pQQs == NULL){
			//Logging(E_LOG_ERROR, "Get item value of JSON error [%s]\n", cJSON_GetErrorPtr());
			ret = -4; break;
		}
		int len = strlen(pQQs);
		if (len > 0 && pQQs[0] == '"'){
			pQQs++;
			if (pQQs[len - 2] == '"') pQQs[len - 2] = '\0';
		}
		//printf("QQs 2: %s\n", pQQs);
		numbers_split(nis, pQQs);

	} while (0);

	return ret;
}

int upload_detected_result(vector<number_info_t> &nis, const char *user, const char *url)
{
	char *buf = NULL;
	int len = nis.size();
	int ret;

	char *post_url = NULL;
	const char *p_url = NULL;
	data_t http_data;

	int buf_len = 30 * len + 100; // 估算内存大小，每个QQ的信息大小大概是30个字节
	//Logging(E_LOG_DEBUG, "1 buffer len: %d, bufp: %p, errno: %d\n", buf_len, buf, errno);
	while (buf == NULL){
		buf = new char[buf_len];
		if (buf == NULL) Logging(E_LOG_ERROR, "new buffer for detected result failed");
	}
	//Logging(E_LOG_DEBUG, "2 buffer len: %d, bufp: %p, errno: %d\n", buf_len, buf, errno);
	int buf_size = 0;
	buf_size = snprintf(buf, buf_len, "result={\"version\":\"%s\",\"result\":[", VERSION);
	//Logging(E_LOG_DEBUG, "buffer 1: %s\n", buf);

	int flag = 0;
	int total = len;
	int active = 0;
	for (int i = 0; i < len; i++)
	{
		if (nis[i].weight > 0) active++;
		if (flag == 0){
			buf_size += snprintf(buf + buf_size, buf_len - buf_size, "{\"qq\":\"%s\",\"weight\":%d}", nis[i].number.c_str(), nis[i].weight);
			flag = 1;
		}
		else{
			buf_size += snprintf(buf + buf_size, buf_len - buf_size, ",{\"qq\":\"%s\",\"weight\":%d}", nis[i].number.c_str(), nis[i].weight);
		}
	}
	buf_size += snprintf(buf + buf_size, buf_len - buf_size, "]}");
	buf[buf_size] = '\0';

	//Logging(E_LOG_INFO, "active number: %d(%d), upload json:\n%s\n", total, active, buf);
	Logging(E_LOG_INFO, "active number: %d(%d)\n", total, active);


	if (url == NULL){
		post_url = new char[1024];
		snprintf(post_url, 1024, UPLOAD_RESULT_FORMAT, user, VERSION);
		p_url = post_url;
	}
	else{
		p_url = url;
	}

	ret = http_post(p_url, buf, http_data);
	//Logging(E_LOG_INFO, "upload respone:\n%s\n", http_data.buffer.c_str());
	if (buf) delete[] buf;
	if (post_url) delete[] post_url;

	return ret;
}

