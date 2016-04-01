#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <vector>
namespace Network
{
typedef void (*Interface)(std::vector<char*>);
void call_interface(char* cmd, std::vector<char*> argv);
void init_interfaces();
}
#endif