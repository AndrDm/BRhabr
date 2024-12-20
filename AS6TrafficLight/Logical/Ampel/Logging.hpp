#define MAX_LOG_ENTRIES 200 // ca 16KB Log - Default 200 Entries. 
							// Can be increased, but don't forget to allocate TraceMessages Variable!

void ClearTraceLog()
{
	for (int i=0; i<MAX_LOG_ENTRIES; i++) strcpy(TraceMessages[i], "");
}

void TraceMessage(const char* Message)
{
	for (int i=0; i<MAX_LOG_ENTRIES; i++){
		strcpy(TraceMessages[i], TraceMessages[i+1]);
	}
	strcpy(TraceMessages[MAX_LOG_ENTRIES], Message);
}
