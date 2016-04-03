#ifndef _INPUT_H_
#define _INPUT_H_
#include <uv.h>
namespace Input
{
void init(uv_loop_t* loop);
void end();
}

#endif