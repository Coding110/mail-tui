#include <stdio.h>
#include "detect.h"
#include "config.h"

int detect_test(int argc, char *argv[]);
int config_test(int argc, char *argv[]);

#ifndef WIN32
int main(int argc, char *argv[])
{
	//detect_test(argc, argv);
	config_test(argc, argv);
	return 0;
}
#endif

int config_test(int argc, char *argv[])
{
	config_t g_cfg;
	//ead_config("config.ini", g_cfg);
	config_show(g_cfg);
	return 0;
}

int detect_test(int argc, char *argv[])
{

	// test 1
	//if(argc != 2){
	//	fprintf(stderr, "Usage: %s <number>\n", argv[0]);
	//	return 1;
	//}

	//qzone_detect(argv[1]);


	// test 2
	const char *get_num_url = NULL;//"http://192.168.0.81/www/nums.php";
	const char *upload_url = NULL;//"http://192.168.0.81/www/upload.php";
	vector<number_info_t> nis;
	get_detecting_numbers(nis, "hua", get_num_url);
	printf("get detectiong numbers completed, count numbers: %d\n", nis.size());
	multi_qzone_detect(nis);
	printf("detect completed\n");
	upload_detected_result(nis, "hua", upload_url);
	printf("upload detected result completed\n");

	return 0;
}
