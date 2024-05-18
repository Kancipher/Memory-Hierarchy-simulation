#include "cache.cpp" // Include the header file for the Cache class
#include <iostream> // Include the input/output stream library
#include <fstream> // Include the file stream library
#include <vector> // Include the vector library
#include <string> // Include the string library
#include <sstream> // Include the string stream library
#include <cmath> // Include the math library
#include <iomanip> // Include the input/output manipulation library
#include <bitset> // Include the bitset library

using namespace std; // Use the standard namespace

// Function to remove spaces in a line
string removeSpaces(const string& line) {
    string result;
    for (char c : line) {
        if (c != ' ') {
            result += c;
        }
    }
    return result;
}

// Function to read the access sequence from a file
void readAccess(const string& filename, vector<unsigned int>& instructionAccessSequence, vector<unsigned int>& dataAccessSequence) {
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) { 
            line = removeSpaces(line);
            string binaryString= line.substr(0, line.size() - 2);
            unsigned int address = stoul(binaryString, nullptr, 2);
            if (address > 0x3FFFFFFF) {
                cerr << "Error: Address exceeds 30 bits: " << address << endl;
                continue;
            }
            if (line.at(line.size() - 1) == 'I') {
                instructionAccessSequence.push_back(address);
            }
            else if (line.at(line.size() - 1) == 'D') {
                dataAccessSequence.push_back(address);
            }
            else {
                cerr << "Error: Invalid access type: " << line.at(line.size() - 1) << endl;
                exit(1);
            }
        }
        file.close();
    }
    else {
        cerr << "Error opening file: " << filename << endl;
    }
}

int main() {
    // Cache configurations
    unsigned int icacheSize, dcacheSize, lineSize, cacheAccessTime, memoryAccessTime;

    // Get cache configurations from user input
    cout << "Enter instruction cache size (in bytes): ";
    cin >> icacheSize;
    cout << "Enter data cache size (in bytes): ";
    cin >> dcacheSize;

   
    cout << "Enter cache line size (in bytes): ";
    cin >> lineSize;
     // Check if cache sizes are powers of 2 and not exceeding the cache size
    if ((icacheSize & (icacheSize - 1)) != 0 || (dcacheSize & (dcacheSize - 1)) != 0 || lineSize > icacheSize || dcacheSize < lineSize) {
        cerr << "Error: Cache sizes must be powers of 2 and not exceed the cache size." << endl;
        return 1;
    }
    cout << "Enter cache access time (in cycles): ";
    cin >> cacheAccessTime;
    if (cacheAccessTime < 1 && cacheAccessTime > 10)
    {
        cout << "Invalid number of cycles to access the cache. Must be between 1 and 10." << endl;
        exit(2);
    }
    // Instantiate caches
    Cache instructionCache(icacheSize, lineSize, cacheAccessTime);
    Cache dataCache(dcacheSize, lineSize, cacheAccessTime);
    // Read the access sequences
    string file;
    cout << "Enter access sequence file name: ";
    cin >> file;

    vector<unsigned int> instructionAccessSequence;
    vector<unsigned int> dataAccessSequence;
    readAccess(file, instructionAccessSequence, dataAccessSequence);

    // Simulate instruction cache behavior
    cout << "Simulating Instruction Cache:\n";
    for (int i = 0; i < instructionAccessSequence.size(); i++) {
        unsigned int address = instructionAccessSequence[i];
        instructionCache.access(address);
        instructionCache.printStatus();
    }

    // Simulate data cache behavior
    cout << "\nSimulating Data Cache:\n";
    for (int i = 0; i < dataAccessSequence.size(); i++) {
        unsigned int address = dataAccessSequence[i];
        dataCache.access(address);
        dataCache.printStatus();
    }

    // Print final status of caches
    cout<<"-----------------------------------------------------------------------------"<<endl;
    cout << "\nFinal Instruction Cache Status:\n";
    instructionCache.printStatus();
    cout << "\nFinal Data Cache Status:\n";
    dataCache.printStatus();

    return 0;
}
