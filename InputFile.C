#include "secdel.H"

// CONSTRUCTOR: Sets filename string
InputFile::InputFile(LPCTSTR stringIn)
{
    filename = stringIn;
}

// Opens the file from string
void InputFile::open()
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
DWORD InputFile::resetHandle()
{
    return SetFilePointer(
        hInput, 
        0, 
        NULL, 
        FILE_BEGIN
    );
}

// Returns size of input file
__int64 InputFile::filesize()
{
    LO32 = GetFileSize (hInput, &HI32);
    return (__int64)HI32 + (__int64)LO32;
}

// Writes to the file
bool InputFile::write(BYTE *byteArr, __int64 bytecount)
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
bool InputFile::rename(LPCTSTR currentname, LPCTSTR newname)
{
    return MoveFileEx(
        currentname, 
        newname, 
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
    );
}

// CLoses the handle to the InputFile
void InputFile::close()
{
    CloseHandle(hInput);
}

// Deletes the file
bool InputFile::remove(LPCTSTR currentname)
{
    return DeleteFile(currentname);
}
