#include "Cache.h"
#include <vector>
#include <algorithm>

static int accessCounter = 0;

Cache::Cache(int sets, int ways)
{
    waySize = sets;
    numWays = ways;
    tagShift = 0;

    cache = new Entry *[numWays];

    for (int w = 0; w < numWays; ++w)
    {
        cache[w] = new Entry[waySize];

        for (int s = 0; s < waySize; ++s)
        {
            cache[w][s].tag = 0;
            cache[w][s].valid = false;
            cache[w][s].lastChanged = 0;
        }
    }
}

Cache::~Cache()
{
    for (int w = 0; w < numWays; ++w)
        delete[] cache[w];

    delete[] cache;
}

bool Cache::getValueFromCache(int address)
{
    const int setIndex = address % waySize;
    const int tag = address / waySize;

    for (int w = 0; w < numWays; ++w)
    {
        Entry &entry = cache[w][setIndex];

        if (entry.valid && entry.tag == tag)
        {
            entry.lastChanged = ++accessCounter;
            return true;
        }
    }

    int victim = -1;

    for (int w = 0; w < numWays; ++w)
    {
        if (!cache[w][setIndex].valid)
        {
            victim = w;
            break;
        }
    }

    if (victim == -1)
    {
        victim = 0;

        for (int w = 1; w < numWays; ++w)
        {
            if (cache[w][setIndex].lastChanged <
                cache[victim][setIndex].lastChanged)
            {
                victim = w;
            }
        }
    }

    Entry &entry = cache[victim][setIndex];
    entry.tag = tag;
    entry.valid = true;
    entry.lastChanged = ++accessCounter;

    return false;
}

int Cache::findTag(int address)
{
    return address / waySize;
}

int Cache::findIndex(int address)
{
    return address % waySize;
}