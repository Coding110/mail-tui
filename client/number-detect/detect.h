#ifndef _DETECT_H_
#define _DETECT_H_

#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

#include "common.h"

int get_detecting_numbers(vector<number_info_t> &nis, const char *user = "selfless guy", const char *url = NULL);
int upload_detected_result(vector<number_info_t> &nis, const char *user = "selfless guy", const char *url = NULL);

int qzone_detect(const char *number);
int multi_qzone_detect(vector<number_info_t> &nis);

// do all the detecting work
int easy_detect();

#endif

