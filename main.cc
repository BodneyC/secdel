#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm> // std::fill
#include <cstdlib> // std::srand
#include <ctime> // std::time
#include <tclap/CmdLine.h>
#include "secdel.h"

struct inputInfo {
	std::string inputString;
	int levelVal;
	bool streamVal;
} CMDArgs;
/*------------------------------------------------------------
RNG dependant
------------------------------------------------------------*/
BYTE getRandomNumber(){
    return std::rand() & 0xFF;
}
/*------------------------------------------------------------
Wiping function
------------------------------------------------------------*/
void wipefunction (inputFile &cFile) {
	BYTE *byteArr, randVal;
	std::ios init(NULL); // Default formatting saved
	init.copyfmt(std::cout);


	// TEST STREAM TAG!!!
	// RENAME FILE


	if (CMDArgs.streamVal) {
		int loopcount = cFile.getInputInfo() / MEGABYTE;
		int loopremainder = cFile.getInputInfo() % MEGABYTE;
		byteArr = new BYTE[MEGABYTE];

		for (int levCount = 0; levCount < CMDArgs.levelVal; levCount++) {
			for (int i = 0; i < loopcount; i++) {
				randVal = getRandomNumber();
				memset (byteArr, randVal, MEGABYTE);
				cFile.writeToFile (byteArr, MEGABYTE);
			}
			randVal = getRandomNumber();
			memset (byteArr, randVal, MEGABYTE);
			cFile.writeToFile (byteArr, loopremainder);

			std::cout.copyfmt(init);
			std::cout << "Pass " << std::setw(2) << levcount << ": written value: " << std::hex << std::setfill('0') << std::internal << std::showbase << std::setw(4) << (int)randVal << '\n';
		}
	} else {
		byteArr = new BYTE[cFile.getInputInfo()];
		for (int levcount = 0; levcount < CMDArgs.levelVal; levcount++) {
			randVal = getRandomNumber();
			memset (byteArr, randVal, cFile.getInputInfo());
			cFile.writeToFile (byteArr, cFile.getInputInfo());
			cFile.resetFilePointer();
			std::cout.copyfmt(init);
			std::cout << "Pass " << std::setw(2) << levcount << ": written value: " << std::hex << std::setfill('0') << std::internal << std::showbase << std::setw(4) << (int)randVal << '\n';
		}
	}
	delete[] byteArr;
}
/*------------------------------------------------------------
Argument Returning (CMDArgs)
------------------------------------------------------------*/
void returnArgs (int argn, char const *args[]) {
	try {
		TCLAP::CmdLine cmd("SECDEL", ' ', "0.01");
		TCLAP::ValueArg<std::string> inputArg("i", "input", "Input File-path", true, "empty", "String");
		TCLAP::ValueArg<int> levelArg("l", "levels", "Number of times to wipe", false, 1, "Integer");
		TCLAP::SwitchArg streamSwitch("s", "stream", "Wipe in small chunks", cmd, 0);
		cmd.add(inputArg);
		cmd.add(levelArg);
		cmd.parse(argn, args);
		CMDArgs.inputString = inputArg.getValue();
		CMDArgs.levelVal = levelArg.getValue();
		CMDArgs.streamVal = streamSwitch.getValue();
	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}
/*----------------------------------------------------
MAIN()
----------------------------------------------------*/
int main(int argc, char const *argv[]) {
	std::srand(std::time(0));
	returnArgs(argc, argv);

	LPCTSTR tempName = CMDArgs.inputString.c_str();
	inputFile cFile (tempName);
	cFile.openFile();
	std::cout << "\nInput file: " << '\t' << tempName << '\n';
	if (cFile.hInput == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to open handle to file. Error code: " << GetLastError() << '\n';
		return 1;
	}
	std::cout << "Filesize: " << '\t' << cFile.getInputInfo() << " bytes\n\n";
	wipefunction(cFile);

	// if (!cFile.deleteFile()) {
	// 	std::cout << "Could not delete file. Error code: " << GetLastError() << '\n';
	// 	return 1;
	// } else {
	// 	std::cout << "File deleted (securely)" << '\n';
	// 	std::cout << '\n' << "Program Completed Successfully" << '\n';
	// }
	return 0;
}
