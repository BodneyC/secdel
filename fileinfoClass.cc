#include "secdel.h"
// CONSTRUCTOR: Sets filename string
inputFile::inputFile (LPCTSTR stringIn) {
	filename = stringIn;
}
// Opens the file from string
void inputFile::openFile () {
	hInput = CreateFile (	filename,
												GENERIC_READ | GENERIC_WRITE,
												0,
												NULL,
												OPEN_EXISTING,
												FILE_ATTRIBUTE_NORMAL,
												NULL
											);
}
// Returns size of input file
__int64 inputFile::getInputInfo () {
	LO32 = GetFileSize (hInput, &HI32);
	return (__int64)HI32 + (__int64)LO32;
}
// Writes to the file
bool inputFile::writeToFile (BYTE *zeroArr) {
	return WriteFile (	hInput,
											zeroArr,
											this->getInputInfo(),
											&byteswritten,
											NULL
										);
}
// Deletes the file
bool inputFile::deleteFile () {
	CloseHandle(hInput);
	return DeleteFile(filename);
}
