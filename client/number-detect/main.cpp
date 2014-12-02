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

extern config_t gcfg;

int easy_detect();

#ifndef WIN32

int main(int argc, char *argv[])
{
	time_t time_now = time(NULL);
	struct tm mt;
	localtime_r(&time_now, &mt);

	//OpenLog("detect.log", E_LOG_DEBUG);
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
	int ret = 0;
	printf("easy detect now.\n");
	if (read_config("config.ini") < 0)
		return -1;
	//config_show(gcfg);

	OpenLog("detect.log", gcfg.log_level);

	vector<number_info_t> nis;
	int nis_len;
	CThdPool thd_pool;
	thd_pool.PoolInit(gcfg.thread_count, detect_callback);

	while (true)
	{
		if (get_detecting_numbers(nis, gcfg.username.c_str(), gcfg.get_num_url.c_str()) < 0){
			sleep(2);
			//Logging(E_LOG_DEBUG, "No task to do.");
			continue;
		}
		nis_len = nis.size();
		Logging(E_LOG_DEBUG, "got %d numbers", nis_len);
		//printf("got %d numbers\n", nis_len);
		for (int i = 0; i < nis_len; i++){
			thd_pool.AddTask(&nis[i]);
		}
		//Logging(E_LOG_DEBUG, "start lazy join");
		thd_pool.LazyJoin();
		//Logging(E_LOG_DEBUG, "end lazy join");
		Logging(E_LOG_DEBUG, "%d numbers detecting finished", nis_len);
		//printf("%d numbers detecting finished\n", nis_len);
		while (true){
			ret = upload_detected_result(nis, gcfg.username.c_str(), gcfg.upload_num_url.c_str());
			if (ret == 0) break;
			sleep(10); // 如果上传失败，重新上传，直到上传成功。
		}
		Logging(E_LOG_DEBUG, "%d numbers detected result upload finished", nis_len);
		//printf("%d numbers detected result upload finished\n", nis_len);
		nis.clear();
		//break;
	}

	return 0;
}