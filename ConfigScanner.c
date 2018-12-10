#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ConfigScanner.h"
#include "MetaScanner.h"
#include "Helpers.h"

struct configData scanConfig(char* fileName)	// Main function that scans the given config file
{
	int lineCounter = 0;
	char currentValue;
	
	FILE *configFile = fopen(fileName, "r");
	struct configData *savedConfig = malloc(sizeof(struct configData));
	
	for(int index = 1; ;index++)
	{
		if((currentValue = fgetc(configFile)) == EOF)	// Will stop loop once the end of file is reached
		{
			break;
		}
		if(currentValue == ':')		// Looking for the colon
		{
			if(lineCounter == 0)	// "If" statements to cover all individual pieces of data
			{
				currentValue = fgetc(configFile);
				currentValue = fgetc(configFile);
				savedConfig->version = currentValue;				
				lineCounter++;
			}
			else if(lineCounter == 1)
			{
				while(currentValue != '\n')
				{
					if(currentValue != ' ')
					{
						append(savedConfig->filePath, currentValue);
						currentValue = fgetc(configFile);
					}
					else
					{
						currentValue = fgetc(configFile);
						
					}
				}
				memmove(savedConfig->filePath, savedConfig->filePath+1, strlen(savedConfig->filePath));
				lineCounter++;
			}
			else if(lineCounter == 2)
			{
				currentValue = fgetc(configFile);
				//currentValue = fgetc(configFile);
				if(currentValue == 'N')
				{
					append(savedConfig->scheduleCode, 'F');
					append(savedConfig->scheduleCode, 'F');
					append(savedConfig->scheduleCode, 'C');
					append(savedConfig->scheduleCode, 'F');
					append(savedConfig->scheduleCode, 'S');
					append(savedConfig->scheduleCode, '-');
					append(savedConfig->scheduleCode, 'N');
				}
				else
				{
					while(currentValue != '\n')
					{
						append(savedConfig->scheduleCode, currentValue);
						currentValue = fgetc(configFile);
					}
				}
					
				memmove(savedConfig->scheduleCode, savedConfig->scheduleCode+1, strlen(savedConfig->scheduleCode));
				if(strcmp(savedConfig->scheduleCode, "NONE") == 0)
				{
					memset(savedConfig->scheduleCode, 0, sizeof(savedConfig->scheduleCode));
					append(savedConfig->scheduleCode, 'F');
					append(savedConfig->scheduleCode, 'C');
					append(savedConfig->scheduleCode, 'F');
					append(savedConfig->scheduleCode, 'S');
					append(savedConfig->scheduleCode, '-');
					append(savedConfig->scheduleCode, 'N');
				}
				lineCounter++;
			}
			else if(lineCounter == 3)
			{
				while(currentValue != '\n')
				{
					currentValue = fgetc(configFile);
					append(savedConfig->qTimeTemp, currentValue);
				}
				append(savedConfig->qTimeTemp, '\0');
				savedConfig->qTime = atoi(savedConfig->qTimeTemp);
				lineCounter++;
			}
			else if(lineCounter == 4)
			{
				while(currentValue != '\n')
				{
					currentValue = fgetc(configFile);
					append(savedConfig->memTemp, currentValue);
				}
				memmove(savedConfig->memTemp, savedConfig->memTemp+1, strlen(savedConfig->memTemp));
				savedConfig->availableMem = atoi(savedConfig->memTemp);
				lineCounter++;
			}
			else if(lineCounter == 5)
			{
				currentValue = fgetc(configFile);
				while(currentValue != '\n')
				{
					currentValue = fgetc(configFile);
					append(savedConfig->pCycleTemp, currentValue);
				}
				memmove(savedConfig->pCycleTemp, savedConfig->pCycleTemp+1, strlen(savedConfig->pCycleTemp));
				savedConfig->PCycleTime = atoi(savedConfig->pCycleTemp);
				lineCounter++;
			}
			else if(lineCounter == 6)
			{
				while(currentValue != '\n')
				{
					currentValue = fgetc(configFile);
					append(savedConfig->IOCycleTemp, currentValue);
				}
				memmove(savedConfig->IOCycleTemp, savedConfig->IOCycleTemp+1, strlen(savedConfig->IOCycleTemp));
				savedConfig->IOCycleTime = atoi(savedConfig->IOCycleTemp);
				lineCounter++;
			}
			else if(lineCounter == 7)
			{
				while(currentValue != '\n')
				{
					if(currentValue != ' ')
					{
						append(savedConfig->logTo, currentValue);
						currentValue = fgetc(configFile);
					}
					else
					{
						currentValue = fgetc(configFile);
						
					}
				}
				memmove(savedConfig->logTo, savedConfig->logTo+1, strlen(savedConfig->logTo));
				lineCounter++;
			}
			else if(lineCounter == 8)
			{
				while(currentValue != '\n')
				{
					if(currentValue != ' ')
					{
						append(savedConfig->logFilePath, currentValue);
						currentValue = fgetc(configFile);
					}
					else
					{
						currentValue = fgetc(configFile);
						
					}
				}
				memmove(savedConfig->logFilePath, savedConfig->logFilePath+1, strlen(savedConfig->logFilePath));
				lineCounter++;
			}
		}
	}
	fclose(configFile);
	free(savedConfig);
	return *savedConfig;
}

