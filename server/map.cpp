#include <unordered_map>

#include "map.h"

namespace Map
{
    chunk::chunk(intp x,intp y)
    {
        // TODO: Load chunk data from disk.
    }

    std::unordered_map<intp,std::unordered_map<intp,chunk*>> chunks;
    void init()
    {

    }

    bool loadChunk(intp x,intp y)
    {
        if (chunks[x][y] == nullptr)
        {
            chunks[x][y] = new chunk(x,y);
            return true;
        }

        return false;
    }

    bool freeChunk(intp x,intp y)
    {
        chunk* ptr = chunks[x][y];

        if (ptr != nullptr && !ptr->isFreeProtected())
        {
            delete ptr;
            chunks[x][y] = nullptr;
            return true;
        }

        return false;
    }

    bool isChunkLoaded(intp x,intp y)
    {
        return chunks[x][y] == nullptr;
    }

    chunk &getChunk(intp x,intp y)
    {
        return *chunks[x][y];
    }
}