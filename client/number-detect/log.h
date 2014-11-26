#ifndef _HEDADER_LOG_
#define _HEDADER_LOG_

#include <stdio.h>

#define LOG_BUF_SIZE 2048
enum E_LOG_LEVEL
{
	E_LOG_ERROR = 0,
	E_LOG_WARNNING,
	E_LOG_INFO,
	E_LOG_DEBUG
};

int OpenLog(const char *logfile, int log_level);
int Logging(int level, const char *format, ...);
int CloseLog();

#endif
