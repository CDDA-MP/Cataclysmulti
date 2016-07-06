#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <json11.hpp>

namespace Network
{
    namespace Protocol
    {
        typedef void (*Callback)(const json11::Json& json);
        void call_Callback(const json11::Json& json);
        namespace Out
        {
            void sendmeta();
            void sendInfo();
        }
    }
}

#endif