#include <iostream>

#include "thread_pool.hpp"

class SimpleTask : public Task
{
public:
    void Run() override
    {
        std::cout << "I am a worker thread" << std::this_thread::get_id() << std::endl;
    }
};

int main()
{
    std::cout << "--------------CPU Ray Tracing--------------" << std::endl;
    ThreadPool pool {};
    pool.AddTask(new SimpleTask());
    pool.AddTask(new SimpleTask());
    pool.AddTask(new SimpleTask());
    pool.AddTask(new SimpleTask());

    return 0;
}