#include "cache.cpp" // Include the header file for the Cache class
#include<iostream> // Include the input/output stream library
#include<fstream> // Include the file stream library
#include<vector> // Include the vector library
#include<string> // Include the string library
#include<sstream> // Include the string stream library
#include<cmath> // Include the math library
#include<iomanip> // Include the input/output manipulation library
#include<bitset> // Include the bitset library

using namespace std; // Use the standard namespace

// Function to read the access sequence from a file
std::vector<unsigned int> readMemory(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<unsigned int> addresses;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            unsigned int address = std::stoul(line);
            if (address > 0x3FFFFFFF) {
                cerr << "Error: Address exceeds 30 bits: " << address << endl;
                continue;
            }
            addresses.push_back(address);
        }
        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }
    return addresses;
}

int main() {
    // Cache configurations
    unsigned int icacheSize = 32768; // 32 KB for instruction cache
    unsigned int dcacheSize = 32768; // 32 KB for data cache
    unsigned int lineSize = 64; // 64 bytes for both caches
    unsigned int cacheAccessTime = 5; // 5 cycles
    unsigned int memoryAccessTime = 150; // 150 cycles

    // Instantiate caches
    Cache instructionCache(icacheSize, lineSize, cacheAccessTime);
    Cache dataCache(dcacheSize, lineSize, cacheAccessTime);

    // Read the access sequences
    std::string instructionFile = "instruction_access_sequence.txt";
    std::string dataFile = "data_access_sequence.txt";
    std::vector<unsigned int> instructionAccessSequence = readMemory(instructionFile);
    std::vector<unsigned int> dataAccessSequence = readMemory(dataFile);

    // Simulate instruction cache behavior
    std::cout << "Simulating Instruction Cache:\n";
    for (unsigned int address : instructionAccessSequence) {
        instructionCache.access(address,false);
        instructionCache.printStatus();
    }

    // Simulate data cache behavior
    std::cout << "\nSimulating Data Cache:\n";
    for (unsigned int address : dataAccessSequence) {
        dataCache.access(address, false);
        dataCache.printStatus();
    }

    return 0;
}