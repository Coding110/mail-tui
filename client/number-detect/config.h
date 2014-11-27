#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
//#include <vector>
#include <cstring>
using namespace std;

typedef struct _config_t
{
	int thread_count;
	string num_detect_url_1;
	string num_detect_url_2;
	string get_num_url;
	string upload_num_url;
	string username;
	int log_level;
}config_t;

int read_config(const char *cfgfile, config_t &cfg);
int config_show(config_t &cfg);

#endif

