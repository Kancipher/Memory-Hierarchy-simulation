#include "cache.h" // Include the header file for the Cache class
#include<iostream> // Include the input/output stream library
#include<fstream> // Include the file stream library
#include<vector> // Include the vector library
#include<string> // Include the string library
#include<sstream> // Include the string stream library
#include<cmath> // Include the math library
#include<iomanip> // Include the input/output manipulation library
#include<bitset> // Include the bitset library

using namespace std; // Use the standard namespace

// Constructor for the Cache class
Cache::Cache(unsigned int cacheSize, unsigned int lineSize, unsigned int cacheAccessTime)
    : cacheSize(cacheSize), lineSize(lineSize), cacheAccessCycles(cacheAccessTime), accesses(0), hits(0), misses(0) {
    memoryAccessTime = 150; // Set the memory access time to 100 cycles
    numberOfLines = cacheSize / lineSize; // Calculate the number of cache lines
    indexBits = (int)log2(numberOfLines); // Calculate the number of index bits
    displacementBits = (int)log2(lineSize); // Calculate the number of displacement bits
    tagBits= 30 - indexBits - displacementBits; // Calculate the number of tag bits
    writePolicy = WritePolicy::WriteBack; // Set the write policy to write-back
    replacementPolicy = ReplacementPolicy::Allocate; // Set the replacement policy to allocate
    cout<<"number of lines: "<<numberOfLines<<endl;
    cout<<"index bits: "<<indexBits<<endl;
    cout<<"displacement bits: "<<displacementBits<<endl;
    cout<<"tag bits: "<<tagBits<<endl;
    cache.resize(numberOfLines, {false, 0}); // Resize the cache vector and initialize all lines as invalid
}

// Method to calculate the index of the cache line based on the address
unsigned int Cache::getIndex(unsigned int address) {
    return (address >> displacementBits) & (numberOfLines - 1);
}

// Method to calculate the tag of the cache line based on the address
unsigned int Cache::getTag(unsigned int address) {
    return address >> (indexBits + displacementBits);
}

// Method to access the cache
void Cache::access(unsigned int address, bool write) {
    accesses++; // Increment the total number of cache accesses
    unsigned int index = getIndex(address); // Calculate the index of the cache line
    unsigned int tag = getTag(address); // Calculate the tag of the cache line
    if (cache[index].valid && cache[index].tag == tag) { // Check if the cache line is valid and the tag matches
        hits++; // Increment the number of cache hits
        cycles += cacheAccessCycles; // Increment the total number of cycles by the cache access time
        // if (write) {
        //     if (writePolicy == WritePolicy::WriteBack) {
        //         cycles += memoryAccessTime; // Increment the total number of cycles by the memory access time
        //     } else if (writePolicy == WritePolicy::WriteThrough) {
        //         // Perform write-through operation
        //         // Write the data to both the cache line and the memory
        //         cache[index].dirty = true; // Set the cache line as dirty
        //     }
        // }
    } else {
        misses++; // Increment the number of cache misses
        cache[index].valid = true; // Set the cache line as valid
        cache[index].tag = tag; // Set the tag of the cache line
        cycles += (memoryAccessTime + cacheAccessCycles); // Increment the total number of cycles by the memory access time and cache access time
        // if (write) {
        //     if (replacementPolicy == ReplacementPolicy::Allocate) {
        //     // Perform allocate operation
        //     // Allocate a new cache line for the missed address
        //     // You can implement the allocate operation here
        //     } else if (replacementPolicy == ReplacementPolicy::Around) {
        //     // Perform around operation
        //     // Replace the cache line using the around replacement policy
        //     // You can implement the around operation here
        // }
    //      }
    }
}
float Cache::getHitRatio() {
    return (float)hits / accesses; // Calculate the hit ratio
}
float Cache::getMissRatio() {
    return (float)misses / accesses; // Calculate the miss ratio
}
float Cache::getAMAT() {
    return (float)cycles / accesses; // Calculate the average memory access time
}
// Method to print the cache status
void Cache::printStatus() {
    cout << "Cache Status:\n";
    for (unsigned int i = 0; i < numberOfLines; i++) {
        if(cache[i].valid) {
        unsigned int tag = cache[i].tag;
        string binaryTag = bitset<32>(tag).to_string(); // Convert to 32-bit binary string
        binaryTag = binaryTag.substr(32 - tagBits); // Extract the last 'tagBits' bits
        cout << "Line " << i << ": Valid = " << cache[i].valid << ", Tag = " << binaryTag << "\n";
    }
    }
    cout << "Cache Size: " << cacheSize << " bytes" << endl; // Print the cache size
    cout << "Line Size: " << lineSize << " bytes" << endl; // Print the line size
    cout << "Number of Lines: " << numberOfLines << endl; // Print the number of cache lines
    cout << "Cache Access Time: " << cacheAccessCycles << " cycles" << endl; // Print the cache access time
    cout << "Memory Access Time: " << memoryAccessTime << " cycles" << endl; // Print the memory access time
    cout << "Total Accesses: " << accesses << endl; // Print the total number of cache accesses
    cout << "Hits: " << hits << endl; // Print the number of cache hits
    cout << "Misses: " << misses << endl; // Print the number of cache misses
    cout << "Hit Ratio: " << fixed << setprecision(2) << getHitRatio() << endl; // Print the hit ratio
    cout << "Miss Ratio: " << fixed << setprecision(2) << getMissRatio() << endl; // Print the miss ratio
    cout << "Average Memory Access Time (AMAT): " << fixed << setprecision(2) << getAMAT() << " cycles" << endl; // Print the average memory access time
}