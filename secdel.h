#ifndef SECDEL_H
#define SECDEL_H

#include <windows.h>
#include <tchar.h>

#define MEGABYTE 1048576
typedef unsigned char BYTE;

/*----------------------------------------------------
Class Definition
----------------------------------------------------*/
class inputFile {
	LPCTSTR filename;
	DWORD HI32, LO32, byteswritten;
public:
	HANDLE hInput;
	inputFile(LPCTSTR);
	void openFile();
	DWORD resetFilePointer();
	__int64 getInputInfo();
	bool writeToFile(BYTE*, __int64);
	bool renameFile(LPCTSTR, LPCTSTR);
	void closeHandle();
	bool deleteFile(LPCTSTR);
};

#endif
