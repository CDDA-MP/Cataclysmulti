#ifndef _MAP_H_
#define _MAP_H_
#include <string>

namespace Map
{
    typedef long intp;
    class chunk
    {
    private:
        int m_x;
        int m_y;
        bool m_freeProtected;
    public:
        inline bool isFreeProtected() {return m_freeProtected;}
        inline bool setFreeProtect(bool fp) {m_freeProtected = fp; return fp;}
        chunk(intp x,intp y);
    };
    // The x/y here are the position of a chunk,not tile.
    void init();
    bool loadChunk(intp x,intp y);
    bool freeChunk(intp x,intp y);
    bool isChunkLoaded(intp x,intp y);
    chunk &getChunk(intp x,intp y);
}

#endif