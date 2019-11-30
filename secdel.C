#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <tclap/CmdLine.h>
#include "secdel.H"

typedef std::vector<std::string>::const_iterator FnameIter;

struct inputInfo {
    std::string basepath, basename;
    std::vector<std::string> inputStrings;
    int levelVal;
    bool streamVal, renameVal;
} CMDArgs;

/*------------------------------------------------------------
 * RNG dependant
 *------------------------------------------------------------*/

BYTE randomNumber() { return std::rand() & 0xFF; }
char randomChar() { return 'a' + std::rand() % 26; }

/*------------------------------------------------------------
 * Wiping function
 *------------------------------------------------------------*/
void wipeFunction (InputFile &file)
{
    int cnt = file.filesize() / MEGABYTE;
    int mod = file.filesize() % MEGABYTE;

    std::ios fmt(NULL);
    fmt.copyfmt(std::cout);

    BYTE *chunk;
    if (CMDArgs.streamVal)
        chunk = new BYTE[MEGABYTE];
    else
        chunk = new BYTE[file.filesize()];

    for (int levCount = 0; levCount < CMDArgs.levelVal; levCount++) {
        BYTE rand = randomNumber();

        if (CMDArgs.streamVal) {
            memset (chunk, rand, MEGABYTE);
            for (int i = 0; i < cnt; i++)
                file.write(chunk, MEGABYTE);
            file.write(chunk, mod);
        } else {
            memset (chunk, rand, file.filesize());
            file.write(chunk, file.filesize());
        }

        file.resetHandle();
        std::cout << "Pass " << std::setw(2) << levCount
            << ": written value: " << std::hex << std::setfill('0')
            << std::internal << std::showbase << std::setw(4)
            << (int)rand << '\n';
        std::cout.copyfmt(fmt);
    }

    delete[] chunk;
}

/*------------------------------------------------------------
 * Renaming function
 *------------------------------------------------------------*/
std::string rename (InputFile &file, std::string filename)
{
    std::string tempFilename = filename;
    for (int i = 0; i < CMDArgs.levelVal; i++) {
        std::string newFilename(tempFilename.size(), randomChar());
        if (file.rename(tempFilename.c_str(), newFilename.c_str()))
            std::cout << "Pass " << std::setw(2) << i << ": renamed as: "
                << newFilename << '\n';
        tempFilename = newFilename;
    }
    return tempFilename;
}

/*------------------------------------------------------------
 * Reverse iterate the provided file path
 *------------------------------------------------------------*/
void cmdArgsFileProcess (std::string origName)
{
    std::string::reverse_iterator revIt = origName.rbegin();
    CMDArgs.basepath.clear();
    CMDArgs.basename.clear();

    while (*revIt != '\\') {
        CMDArgs.basename.push_back(*revIt);
        revIt++;
    }

    if (revIt > origName.rend()) {
        CMDArgs.basepath = ".\\";
        CMDArgs.basename = origName;
    } else {
        for (; revIt < origName.rend(); revIt++)
            CMDArgs.basepath.push_back(*revIt);
        std::reverse (CMDArgs.basepath.begin(), CMDArgs.basepath.end());
        std::reverse (CMDArgs.basename.begin(), CMDArgs.basename.end());
    }
}

/*------------------------------------------------------------
 * Process a file by name
 *------------------------------------------------------------*/
int processFile(std::string filename) {
    cmdArgsFileProcess(filename);

    std::cout << "\nInput file: " << '\t' << CMDArgs.basename << '\n';

    InputFile file((LPCTSTR)filename.c_str());
    file.open();

    if (file.hInput == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open handle to file. Error code: "
            << GetLastError() << '\n';
        return 1;
    }

    std::cout << "Filesize: " << '\t' << file.filesize()
        << " bytes\n\nWiping File Contents:\n";
    wipeFunction(file);
    file.close();

    if (CMDArgs.renameVal) {
        std::cout << "\nWiping Filename:" << filename << '\n';
        filename = rename(file, filename);
    }

    return file.remove(filename.c_str());
}

/*------------------------------------------------------------
Argument Returning (CMDArgs)
------------------------------------------------------------*/
void tclapArgs (int argn, char const *args[])
{
    try {
        TCLAP::CmdLine cmd("Secure deletion tool for Windows", ' ', "1.05");
        TCLAP::UnlabeledMultiArg<std::string> inputArg("input",
                "Input File-path", true, "String", false);
        TCLAP::ValueArg<int> levelArg("l", "levels",
                "Number of times to wipe", false, 1, "Integer");
        TCLAP::SwitchArg streamSwitch("s", "stream",
                "Wipe in small chunks", cmd, 0);
        TCLAP::SwitchArg renameSwitch("r", "rename",
                "Rename [-l] levels before deletion", cmd, 0);
        cmd.add(inputArg);
        cmd.add(levelArg);
        cmd.parse(argn, args);
        CMDArgs.inputStrings = inputArg.getValue();
        CMDArgs.levelVal = levelArg.getValue();
        CMDArgs.renameVal = renameSwitch.getValue();
        CMDArgs.streamVal = streamSwitch.getValue();
    } catch (TCLAP::ArgException &e) {
        std::cerr << "Error: " << e.error() << " for arg "
            << e.argId() << std::endl;
    }
}

/*----------------------------------------------------
 * MAIN()
 *----------------------------------------------------*/
int main(int argc, char const *argv[])
{
    std::srand(std::time(0));
    tclapArgs(argc, argv);

    std::vector<std::string> fnames = CMDArgs.inputStrings;
    for (FnameIter it = fnames.begin(); it != fnames.end(); it++)
        if (processFile(*it))
            std::cout << "\nFile deleted (securely)" << '\n';
        else
            std::cout << "\nCould not process file. Error code: "
                << GetLastError() << '\n';

    std::cout << '\n' << "Program Completed Successfully" << '\n';
    return 0;
}
