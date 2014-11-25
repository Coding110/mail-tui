#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int line_string_clean(char *line, int line_size);
int config_show(config_t &cfg);

int read_config(const char *cfgfile, config_t &cfg)
{
	if(cfgfile == NULL) return -1;

	FILE *fp = fopen(cfgfile, "r");
	if(fp == NULL) return -2;

	char read_buf[1024], key[1024], value[1024];
	int i = 1;
	int buf_size = 0;
	while(fgets(read_buf, 1024, fp)){
		buf_size = strlen(read_buf);
		line_string_clean(read_buf, buf_size);
		if(buf_size < 3 || read_buf[0] == '#') continue;
		
		key[0] = 0;
		value[0] = 0;
		sscanf(read_buf, "%[^=]=%s", key, value);
		//printf("line %2d: size(%d), %s\n", i++, buf_size, read_buf);
		//printf("\t%s\t%s\n", key, value);

		if(memcmp(key, "num_detect_url_1", 16) == 0){
			cfg.num_detect_url_1 = value;
		}else if(memcmp(key, "num_detect_url_2", 16) == 0){
			cfg.num_detect_url_2 = value;
		}else if(memcmp(key, "num_get_url ", 11) == 0){
			cfg.get_num_url = value;
		}else if(memcmp(key, "num_upload_url", 14) == 0){
			cfg.upload_num_url = value;
		}else if(memcmp(key, "thread_count", 12) == 0){
			cfg.thread_count = atoi(value);
		}else{
		}
	}

	return 0;
}

/*
 *	clean ' ', '\n', '\r' in the string
 */
int line_string_clean(char *line, int line_size)
{
	int j = 0;
	for(int i = 0; i < line_size; i++)
	{
		if(line[i] == ' ' || line[i] == '\n' || line[i] == '\r') continue;
		if(i > j) line[j] = line[i];
		j++;
	}
	line[j] = '\0';

	return 0;
}

int config_show(config_t &cfg)
{
	printf("------ Config show ------\n");
	printf("num_detect_url_1: %s\n", cfg.num_detect_url_1.c_str());
	printf("num_detect_url_2: %s\n", cfg.num_detect_url_2.c_str());
	printf("     get_num_url: %s\n", cfg.get_num_url.c_str());
	printf("  upload_num_url: %s\n", cfg.upload_num_url.c_str());
	printf("    thread_count: %d\n", cfg.thread_count);
	return 0;
}


