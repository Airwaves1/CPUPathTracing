#pragma once

#include <cstddef>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

// 任务类，所有任务都继承自这个类，重写Run函数来实现具体的任务
class Task
{
public:
    Task() = default;
    virtual ~Task() = default;
    virtual void Run() = 0;
};

class ThreadPool
{
public:
    static void WorkerThread(ThreadPool *master);

    ThreadPool(size_t thread_count = 0);
    ~ThreadPool();

    void AddTask(Task *task);
    Task *GetTask();

private:

    bool m_Alive;   //加入线程池是否存活的标志，用于控制线程的退出

    std::vector<std::thread> m_Threads; //线程队列

    std::list<Task *> m_Tasks;  //任务队列

    std::mutex m_Mutex;
};
