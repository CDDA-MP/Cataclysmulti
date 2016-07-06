#ifndef _INPUT_H_
#define _INPUT_H_

#include <queue>
namespace Input
{
    class InputHandler
    {
    public:
        bool Inited;
        InputHandler();
        virtual ~InputHandler();
        virtual void Init();
        virtual bool HandleInput(int key);
    };

    extern std::queue<InputHandler*> iqueue;
    void init();
    void end();
    void queue_pushback(InputHandler* handler);
}
#endif