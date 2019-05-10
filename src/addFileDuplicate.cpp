//============================================================================
// Name        : deleteDuplicates.cpp
// Author      : Jon Dellaria
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <cstring>
#include <File.h>
#include <Directory.h>
#include <DLog.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>

#include "configurationFile.h"
#include "coopDB.h"

using namespace std;
DLog myLog;
configurationFile myConfig;
coopDB myDB;

fileAction myFileAction = FILE_ACTION_NOTHING;

bool addToDatabase = true; // add file to database



int findDuplicates(string originalFileName);

bool cmdOptionExists(char** begin, char** end, const string& option);
int executeFileActionOption(string originalFileName, string serverFileName);

bool cmdOptionExists(char** begin, char** end, const string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char* argv[])
{
	Directory myDirectory;
	string message;

	char *myFileName;

	myLog.printFile = false;

// Set the action defaults
    if(cmdOptionExists(argv, argv+argc, "-delete")||cmdOptionExists(argv, argv+argc, "-d"))
    {
    	myFileAction = FILE_ACTION_DELETE;
    }
    if(cmdOptionExists(argv, argv+argc, "-hard")||cmdOptionExists(argv, argv+argc, "-h"))
    {
    	myFileAction = FILE_ACTION_HARD_LINK;
    }
    if(cmdOptionExists(argv, argv+argc, "-symbolic")||cmdOptionExists(argv, argv+argc, "-s"))
    {
    	myFileAction = FILE_ACTION_SYMBOLIC_LINK;
    }
    if(cmdOptionExists(argv, argv+argc, "-noadd")||cmdOptionExists(argv, argv+argc, "-n")) // will not add new files to the database
    {
    	addToDatabase = false;
    }
    else
    {
    	addToDatabase = true;
    }

    if( argc > 1 )
    {
        myFileName = argv[argc-1]; // assume last argument is the filename
    	myDB.OpenConnection();
    	myDB.setServerConnection(1);
    	findDuplicates(myFileName);
    	myDB.CloseConnection();
    }
    else
    {
		message.clear();
		message.append("No File Name received to perform activity.");
		myLog.print(logError, message);
    }
}

int findDuplicates(string originalFileName)
{
	File myFile;
	char originalFileHash[36];
	string destinationFileName;
	string destinationVersionFileName;
	string message;

	myFile.getMD5Hash((char* const)originalFileName.c_str(),originalFileHash);

	if (myDB.doesHashExistInDataBase(originalFileHash))
	{
		if (myDB.getServerFile(originalFileHash) >= 0)
		{
			executeFileActionOption(originalFileName,myDB.serverDirectoryEntry);
		}
	}
	else
	{
		myDB.setServerDirectoryEntry(originalFileName.c_str());
		myDB.setMD5Hash(originalFileHash);
		myDB.addHashFile();
	}
	return 0;
}

int executeFileActionOption(string originalFileName, string serverFileName)
{
	File myFile;
	string message;

	if(originalFileName.compare(serverFileName) == 0)
	{
		message.clear();
		message.append("executeFileActionOption: File is original file. File action not performed:" + originalFileName);
		myLog.print(logWarning, message);
		return(0);
	}

	if(myFileAction ==  FILE_ACTION_DELETE)
	{
		if (myFile.remove(originalFileName.c_str()) == 0) // if delete file succeeds
		{
			message = "Deleting file:"; // log the event
			message.append(originalFileName);
			myLog.print(logWarning, message);
		}
		else
		{
			message = "Error deleting file:"; // report error if file is not deleted
			message.append(myFile.errorMessageBuffer);
			message.append(": ");
			message.append(originalFileName);
			myLog.print(logError, message);
		}
	}
	if(myFileAction ==  FILE_ACTION_HARD_LINK)
	{
		message.clear();
		message.append("Replacing File:");
		message.append(originalFileName);
		message.append(": with a hard link");
		myLog.print(logWarning, message);
		if (myFile.remove(originalFileName.c_str()) == 0) // if delete file succeeds
		{
			message = "Deleting file:"; // log the event
			message.append(originalFileName);
			myLog.print(logWarning, message);
		}
		else
		{
			message = "Error deleting file:"; // report error if file is not deleted
			message.append(myFile.errorMessageBuffer);
			message.append(": ");
			message.append(originalFileName);
			myLog.print(logError, message);
		}
		myFile.createHardLink(serverFileName.c_str(),originalFileName.c_str());
	}
	if(myFileAction ==  FILE_ACTION_SYMBOLIC_LINK)
	{
		message.clear();
		message.append("Replacing File:");
		message.append(originalFileName);
		message.append(": with a symbolic link");

		myLog.print(logWarning, message);
		if (myFile.remove(originalFileName.c_str()) == 0) // if delete file succeeds
		{
			message = "Deleting file:"; // log the event
			message.append(originalFileName);
			myLog.print(logWarning, message);
		}
		else
		{
			message = "Error deleting file:"; // report error if file is not deleted
			message.append(myFile.errorMessageBuffer);
			message.append(": ");
			message.append(originalFileName);
			myLog.print(logError, message);
		}
		myFile.createSoftLink(serverFileName.c_str(),originalFileName.c_str());
	}
	return(1);
}
