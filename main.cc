#include <iostream>
#include "secdel.h"
/*----------------------------------------------------
MAIN()
----------------------------------------------------*/
int main(int argc, char const *argv[]) {
	LPCTSTR tempName = argv[1];
	BYTE *zeroArr;

	inputFile cFile (tempName);
	cFile.openFile();
	std::cout << "Input file: " << tempName << '\n';
	if (cFile.hInput == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to open handle to file. Error code: " << GetLastError() << '\n';
		return 1;
	}
	std::cout << "Filesize: " << cFile.getInputInfo() << " bytes\n";
	zeroArr = new BYTE[cFile.getInputInfo()]();
	cFile.writeToFile (zeroArr);

	if (!cFile.deleteFile()) {
		std::cout << "Could not delete file. Error code: " << GetLastError() << '\n';
		return 1;
	} else {
		std::cout << "File deleted (securely)" << '\n';
		std::cout << '\n' << "Program Completed Successfully" << '\n';
	}
	delete[] zeroArr;
	return 0;
}
