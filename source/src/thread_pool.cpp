#include "thread_pool.hpp"
#include <iostream>

void ThreadPool::WorkerThread(ThreadPool *master)
{
    while (master->m_Alive)
    {
        Task *task = master->GetTask();
        if (task != nullptr)
        {
            task->Run();
        }
        else
        {
            std::this_thread::yield(); // 让出CPU时间片,把线程控制权交给操作系统
        }
    }
}

ThreadPool::ThreadPool(size_t thread_count)
{
    m_Alive = true;

    if (thread_count == 0)
    {
        thread_count = std::thread::hardware_concurrency(); // 如果thread_count为0，则使用硬件线程数
    }

    for (size_t i = 0; i < thread_count; i++)
    {
        m_Threads.push_back(std::thread(WorkerThread, this)); // 创建线程
    }
}

ThreadPool::~ThreadPool()
{
    // 结束之前,先等待任务队列清空
    while (!m_Tasks.empty())
    {
        std::this_thread::yield(); // 让出CPU时间片,把线程控制权交给操作系统
    }

    m_Alive = false;

    for (auto &thead : m_Threads)
    {
        thead.join(); // 等待每个线程结束
    }
    m_Threads.clear();

    std::cout << "ThreadPool is destroyed" << std::endl;
}

void ThreadPool::AddTask(Task *task)
{
    std::lock_guard<std::mutex> lock(m_Mutex); // 加锁，每次进入这个函数都会加锁，离开这个函数会自动解锁

    m_Tasks.push_back(task);
}

Task *ThreadPool::GetTask()
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    if (m_Tasks.empty())
    {
        return nullptr;
    }

    Task *task = m_Tasks.front();
    m_Tasks.pop_front();
    return task;
}
