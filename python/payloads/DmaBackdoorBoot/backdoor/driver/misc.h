
extern "C"
{

void clear_wp(void);
void set_wp(void);

}

BOOLEAN EqualUnicodeString_r(PUNICODE_STRING Str1, PUNICODE_STRING Str2, BOOLEAN CaseInSensitive);
BOOLEAN GetProcessFullImagePathNative(PCLIENT_ID ClientId, PVOID ImagePath, ULONG ImagePathBuffSize);
BOOLEAN DumpToFile(PUNICODE_STRING FileName, PVOID Data, ULONG DataSize);
BOOLEAN DeleteFile(PUNICODE_STRING usFileName);
BOOLEAN ReadFromFile(PUNICODE_STRING FileName, PVOID *Data, PULONG DataSize);
