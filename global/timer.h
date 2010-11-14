#ifndef TIMER_H
#define TIMER_H

#include <global\globref.h>
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

static FILETIME tCurr;
static FILETIME tElapsed;
static FILETIME tStart;
static SYSTEMTIME lt;

extern ofstream gloLog;

static void FilePrintTimestamp();
static void PrintToLog(const char* prnErr);

static void initTime()
{
	GetSystemTimeAsFileTime(&tStart);
	PrintToLog("Begin Initialization");
}

static void FilePrintTimestamp()
{
	if(gloLog.is_open());
	else
		gloLog.open("data\\log.txt",ios_base::app);

	char* localTime = NULL;
	GetLocalTime(&lt);
	localTime = (char *)malloc(sizeof("%02d/%02d/%04d %02d:%02d:%02d:%03d")+1);
	// Copy Full Date in Format MM/DD/YYYY HH:MM:SS:mmm
	sprintf(localTime,"%02d/%02d/%04d %02d:%02d:%02d:%03d",
			lt.wMonth, lt.wDay, lt.wYear, 
			lt.wHour, lt.wMinute,lt.wSecond, lt.wMilliseconds);
	gloLog << localTime;
	// Return Timestamp
	return;
}

static long elapsed()
{
	GetSystemTimeAsFileTime(&tCurr);
	tElapsed.dwHighDateTime = tCurr.dwHighDateTime - tStart.dwHighDateTime;
	tElapsed.dwLowDateTime = tCurr.dwLowDateTime - tStart.dwLowDateTime;

	return tElapsed.dwLowDateTime;
}


// Print Log
static void PrintToLog(const char* prnErr)
{
		FilePrintTimestamp();
		//prnLog = (char *)malloc(sizeof(prnErr)+3);
		//sprintf(prnLog,"%s",prnErr);
		gloLog << "\t \t" << prnErr << endl;
}

#endif