#pragma once

#include <cstddef>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

#include "spinlock.hpp"

// 任务类，所有任务都继承自这个类，重写Run函数来实现具体的任务
class Task {
public:
  Task() = default;
  virtual ~Task() = default;
  virtual void Run() = 0;
};

class ParallelForTask : public Task {
public:
  ParallelForTask(size_t x, size_t y, size_t chunk_width, size_t chunk_height,
                  const std::function<void(size_t, size_t)> &lambda)
      : m_x(x), m_y(y), m_ChunkWidth(chunk_width), m_ChunkHeight(chunk_height),
        lambda(lambda) {}

  void Run() override {
    for (size_t dix_x = 0; dix_x < m_ChunkWidth; dix_x++) {
      for (size_t dix_y = 0; dix_y < m_ChunkHeight; dix_y++) {
        lambda(m_x + dix_x, m_y + dix_y);
      }
    }
  }

private:
  size_t m_x, m_y;
  size_t m_ChunkWidth, m_ChunkHeight;
  std::function<void(size_t, size_t)> lambda;
};

class ThreadPool {
public:
  static void WorkerThread(ThreadPool *master);

  ThreadPool(size_t thread_count = 0);
  ~ThreadPool();

  // 并行for循环
  void ParallelFor(size_t width, size_t height,
                   const std::function<void(size_t, size_t)> &lambda, bool complex = true);

  // 主动等待所有线程执行完毕
  void Wait() const;

  void AddTask(Task *task);
  Task *GetTask();

private:
  std::atomic<int> m_Alive; // 加入线程池是否存活的标志，用于控制线程的退出

  std::vector<std::thread> m_Threads; // 线程队列

  std::atomic<int> m_Pending_task_count; // 任务队列中任务的数量

  std::queue<Task *> m_Tasks; // 任务队列

  // std::mutex m_Mutex;

  SpinLock m_SpinLock{};
};

extern ThreadPool thread_pool;