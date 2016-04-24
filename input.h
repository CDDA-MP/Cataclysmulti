#ifndef _INPUT_H_
#define _INPUT_H_

#include <vector>
namespace Input
{
class InputHandler {
public:
    virtual bool HandleInput(int key) {
        return true;
    }
};
extern std::vector<InputHandler*> queue;
void init();
void end();
}
#endif