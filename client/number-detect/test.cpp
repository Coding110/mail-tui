#include <stdio.h>
#include "detect.h"

int main(int argc, char *argv[])
{
	if(argc != 2){
		fprintf(stderr, "Usage: %s <number>\n", argv[0]);
		return 1;
	}

	qzone_detect(argv[1]);

	return 0;
}
