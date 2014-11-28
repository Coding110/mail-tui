#include <stdio.h>
#include "detect.h"
#include "config.h"
#include "thdpool.h"
#include "log.h"

#ifdef WIN32
#include "stdafx.h"
#else
#include <unistd.h>
#endif

int easy_detect();

#ifndef WIN32

int main(int argc, char *argv[])
{
	time_t time_now = time(NULL);
    struct tm mt;
    localtime_r(&time_now, &mt);

	OpenLog("detect.log", E_LOG_DEBUG);
	easy_detect();
	return 0;
}

#endif

void detect_callback(void *arg)
{
	number_info_t *ni;
	ni = (number_info_t*)arg;
	ni->weight = qzone_detect(ni->number.c_str());
}

int easy_detect()
{
	printf("easy detect now.\n");
	config_t cfg;
	if(read_config("config.ini", cfg) < 0)
		return -1;
	config_show(cfg);

	OpenLog("detect.log", cfg.log_level);

	vector<number_info_t> nis;
	int nis_len;
	CThdPool thd_pool;
	thd_pool.PoolInit(cfg.thread_count, detect_callback);

	//printf("easy detect starting 1.\n");
	Logging(E_LOG_DEBUG, "easy detect starting.");
	//printf("easy detect starting 2.\n");

	while(true)
	{
		if(get_detecting_numbers(nis, cfg.username.c_str(), cfg.get_num_url.c_str()) < 0) sleep(2);
		nis_len = nis.size();
		Logging(E_LOG_DEBUG, "got %d numbers", nis_len);
		//printf("got %d numbers\n", nis_len);
		for(int i = 0; i < nis_len; i++){
			thd_pool.AddTask(&nis[i]);
		}
		Logging(E_LOG_DEBUG, "start lazy join");
		thd_pool.LazyJoin();
		Logging(E_LOG_DEBUG, "end lazy join");
		Logging(E_LOG_DEBUG, "%d numbers detecting finished", nis_len);
		//printf("%d numbers detecting finished\n", nis_len);
		upload_detected_result(nis, cfg.username.c_str(), cfg.upload_num_url.c_str());
		Logging(E_LOG_DEBUG, "%d numbers detected result upload finished", nis_len);
		//printf("%d numbers detected result upload finished\n", nis_len);
		nis.clear();
		//break;
	}

	return 0;
}

