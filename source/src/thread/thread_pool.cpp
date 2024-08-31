#include "thread/thread_pool.hpp"

#include <iostream>

#include "util/profile.hpp"

ThreadPool thread_pool{};

void ThreadPool::WorkerThread(ThreadPool *master) {
  while (master->m_Alive == 1) {
    Task *task = master->GetTask();
    if (task != nullptr) {
      task->Run();

      master->m_Pending_task_count--;

    } else {
      std::this_thread::yield(); // 让出CPU时间片,把线程控制权交给操作系统
    }
  }
}

ThreadPool::ThreadPool(size_t thread_count) {
  m_Alive = 1;

  m_Pending_task_count = 0;

  if (thread_count == 0) {
    thread_count = std::thread::
        hardware_concurrency(); // 如果thread_count为0，则使用硬件线程数
  }

  for (size_t i = 0; i < thread_count; i++) {
    m_Threads.push_back(std::thread(WorkerThread, this)); // 创建线程
  }
}

ThreadPool::~ThreadPool() {
  Wait();

  m_Alive = 0; // 设置线程池不再存活

  for (auto &thead : m_Threads) {
    thead.join(); // 等待每个线程结束
  }
  m_Threads.clear();

  m_Pending_task_count = 0;

  std::cout << "ThreadPool is destroyed" << std::endl;
}

void ThreadPool::ParallelFor(
    size_t width, size_t height,
    const std::function<void(size_t, size_t)> &lambda) {

  PROFILE("ParallelFor");
  
  Guard guard(m_SpinLock);

  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      m_Pending_task_count++;

      m_Tasks.push(new ParallelForTask(x, y, lambda));
    }
  }
}

void ThreadPool::Wait() const {
  // 等待任务队列清空
  while (m_Pending_task_count > 0) {
    std::this_thread::yield(); // 让出CPU时间片,把线程控制权交给操作系统
  }
}

void ThreadPool::AddTask(Task *task) {
  // std::lock_guard<std::mutex> lock(m_Mutex); //
  // 加锁，每次进入这个函数都会加锁，离开这个函数会自动解锁

  Guard guard(m_SpinLock); // 使用Guard类，自动管理锁的获取和释放

  m_Pending_task_count++;

  m_Tasks.push(task);
}

Task *ThreadPool::GetTask() {
  // std::lock_guard<std::mutex> lock(m_Mutex);

  Guard guard(m_SpinLock);

  if (m_Tasks.empty()) {
    return nullptr;
  }

  Task *task = m_Tasks.front();
  m_Tasks.pop();
  return task;
}
