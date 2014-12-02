#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>
#include "log.h"
#include "common.h"
#ifdef WIN32
#include "stdafx.h"
#pragma comment(lib, "pthreadVC2.lib")
#endif

static FILE *flog = NULL;
static char slogfile[256];
static int slog_level = 0;
static pthread_mutex_t log_mutex;

static int log_model = M_LOG_FILE;
static LoggingHandle floghd = NULL;
static void *pHandleData = NULL;

char *gettime(char *timestr);

int OpenLog(const char *logfile, int log_level)
{
	if (log_model == M_LOG_FILE){
#ifdef WIN32
		errno_t en = fopen_s(&flog, logfile, "a");
		if (en){
			fprintf(stderr, "Log file open failed, file: %s, errno: %d\n", logfile, en);
			return -1;
		}
#else
		flog = fopen(logfile, "a"); 
		if (flog == NULL){
			fprintf(stderr, "Log file open failed, file: %s, errno: %d\n", logfile, errno);
			return -1;
		}
#endif
	}
	slog_level = log_level;
	pthread_mutex_init(&log_mutex, NULL);
	return 0;
}

int SetLogModel(int model, LoggingHandle flhd, void *arg)
{
	log_model = model;
	floghd = flhd;
	pHandleData = arg;
	return 0;
}

int CloseLog()
{
	if(flog) fclose(flog);
	pthread_mutex_destroy(&log_mutex);
	return 0;
}

int Logging(int level, const char *format, ...)
{
	if (flog == NULL && floghd == NULL){
		fprintf(stderr, "Can't write log, the log file have not open.\n");
		return -1;
	}
	
	if(level > slog_level) return 0;
	if(level < E_LOG_ERROR) return -1;

	pthread_mutex_lock(&log_mutex);

	va_list arglst;
	va_start(arglst, format);

	int nb = 0;
	//char logbuf[LOG_BUF_SIZE] = {0};
	static char *logbuf = NULL, *logbuf2 = NULL;
	if (logbuf == NULL) logbuf = new char[LOG_BUF_SIZE];
	if (logbuf2 == NULL) logbuf2 = new char[LOG_BUF_SIZE];
	char timestr[24];
	switch(level){
	case E_LOG_ERROR:
		snprintf(logbuf, LOG_BUF_SIZE, "%s -ERROR- %s\n", gettime(timestr), format);
		break;
	case E_LOG_WARNNING:
		snprintf(logbuf, LOG_BUF_SIZE, "%s -WARNNING- %s\n", gettime(timestr), format);
		break;
	case E_LOG_INFO:
		snprintf(logbuf, LOG_BUF_SIZE, "%s -INFO- %s\n", gettime(timestr), format);
		break;
	case E_LOG_DEBUG:
		snprintf(logbuf, LOG_BUF_SIZE, "%s -DEBUG- %s\n", gettime(timestr), format);
		break;
	default:
		break;
	}

	if (log_model == M_LOG_DLG && floghd){ // 这种模式在MFC中有问题，无法使用
#ifdef WIN32
		vsnprintf_s(logbuf2, LOG_BUF_SIZE, LOG_BUF_SIZE, logbuf, arglst);
#else
		vsnprintf(logbuf2, LOG_BUF_SIZE, logbuf, arglst);
#endif
		floghd(logbuf2, pHandleData);
	}else{
		nb = vfprintf(flog, logbuf, arglst);
		//fflush(flog);
	}
	pthread_mutex_unlock(&log_mutex);

	return 0;
}

char *gettime(char *timestr) // 'timestr's length at least 20 bytes.
{
#ifdef WIN32
	SYSTEMTIME stm;
	GetLocalTime(&stm);
	snprintf(timestr, 20, "%04d-%02d-%02d %02d:%02d:%02d",
		stm.wYear, stm.wMonth, stm.wDay,
		stm.wHour, stm.wMinute, stm.wSecond);
	return timestr;
#else
    time_t time_now = time(NULL);
    struct tm mt;
    localtime_r(&time_now, &mt);
    mt.tm_year += 1900;
    sprintf(timestr, "%04d-%02d-%02d %02d:%02d:%02d",
        mt.tm_year, mt.tm_mon, mt.tm_mday,
        mt.tm_hour, mt.tm_min, mt.tm_sec);
    return timestr;
#endif
}
