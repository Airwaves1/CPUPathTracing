#include "thread/thread_pool.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>

ThreadPool thread_pool{};

void ThreadPool::WorkerThread(ThreadPool *master) {
  while (master->m_Alive == 1) {
    Task *task = master->GetTask();
    if (task != nullptr) {
      task->Run();
      delete task;
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

  std::cout << "ThreadPool is destroyed" << std::endl;
}

void ThreadPool::ParallelFor(size_t width, size_t height,
                             const std::function<void(size_t, size_t)> &lambda,
                             bool complex) {

  Guard guard(m_SpinLock);

  float chunk_width_float =
      static_cast<float>(width) / sqrt(m_Threads.size());
  float chunk_height_float =
      static_cast<float>(height) / sqrt(m_Threads.size());

  if (complex) {
    // 如果是复杂任务，那么每个任务的大小为16
    chunk_width_float /= sqrt(16);
    chunk_height_float /= sqrt(16);
  }

  size_t chunk_width = std::ceil(chunk_width_float);
  size_t chunk_height = std::ceil(chunk_height_float);

  for (size_t x = 0; x < width; x += chunk_width) {
    for (size_t y = 0; y < height; y += chunk_height) {
      m_Pending_task_count++;
      if (x + chunk_width > width) {
        chunk_width = width - x;
      }
      if (y + chunk_height > height) {
        chunk_height = height - y;
      }
      m_Tasks.push(
          new ParallelForTask(x, y, chunk_width, chunk_height, lambda));
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
