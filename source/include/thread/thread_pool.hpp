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
  ParallelForTask(size_t x, size_t y,
                  const std::function<void(size_t, size_t)> &lambda)
      : x(x), y(y), lambda(lambda) {}

  void Run() override { lambda(x, y); }

 private:
  size_t x, y;
  std::function<void(size_t, size_t)> lambda;
};

class ThreadPool {
 public:
  static void WorkerThread(ThreadPool *master);

  ThreadPool(size_t thread_count = 0);
  ~ThreadPool();

  // 并行for循环
  void ParallelFor(size_t width, size_t height,
                   const std::function<void(size_t, size_t)> &lambda);

  // 主动等待所有线程执行完毕
  void Wait() const;

  void AddTask(Task *task);
  Task *GetTask();

 private:
  std::atomic<int> m_Alive;  // 加入线程池是否存活的标志，用于控制线程的退出

  std::vector<std::thread> m_Threads;  // 线程队列

  std::atomic<int> m_Pending_task_count;  // 任务队列中任务的数量

  std::queue<Task *> m_Tasks;  // 任务队列

  // std::mutex m_Mutex;

  SpinLock m_SpinLock{};
};


extern ThreadPool thread_pool;