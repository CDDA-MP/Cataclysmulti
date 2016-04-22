#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <json11.hpp>
namespace Network
{
typedef void (*Callback)(const json11::Json& dom);
void call_Callback(const json11::Json& json);
void init_Callbacks();
}

#define CHECKMEMBER(json,key) if(json[key].is_null()){return;}

#endif