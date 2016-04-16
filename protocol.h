#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <rapidjson/document.h>

namespace Network
{
typedef void (*Interface)(const rapidjson::Document& dom);
void call_interface(const rapidjson::Document& dom);
void init_interfaces();
}

#define CHECKMEMBER(dom,cmd) if(!dom.HasMember(cmd)){return;}

#endif