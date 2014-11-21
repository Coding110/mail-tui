#include <stdio.h>
#include <curl/curl.h>
#include "common.h"

int memcpy_no_blank(char *src, size_t src_size, string &dst)
{
	char *buf = new char[src_size];
	size_t pos = 0;
	for(int i; i < src_size; i++){
		if(src[i] != ' ' && src[i] != '\r' && src[i] != '\n' && src[i] != '\t') buf[pos++] = src[i];
	}
	buf[pos] = '\0';
	dst = buf;
	delete[] buf;
	return pos;
}

// delete ' ', '\r', '\n' , '\t'
int string_line_format(string &src)
{
	size_t src_size = src.size();
	char *buf = new char[src_size];
	size_t pos = 0;
	for(int i; i < src_size; i++){
		if(src[i] != ' ' && src[i] != '\r' && src[i] != '\n' && src[i] != '\t') buf[pos++] = src[i];
	}
	buf[pos] = '\0';
	src = buf;
	delete[] buf;
	return pos;
}

int http_get(char *url, data_t &http_data)
{
	int ret = 0;
	

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    //headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init();
    if (curl)
    {
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &http_data); 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_data);
        res = curl_easy_perform(curl);   // 执行
        if (res != 0) {
			ret = -1;
			printf("curl error: %s\n", curl_easy_strerror(res));
        }else{
		}
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return 0;
}

size_t http_write_data(void *ptr, size_t size, size_t nmemb, void *args)
{
	size_t total = size*nmemb;
	if(args == NULL) return total;

	data_t *p_curl_data = (data_t*)args; 
	//memcpy_no_blank((char*)ptr, total, p_curl_data->buffer);
	//printf("%s", p_curl_data->buffer.c_str());
	p_curl_data->buffer += string((char*)ptr, total);
	//printf("%s", p_curl_data->buffer.c_str());
	return total;
}

bool http_post(char *url)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "&logintype=uid&u=xieyan&psw=xxx86");    // 指定post内容
        curl_easy_setopt(curl, CURLOPT_URL, url);   // 指定url
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    return true;
}

