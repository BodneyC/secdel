#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm> // std::fill
#include <cstdlib> // std::srand
#include <ctime> // std::time
#include <cstring> // std::time
#include <tclap/CmdLine.h>
#include "secdel.h"

struct inputInfo {
	std::string inputString, pathOnly, filenameOnly, newFilename;
	int levelVal;
	bool streamVal, renameVal;
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
void wipeFunction (inputFile &cFile) {
	BYTE *byteArr, randVal;
	std::ios fmtCout(NULL); // Default formatting saved
	fmtCout.copyfmt(std::cout);

	if (CMDArgs.streamVal) {
		int loopcount = cFile.getInputInfo() / MEGABYTE;
		int loopremainder = cFile.getInputInfo() % MEGABYTE;
		byteArr = new BYTE[MEGABYTE];

		for (int levCount = 0; levCount < CMDArgs.levelVal; levCount++) {
			randVal = getRandomNumber();
			for (int i = 0; i < loopcount; i++) {
				memset (byteArr, randVal, MEGABYTE);
				cFile.writeToFile (byteArr, MEGABYTE);
			}
			randVal = getRandomNumber();
			memset (byteArr, randVal, MEGABYTE);
			cFile.writeToFile (byteArr, loopremainder);

			std::cout.copyfmt(fmtCout);
			std::cout << "Pass " << std::setw(2) << levCount << ": written value: " << std::hex << std::setfill('0') << std::internal << std::showbase << std::setw(4) << (int)randVal << '\n';
		}
	} else {
		byteArr = new BYTE[cFile.getInputInfo()];
		for (int levCount = 0; levCount < CMDArgs.levelVal; levCount++) {
			randVal = getRandomNumber();
			memset (byteArr, randVal, cFile.getInputInfo());
			cFile.writeToFile (byteArr, cFile.getInputInfo());
			cFile.resetFilePointer();
			std::cout.copyfmt(fmtCout);
			std::cout << "Pass " << std::setw(2) << levCount << ": written value: " << std::hex << std::setfill('0') << std::internal << std::showbase << std::setw(4) << (int)randVal << '\n';
		}
	}
	delete[] byteArr;
}
/*------------------------------------------------------------
Renaming function
------------------------------------------------------------*/

// TODO: TEST THIS FUNCTION; ADD A COUT FOR IT
void renameFunction (inputFile &cFile) {
	CMDArgs.newFilename = CMDArgs.pathOnly;
	char ch = 'A';
	for (int o = 0; o < CMDArgs.levelVal || o < 25; o++) {
		for (int i = 0; i < 10; i++) {
			CMDArgs.newFilename.insert(CMDArgs.newFilename.end(), 1, ch);
		}
		cFile.renameFile(CMDArgs.inputString.c_str(), CMDArgs.newFilename.c_str());
		CMDArgs.newFilename.erase(CMDArgs.newFilename.end() - 10, CMDArgs.newFilename.end());
		ch++;
	}
}
/*------------------------------------------------------------
Argument Returning (CMDArgs)
------------------------------------------------------------*/
void returnArgs (int argn, char const *args[]) {
	try {
		TCLAP::CmdLine cmd("SECDEL", ' ', "0.01");
		TCLAP::UnlabeledValueArg<std::string> inputArg("input", "Input File-path", true, "empty", "String", false);
		TCLAP::ValueArg<int> levelArg("l", "levels", "Number of times to wipe", false, 1, "Integer");
		TCLAP::SwitchArg streamSwitch("s", "stream", "Wipe in small chunks", cmd, 0);
		TCLAP::SwitchArg renameSwitch("r", "rename", "Rename [-l] levels before deletion", cmd, 0);
		cmd.add(inputArg);
		cmd.add(levelArg);
		cmd.parse(argn, args);
		CMDArgs.inputString = inputArg.getValue();
		CMDArgs.levelVal = levelArg.getValue();
		CMDArgs.renameVal = renameSwitch.getValue();
		CMDArgs.streamVal = streamSwitch.getValue();
	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}
/*------------------------------------------------------------
Reverse iterate the provided file path
------------------------------------------------------------*/
void retPathname () {
	std::string::reverse_iterator revIt = CMDArgs.inputString.rbegin();
	while (*revIt != '\\') {
		revIt++;
	}
	if (revIt > CMDArgs.inputString.rend()) {
		CMDArgs.pathOnly = ".\\";
	} else {
		for (; revIt < CMDArgs.inputString.rend(); revIt++) {
			CMDArgs.pathOnly.push_back(*revIt);
		}
		std::reverse (CMDArgs.pathOnly.begin(), CMDArgs.pathOnly.end());
	}
}
/*------------------------------------------------------------
Reverse iterate the provided file path
------------------------------------------------------------*/
void retFilename () {
	std::string::reverse_iterator revIt = CMDArgs.inputString.rbegin();
	while (*revIt != '\\') {
		CMDArgs.filenameOnly.push_back(*revIt);
		revIt++;
	}
	if (revIt > CMDArgs.inputString.rend()) {
		CMDArgs.filenameOnly = CMDArgs.inputString;
	} else {
		std::reverse (CMDArgs.filenameOnly.begin(), CMDArgs.filenameOnly.end());
	}
}
/*----------------------------------------------------
MAIN()
----------------------------------------------------*/
int main(int argc, char const *argv[]) {
	std::srand(std::time(0));
	returnArgs(argc, argv);
	retPathname(); retFilename();

	LPCTSTR tempName = CMDArgs.inputString.c_str();
	inputFile cFile (tempName);
	cFile.openFile();
	std::cout << "\nInput file: " << '\t' << CMDArgs.filenameOnly << '\n';
	if (cFile.hInput == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to open handle to file. Error code: " << GetLastError() << '\n';
		return 1;
	}
	std::cout << "Filesize: " << '\t' << cFile.getInputInfo() << " bytes\n\n";
	wipeFunction(cFile);
	if (CMDArgs.renameVal) renameFunction(cFile);

	// if (!cFile.deleteFile()) {
	// 	std::cout << "Could not delete file. Error code: " << GetLastError() << '\n';
	// 	return 1;
	// } else {
	// 	std::cout << "File deleted (securely)" << '\n';
		std::cout << '\n' << "Program Completed Successfully" << '\n';
	// }
	return 0;
}
