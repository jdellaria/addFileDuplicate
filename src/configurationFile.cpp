/*
 * configurationFile.cpp
 *
 *  Created on: Sep 10, 2009
 *      Author: jdellaria
 */

#include <sys/stat.h>
#include <sys/dir.h>
#include <errno.h>
#include <dirent.h>
//#include <string>
//#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "configurationFile.h"
#include <tinyxml.h>
#include <DLog.h>
//#include <ctime>

using namespace std;
//#pragma hdrstop

extern DLog myLog;
extern fileAction myFileAction;
extern bool addToDatabase; // add file to database


configurationFile::configurationFile()
{
	port = 0;
	logFileName.clear();
	logPrintScreen = true;
	logPrintFile = false;
	logPrintTime = true;
	serverName.clear();
	hostFileDirectory.clear();
}

configurationFile::~configurationFile()
{
	// TODO Auto-generated destructor stub
}

// Read a file into a vector

void configurationFile::help(char *program) {
	std::cout << program;
	std::cout << ": Need a filename for a parameter.\n";
}

