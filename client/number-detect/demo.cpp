#include <stdio.h>
#include "detect.h"

int main(int argc, char *argv[])
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
	multi_qzone_detect(nis);
	upload_detected_result(nis, "hua", upload_url);

	return 0;
}
