#include <vector>
using namespace std;

struct CacheLine
{
    int tag;
    bool dirty;
    bool valid;
    

};
enum WritePolicy
{
    WriteBack,
    WriteThrough
};
enum ReplacementPolicy
{
    Allocate,
    Around
};

class Cache
{
private:
    unsigned int cacheSize;
    unsigned int lineSize;
    unsigned int numberOfLines;
    unsigned int cacheAccessCycles;
    unsigned int memoryAccessTime;
    unsigned int accesses;
    unsigned int hits;
    unsigned int misses;
    unsigned int indexBits;
    unsigned int displacementBits;
    unsigned int tagBits;
    unsigned int cycles;
    WritePolicy writePolicy;
    ReplacementPolicy replacementPolicy;
    vector<CacheLine> cache;
    
    unsigned int getIndex(unsigned int address);
    unsigned int getTag(unsigned int address);

public:
    Cache(unsigned int cacheSize, unsigned int lineSize, unsigned int cacheAccessTime);
    void access(unsigned int address,bool write);
    void printStatus();
    float getHitRatio();
    float getMissRatio();
    float getAMAT();
    
};
