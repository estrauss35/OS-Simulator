#include <stdio.h>
#include <string.h>
#include "Helpers.h"
#include "ConfigScanner.h"
#include "MetaScanner.h"

// Handles calling the functions
int main(int argc, char *argv[])
{
	if(argc > 2)
	{
		printf("Too many arguments.\n");
	}
	else if(argc == 1)
	{
		printf("Enter Config File Name.\n");
	}
	else
	{
		printf("Operating System Simulator\n==========================\n\n");
		printf("Loading Config File...\n");
		struct configData configInfo = scanConfig(argv[1]);
		printf("Loading Metadata File...\n\n");
		metaDataScanner(configInfo.filePath, configInfo.scheduleCode, configInfo.logTo, configInfo.logFilePath, configInfo.IOCycleTime, configInfo.qTime);
		
	}
}
