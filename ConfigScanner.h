struct configData
{
	int version;
	char filePath[20];
	char scheduleCode[20];
	char qTimeTemp[5];
	int qTime;
	char memTemp[10];
	int availableMem;
	char pCycleTemp[5];
	int PCycleTime;
	char IOCycleTemp[10];
	int IOCycleTime;
	char logTo[20];
	char logFilePath[20];
};

struct configData scanConfig(char* fileName);
