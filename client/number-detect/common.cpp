#include <stdio.h>
#include <curl/curl.h>
#include "common.h" 
#include "log.h"

#pragma comment(lib, "libcurl.lib")

size_t http_write_data(void *ptr, size_t size, size_t nmemb, void *args);

int memcpy_no_blank(char *src, size_t src_size, string &dst)
{
	char *buf = new char[src_size];
	size_t pos = 0;
	for(size_t i = 0; i < src_size; i++){
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
	for (size_t i = 0; i < src_size; i++){
		if(src[i] != ' ' && src[i] != '\r' && src[i] != '\n' && src[i] != '\t') buf[pos++] = src[i];
	}
	buf[pos] = '\0';
	src = buf;
	delete[] buf;
	return pos;
}

int http_get(const char *url, data_t &http_data)
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
			//Logging(E_LOG_ERROR, "curl error: %s, url: %s\n", curl_easy_strerror(res), url);
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

int http_post(const char *url, char *form_data, data_t &http_data)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
	int ret = 0;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, form_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &http_data); 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_data);
        res = curl_easy_perform(curl);
        if (res != 0) {
			ret = -1;
			Logging(E_LOG_ERROR, "curl error: %s, url: %s\n", curl_easy_strerror(res), url);
        }else{
		}
    }
    curl_easy_cleanup(curl);
    return ret;
}

int numbers_split(vector<number_info_t> &nis, char *numbers)
{
    number_info_t ni;

    char *p = numbers, *key_point;
    //char key[1024], value[1024];

    while(p)
    {
        while ( key_point = strsep(&p,","))
        {
            if (*key_point == 0)
                continue;
            else
                break;
        }

        ni.number = key_point;
        ni.weight = 0;
        nis.push_back(ni);
        //printf("string: %s, key: %s, value: %s\n",key_point, key, value);
    }
	return 0;
}

#ifdef WIN32
char *strsep(char **stringp, const char *delim)
{
	char *p;

	if (!stringp)
		return(NULL);
	p = *stringp;
	while (**stringp && !strchr(delim, **stringp))
		(*stringp)++;
	if (**stringp)
	{
		**stringp = '\0';
		(*stringp)++;
	}
	else
		*stringp = NULL;
	return(p);
}
#endif
