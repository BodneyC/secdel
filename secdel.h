#ifndef SECDEL_H
#define SECDEL_H

#include <windows.h>
#include <tchar.h>

#define SECTOR 512
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
		__int64 getInputInfo();
		bool writeToFile(BYTE*);
		bool deleteFile();
};

#endif
