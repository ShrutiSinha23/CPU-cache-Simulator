#ifndef MY_CACHE_H
#define MY_CACHE_H

struct Entry
{
    int tag;
    bool valid;
    int lastChanged;
};

class Cache
{
public:
    Cache(int, int);
    ~Cache();
    bool getValueFromCache(int);
    static int changeCt;

private:
    Entry **cache;
    int waySize;
    int numWays;
    int tagShift;
    int findTag(int);
    int findIndex(int);
};

#endif