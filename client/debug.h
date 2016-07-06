#ifndef _DEBUG_H_
    #define _DEBUG_H_

    #ifdef DEBUGMODE
        #define DebugInfo(info) writeDebugInfo(info)
    #else
        #define DebugInfo(info)
    #endif

#endif