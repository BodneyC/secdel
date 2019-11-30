#ifndef SECDEL_H
#define SECDEL_H

#include <windows.h>
#include <tchar.h>

#define MEGABYTE 1048576
typedef unsigned char BYTE;

/*----------------------------------------------------
Class Definition
----------------------------------------------------*/
class InputFile
{
    LPCTSTR filename;
    DWORD HI32, LO32, byteswritten;
public:
    HANDLE hInput;
    InputFile(LPCTSTR);
    void open();
    DWORD resetHandle();
    __int64 filesize();
    bool write(BYTE*, __int64);
    bool rename(LPCTSTR, LPCTSTR);
    void close();
    bool remove(LPCTSTR);
};

#endif
