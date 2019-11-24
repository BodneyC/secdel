#include "secdel.H"

// CONSTRUCTOR: Sets filename string
inputFile::inputFile(LPCTSTR stringIn)
{
    filename = stringIn;
}

// Opens the file from string
void inputFile::openFile()
{
    hInput = CreateFile(
        filename, 
        GENERIC_READ | GENERIC_WRITE, 
        0, 
        NULL, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL
    );
}

// Resets the file pointer to 0
DWORD inputFile::resetFilePointer()
{
    return SetFilePointer(
        hInput, 
        0, 
        NULL, 
        FILE_BEGIN
    );
}

// Returns size of input file
__int64 inputFile::getInputInfo()
{
    LO32 = GetFileSize (hInput, &HI32);
    return (__int64)HI32 + (__int64)LO32;
}

// Writes to the file
bool inputFile::writeToFile(BYTE *byteArr, __int64 bytecount)
{
    return WriteFile(
        hInput, 
        byteArr, 
        bytecount, 
        &byteswritten, 
        NULL
    );
}

// Renames the file
bool inputFile::renameFile(LPCTSTR currentname, LPCTSTR newname)
{
    return MoveFileEx(
        currentname, 
        newname, 
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
    );
}

// CLoses the handle to the inputFile
void inputFile::closeHandle()
{
    CloseHandle(hInput);
}

// Deletes the file
bool inputFile::deleteFile (LPCTSTR currentname)
{
    return DeleteFile(currentname);
}
