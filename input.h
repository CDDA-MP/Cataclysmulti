#ifndef _INPUT_H_
#define _INPUT_H_

#include <vector>
namespace Input
{
class InputHandler {
public:
    bool Inited;
    InputHandler();
    virtual ~InputHandler();
    virtual void Init();
    virtual bool HandleInput(int key);
};

extern std::vector<InputHandler*> queue;
void init();
void end();
void queue_pushback(InputHandler* handler);
}
#endif